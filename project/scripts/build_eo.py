#! /usr/bin/python3

import os
import sys
import hashlib
import subprocess
import re as regex

# Our scripts
import settings
import tools


class EOBuilder(object):

    def __init__(self, hash_name=''):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.hash_name = hash_name
        self.hashes = settings.get_cache_hash(self.hash_name)
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = 'target/eo-foreign.csv'

    def build(self):
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        eo_files = tools.search_files_by_pattern('eo', '*.eo', recursive=True)

        if self.is_good_for_recompilation():
            print('Recompilation eo project start\n')
            result = tools.thread_pool().map(self.check_file_hash, eo_files)
            data_for_recompile, self.hashes = parse_result(result)
            self.prepare_for_recompile(data_for_recompile)
            subprocess.run('mvn compile', shell=True)
        else:
            result = tools.thread_pool().map(self.check_file_hash, eo_files)
            _, self.hashes = parse_result(result)
            print('Full eo project compilation start\n')
            subprocess.run('mvn clean compile', shell=True)

        os.chdir(original_path)
        settings.set_cache_hash(self.hash_name, self.hashes)

    def is_good_for_recompilation(self):
        if os.path.exists(self.path_to_foreign_objects):
            print('First eo project building')
            return False

        if is_actual_object_version(self.path_to_foreign_objects, self.current_version):
            print('Compilation on last version detect')
            return False

        if self.hashes is None:
            print('Hash not found')
            return False

        return True

    def check_file_hash(self, file):
        with open(file, 'r') as f:
            package = f.readline()
        package = package.replace('+package', '').strip()
        object_name = f'{package}{tools.get_file_name(file)}'
        new_hash = get_file_hash(file)
        is_equal = self.hashes.get(object_name) == new_hash
        return file, is_equal, object_name, new_hash

    def prepare_for_recompile(self, data):
        for step in ['01-parse', '02-steps', '03-optimize']:
            for _, file in data:
                file = file.replace(self.path_to_eo_project, '')
                os.remove(os.path.join(f'target/{step}', file))
        with open(self.path_to_foreign_objects, 'r') as f:
            data = f.readlines()
        for i, line in enumerate(data):
            result = regex.search(r'(?<=id:).*(?=\tversion:0.0.0$)', line)
            if result:
                for object_name, _ in data:
                    if object_name == result.group():
                        data[i] = ''
                        break
        with open(self.path_to_foreign_objects, 'w') as f:
            f.writelines(data)


def parse_result(result):
    hashes = {}
    files_for_recompile = []
    for file, is_equal, object_name, new_hash in result:
        hashes[object_name] = new_hash
        if not is_equal:
            files_for_recompile.append((object_name, file))
    return files_for_recompile, hashes


def is_actual_object_version(path_to_objects, current_version):
    print('Check version of compiled eo objects')
    with open(path_to_objects, 'r') as f:
        for line in f:
            result = regex.search(r'(?<=version:)\d+\.\d+.\d+$', line)
            compare = tools.version_compare(current_version, result.group())
            if result and compare <= 0:
                return True
    return False


def get_file_hash(file):
    file_chunk_size = settings.get_setting('file_chunk_size')
    file_hash = hashlib.md5()
    with open(file, 'rb') as f:  # rb - read bytes
        chunk = f.read(file_chunk_size)
        while len(chunk) > 0:
            file_hash.update(chunk)
            chunk = f.read(chunk)
    return file_hash.hexdigest()


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        EOBuilder().build()
    else:
        EOBuilder(sys.argv[1]).build()
