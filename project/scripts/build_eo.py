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
import sys
import csv
import argparse
import subprocess
from os import chdir
from os import sep as os_sep
from pathlib import Path

# Our scripts
import tools
import settings


class EOBuilder(object):

    def __init__(self, transpilation_units: list[dict]):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = settings.get_setting('path_to_foreign_objects')
        self.path_to_eo = settings.get_setting('path_to_eo')
        self.path_to_eo_parse = settings.get_setting('path_to_eo_parse')
        self.transpilation_units = transpilation_units
        self.errors = set()
        self.error_result = {}
        original_path = Path.cwd()
        chdir(self.path_to_eo_project)
        self.can_recompile = self.is_recompilation()
        chdir(original_path)

    def build(self) -> (set[dict], dict):
        original_path = Path.cwd()
        chdir(self.path_to_eo_project)
        cmd = ['mvn'] if self.can_recompile else ['mvn', 'clean']
        cmd.extend(['compile', '-D', 'jansi.force=true', '-D', 'style.color=always'])
        tools.pprint('\n', ' '.join(cmd), '\n')
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, text=True)
        for line in process.stdout:
            if line:
                print(line.rstrip())
                if 'error:' in line:
                    self.handle_eo_error(line)
            elif process.poll() is not None:
                break
        chdir(original_path)
        if process.poll():
            exit('compilation failed')
        return self.errors, self.error_result

    def is_recompilation(self) -> bool:
        if not self.path_to_foreign_objects.exists():
            tools.pprint('Compilation information not found', status=tools.WARNING)
            return False

        tools.pprint('Compilation information found', status=tools.PASS)
        if not self.is_actual_object_version():
            tools.pprint('Old version detected', status=tools.WARNING)
            return False

        tools.pprint('Latest version detected', status=tools.PASS)
        eo_src_files = tools.search_files_by_patterns(self.path_to_eo, {'*.eo'}, recursive=True)
        eo_src_files = {str(x).replace(str(self.path_to_eo), '', 1).replace('.eo', '', 1) for x in eo_src_files}
        project_eo_files = tools.search_files_by_patterns(self.path_to_eo_parse, {'*.xmir'},
                                                          recursive=True, filters={'!org/eolang'})
        project_eo_files = {str(x).replace(str(self.path_to_eo_parse), '', 1).replace('.xmir', '', 1) for x in
                            project_eo_files}
        difference = project_eo_files - eo_src_files
        tools.pprint()
        if difference:
            tools.pprint('EO project files are incompatible', status=tools.WARNING)
            tools.pprint(f'The following files may have been deleted: {sorted(difference, key=str.casefold)}', '\n')
            return False

        tools.pprint('EO project files are compatible', status=tools.PASS)
        return True

    def is_actual_object_version(self) -> bool:
        tools.pprint('\n', 'Check version of compiled eo objects', '\n')
        with open(self.path_to_foreign_objects) as f:
            for package in csv.DictReader(f):
                if package['version'] in ['*.*.*', '0.0.0']:
                    continue
                if tools.version_compare(self.current_version, package['version']) == 0:
                    return True
        return False

    def handle_eo_error(self, message) -> None:
        file, error = message.split(os_sep, maxsplit=1)[1].split('with error:', maxsplit=1)
        file, error = file.strip(), error.strip()
        for unit in self.transpilation_units:
            if unit['unique_name'] in file:
                if error not in self.error_result:
                    self.error_result[error] = {}
                self.error_result[error][unit['unique_name']] = set()
                self.errors.add(unit['unique_name'])
                return


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for building eo project in the result dir',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    EOBuilder([]).build()
