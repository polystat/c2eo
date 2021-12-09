#! /usr/bin/python3

import os
import sys
import json
import subprocess

# Our scripts
import settings
import tools


class EOBuilder(object):

    def __init__(self):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = 'target/eo-foreign.json'
        self.chunk_size = settings.get_setting('file_chunk_size')

    def build(self):
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        if self.is_good_for_recompilation():
            print('\nRecompilation eo project start\n')
            subprocess.run('mvn compile', shell=True)
        else:
            print('Full eo project compilation start\n')
            subprocess.run('mvn clean compile', shell=True)
        os.chdir(original_path)

    def is_good_for_recompilation(self):
        if not os.path.exists(self.path_to_foreign_objects):
            print('\nCompile files not found')
            return False
        else:
            print('\nCompile files detected')

        if not self.is_actual_object_version():
            print('\nCompilation on old version detected')
            return False
        else:
            print('Compilation on latest version detected')

        eo_src_files = tools.search_files_by_pattern('eo/c2eo/src', '*.eo')
        eo_src_files = set(map(tools.get_file_name, eo_src_files))
        project_eo_files = tools.search_files_by_pattern('target/01-parse/c2eo/src', '*.xmir')
        project_eo_files = set(map(tools.get_file_name, project_eo_files))
        if project_eo_files - eo_src_files:
            print('\nEO project files are incompatible')
            return False
        else:
            print('\nEO project files are compatible')

        return True

    def is_actual_object_version(self):
        print('\nCheck version of compiled eo objects')
        with open(self.path_to_foreign_objects) as f:
            data = json.load(f)
        for token in data:
            if token['version'] not in ['*.*.*', '0.0.0']:
                compare = tools.version_compare(self.current_version, token['version'])
                if compare <= 0:
                    return True
        return False


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    EOBuilder().build()
