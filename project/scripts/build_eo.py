#! /usr/bin/python3

import os
import sys
import json
import hashlib
import subprocess

# Our scripts
import settings
import tools


class EOBuilder(object):

    def __init__(self, hash_name=''):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = 'target/eo-foreign.json'
        self.chunk_size = settings.get_setting('file_chunk_size')
        self.hash_name = hash_name
        self.hashes = settings.get_cache_hash(self.hash_name)

    def build(self):
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        eo_files = tools.search_files_by_pattern('eo', '*.eo', recursive=True)
        if self.is_good_for_recompilation():
            print('\nRecompilation eo project start\n')
            result = tools.thread_pool().map(self.check_file_hash, eo_files)
            objects, files, self.hashes = parse_result(result)
            self.prepare_for_recompile(objects, files)
            subprocess.run('mvn compile', shell=True)
        else:
            result = tools.thread_pool().map(self.check_file_hash, eo_files)
            _, _, self.hashes = parse_result(result)
            print('Full eo project compilation start\n')
            subprocess.run('mvn clean compile', shell=True)

        os.chdir(original_path)
        settings.set_cache_hash(self.hash_name, self.hashes)

    def is_good_for_recompilation(self):
        if not os.path.exists(self.path_to_foreign_objects):
            print('\nCompile files not found')
            return False
        else:
            print('\nCompile files detected')

        if not is_actual_object_version(self.path_to_foreign_objects, self.current_version):
            print('\nCompilation on old version detected')
            return False
        else:
            print('Compilation on latest version detected')

        if not self.hashes:
            print('\nHash not found')
            return False
        else:
            print('Hash found')

        return True

    def check_file_hash(self, file):
        with open(file, 'r') as f:
            package = f.readline()
        package = package.replace('+package', '').strip()
        object_name = f'{package}.{tools.get_file_name(file)}'
        new_hash = get_file_hash(file, self.chunk_size)
        is_equal = self.hashes.get(object_name) == new_hash
        return file, is_equal, object_name, new_hash

    def prepare_for_recompile(self, objects, files):
        for file in files:
            file = file[3:].replace('.eo', '.xmir')
            os.remove(os.path.join(f'target/01-parse', file))
            os.remove(os.path.join(f'target/03-optimize', file))
        with open(self.path_to_foreign_objects) as f:
            data = json.load(f)
        for token in data:
            if token['id'] in objects:
                data.remove(token)
        print(data)
        with open(self.path_to_foreign_objects, 'w') as f:
            json.dump(data, f)


def parse_result(result):
    hashes = {}
    objects_for_recompile = []
    files_for_recompile = []
    for file, is_equal, object_name, new_hash in result:
        hashes[object_name] = new_hash
        if not is_equal:
            objects_for_recompile.append(object_name)
            files_for_recompile.append(file)
    print(f'Objects for recompilation: {objects_for_recompile}\n')
    return objects_for_recompile, files_for_recompile, hashes


def is_actual_object_version(path_to_objects, current_version):
    print('\nCheck version of compiled eo objects')
    with open(path_to_objects) as f:
        data = json.load(f)
    for token in data:
        if token['version'] not in ['*.*.*', '0.0.0']:
            compare = tools.version_compare(current_version, token['version'])
            if compare <= 0:
                return True
    return False


def get_file_hash(file, chunk_size):
    file_hash = hashlib.md5()
    with open(file, 'rb') as f:  # rb - read bytes
        chunk = f.read(chunk_size)
        while len(chunk) > 0:
            file_hash.update(chunk)
            chunk = f.read(chunk_size)
    return file_hash.hexdigest()


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        EOBuilder().build()
    else:
        EOBuilder(sys.argv[1]).build()
