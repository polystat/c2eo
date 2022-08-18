#! /usr/bin/python3

"""
The MIT License (MIT)

Copyright (c) 2021-2022 c2eo team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
import sys
import subprocess

# Our scripts
import tools
import settings


class EOBuilder(object):

    def __init__(self, transpilation_units):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = settings.get_setting('path_to_foreign_objects')
        self.path_to_eo = settings.get_setting('path_to_eo')
        self.path_to_eo_parse = settings.get_setting('path_to_eo_parse')
        self.transpilation_units = transpilation_units
        self.errors = set()
        self.error_result = {}

    def build(self):
        tools.pprint('Compilation start\n')
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        result = self.is_recompilation()
        tools.pprint(f'\n{"Recompilation eo project starts" if result else "Full eo project compilation starts"}\n')
        cmd = f'mvn {"" if result else "clean"} compile'
        process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, text=True)
        for line in process.stdout:
            if line:
                print(line.replace('\x1b', '\033'), end='')
                if 'error:' in line:
                    self.handle_eo_error(line)
            elif process.poll() is not None:
                break
        os.chdir(original_path)
        if process.poll():
            exit('compilation failed')
        return self.errors, self.error_result

    def is_recompilation(self):
        if not os.path.exists(self.path_to_foreign_objects):
            tools.pprint('Compile dir not found', status=tools.WARNING)
            return False

        tools.pprint('Compile dir found', status=tools.PASS)
        if not self.is_actual_object_version():
            tools.pprint('Old version detected', status=tools.WARNING)
            return False

        tools.pprint('Latest version detected', status=tools.PASS)
        eo_src_files = tools.search_files_by_patterns(self.path_to_eo, ['*.eo'], recursive=True)
        eo_src_files = set(map(lambda x: x.replace(self.path_to_eo, '', 1).replace('.eo', '', 1), eo_src_files))
        project_eo_files = tools.search_files_by_patterns(self.path_to_eo_parse, ['*.xmir'],
                                                          recursive=True, filters=['!org/eolang'])
        project_eo_files = set(map(lambda x: x.replace(self.path_to_eo_parse, '', 1).replace('.xmir', '', 1),
                                   project_eo_files))
        difference = project_eo_files - eo_src_files
        tools.pprint()
        if difference:
            tools.pprint('EO project files are incompatible', status=tools.WARNING)
            tools.pprint(f'The following files may have been deleted: {sorted(difference, key=str.casefold)}\n')
            return False

        tools.pprint('EO project files are compatible', status=tools.PASS)
        return True

    def is_actual_object_version(self):
        tools.pprint('\nCheck version of compiled eo objects\n')
        data = tools.read_file_as_dictionary(self.path_to_foreign_objects)
        for package in data:
            if package['version'] not in ['*.*.*', '0.0.0']:
                compare = tools.version_compare(self.current_version, package['version'])
                if compare <= 0:
                    return True
        return False

    def handle_eo_error(self, message):
        file, error = message.split('/', maxsplit=1)[1].split('with error:', maxsplit=1)
        file, error = file.strip(), error.strip()
        for unit in self.transpilation_units:
            if unit['unique_name'] in file:
                if error not in self.error_result:
                    self.error_result[error] = {}
                self.error_result[error][unit['unique_name']] = set()
                self.errors.add(unit['unique_name'])
                return


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    EOBuilder([]).build()
