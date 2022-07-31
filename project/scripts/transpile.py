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
import time
import shutil
import argparse
import subprocess
import re as regex

# Our scripts
import tools
import settings
import build_c2eo
import clean_before_transpilation


class Transpiler(object):

    def __init__(self, path_to_c_files, skips_file_name, need_to_prepare_c_code=True):
        self.filters = None
        if os.path.isfile(path_to_c_files):
            self.filters = [os.path.split(path_to_c_files)[1]]
            path_to_c_files = os.path.dirname(path_to_c_files)
        self.skips = settings.get_skips(skips_file_name)
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_c2eo_transpiler = settings.get_setting('path_to_c2eo_transpiler')
        self.path_to_c_files = os.path.join(os.path.abspath(path_to_c_files), '')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_eo_src = os.path.join(os.path.abspath(settings.get_setting('path_to_eo_src')), '')
        self.path_to_eo_external = os.path.join(os.path.abspath(settings.get_setting('path_to_eo_external')), '')
        self.plug_code = settings.get_meta_code('plug')
        self.result_dir_name = settings.get_setting('result_dir_name')
        self.run_sh_code = settings.get_meta_code('run.sh')
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.transpilation_units = []
        self.replaced_path = f'{os.path.split(self.path_to_c_files[:-1])[0]}{os.sep}'
        self.files_handled_count = 0
        self.files_count = 0
        self.ignored_transpilation_warnings = settings.get_setting('ignored_transpilation_warnings')
        if not self.ignored_transpilation_warnings:
            self.ignored_transpilation_warnings = []

    def transpile(self):
        start_time = time.time()
        build_c2eo.main(self.path_to_c2eo_build)
        tools.pprint('\nTranspilation start\n')
        clean_before_transpilation.main(self.path_to_c_files)
        c_files = tools.search_files_by_patterns(self.path_to_c_files, ['*.c'], filters=self.filters, recursive=True,
                                                 print_files=True)
        self.files_count = len(c_files)
        original_path = os.getcwd()
        os.chdir(self.path_to_c2eo_build)
        tools.pprint('\nTranspile files:\n', slowly=True)
        tools.print_progress_bar(0, self.files_count)
        with tools.thread_pool() as threads:
            self.transpilation_units = list(threads.map(self.start_transpilation, c_files))
        result = self.group_transpilation_results()
        is_failed = sum(map(len, result[tools.EXCEPTION].values()))
        tools.pprint_result('TRANSPILE', len(self.transpilation_units), int(time.time() - start_time), result,
                            is_failed)
        if is_failed:
            exit(f'transpilation failed')

        self.remove_unused_eo_files()
        self.move_transpiled_files()
        self.move_aliases()
        if len(c_files) == 1:
            self.generate_run_sh(self.transpilation_units[0]['full_name'])
        tools.pprint('\nTranspilation done\n')
        os.chdir(original_path)
        return self.transpilation_units

    def start_transpilation(self, c_file):
        path, name, _ = tools.split_path(c_file, with_end_sep=True)
        rel_c_path = path.replace(self.replaced_path, '')
        full_name = f'{tools.make_name_from_path(rel_c_path)}.{name}'
        prepared_c_file, result_path = self.prepare_c_file(path, name, c_file)
        eo_file = f'{full_name}.eo'
        transpile_cmd = f'{self.path_to_c2eo_transpiler}c2eo {prepared_c_file} {eo_file}'
        result = subprocess.run(transpile_cmd, shell=True, capture_output=True, text=True)
        self.files_handled_count += 1
        tools.print_progress_bar(self.files_handled_count, self.files_count)
        return {'c_file': c_file, 'rel_c_path': rel_c_path, 'full_name': full_name, 'transpilation_result': result,
                'eo_file': os.path.abspath(eo_file), 'rel_eo_file': os.path.join(rel_c_path, f'{name}.eo'),
                'name': name, 'result_path': result_path, 'prepared_c_file': prepared_c_file}

    def prepare_c_file(self, path, file_name, c_file):
        with open(f'{c_file}', 'r', encoding='ISO-8859-1') as f:
            data = f.readlines()
        if self.need_to_prepare_c_code:
            prepare_c_code(data)
        result_path = os.path.join(path, self.result_dir_name)
        prepared_c_file = os.path.join(path, f'{file_name}-eo.c')
        if not os.path.exists(result_path):
            os.makedirs(result_path, exist_ok=True)
        with open(prepared_c_file, 'w') as f:
            f.writelines(data)
        return prepared_c_file, result_path

    def remove_unused_eo_files(self):
        transpiled_eo_names = set(map(lambda x: x['rel_eo_file'], self.transpilation_units))
        src_eo_names = tools.search_files_by_patterns(self.path_to_eo_src, ['*.eo'], recursive=True)
        src_eo_names = set(map(lambda x: x.replace(self.path_to_eo_src, ''), src_eo_names))
        for name in src_eo_names - transpiled_eo_names:
            os.remove(f'{self.path_to_eo_src}{name}')
        for unit in self.transpilation_units:
            unit['src_eo_path'] = os.path.join(self.path_to_eo_src, unit['rel_c_path'])
            unit['src_eo_file'] = os.path.join(unit['src_eo_path'], f'{unit["name"]}.eo')
        tools.remove_empty_dirs(self.path_to_eo_src)

    def create_plug_file(self, unit, message):
        plug = regex.sub('<file_name>', unit['full_name'], self.plug_code)
        plug = regex.sub('<exception_message>', message, plug)
        with open(unit['eo_file'], 'w') as f:
            f.write(plug)

    def group_transpilation_results(self):
        result = {tools.PASS: set([unit['name'] for unit in self.transpilation_units]), tools.NOTE: {},
                  tools.WARNING: {}, tools.ERROR: {}, tools.EXCEPTION: {}, tools.SKIP: {}}
        tools.pprint('\nGetting results\n', slowly=True, on_the_next_line=True)
        for unit in self.transpilation_units:
            exception_message = check_unit_exception(unit)
            if exception_message:
                if exception_message not in [tools.EXCEPTION]:
                    result[tools.EXCEPTION][exception_message] = {}
                if unit['name'] not in result[tools.EXCEPTION][exception_message]:
                    result[tools.EXCEPTION][exception_message][unit['name']] = set()

            for line in unit['transpilation_result'].stderr.split('\n'):
                line = line.lower()
                if any(warning in line for warning in self.ignored_transpilation_warnings):
                    continue

                for status in [tools.NOTE, tools.WARNING, tools.ERROR]:
                    if f'{status.lower()}:' in line:
                        place, _, message = line.partition(f'{status.lower()}:')
                        message = message.strip()
                        if message not in result[status]:
                            result[status][message] = {}
                        if unit['name'] not in result[status][message]:
                            result[status][message][unit['name']] = set()
                        if unit['name'] in place:
                            result[status][message][unit['name']].add(place.split(':', 1)[1][:-2])
        result[tools.PASS] -= set(file for value in result[tools.EXCEPTION].values() for file in value.keys())
        return result

    def move_transpiled_files(self):
        difference = []
        for unit in self.transpilation_units:
            shutil.copy(unit['eo_file'], os.path.join(unit['result_path'], f'{unit["name"]}.eo'))
            shutil.move(unit['prepared_c_file'], unit['result_path'])
            shutil.move(f'{unit["prepared_c_file"]}.i', unit['result_path'])
            if not tools.compare_files(unit['eo_file'], unit['src_eo_file']):
                if not os.path.exists(unit['src_eo_path']):
                    os.makedirs(unit['src_eo_path'], exist_ok=True)
                shutil.move(unit['eo_file'], unit['src_eo_file'])
                difference.append(unit['eo_file'])
            if os.path.isfile(unit['eo_file']):
                os.remove(unit['eo_file'])
        difference = list(filter(lambda x: x, difference))  # Filter None values
        if difference:
            tools.pprint(f'\nDetected changes in src files:')
            tools.pprint_only_file_names(difference)
            tools.pprint('Move these files to src dir\n')
        else:
            tools.pprint('\nNot found any changes in src files')

    def move_aliases(self):
        aliases = tools.search_files_by_patterns('.', ['*.alias'], print_files=True)
        if not os.path.exists(self.path_to_eo_external):
            os.makedirs(self.path_to_eo_external, exist_ok=True)
        tools.clear_dir_by_patterns(self.path_to_eo_external, ['*.eo'])
        for alias in aliases:
            file_name = tools.get_file_name(alias)
            destination_file = os.path.join(self.path_to_eo_external, file_name)
            shutil.move(alias, destination_file)

    def generate_run_sh(self, full_name):
        code = regex.sub(self.run_sh_replace, full_name, self.run_sh_code)
        with open(f'{self.path_to_eo_project}run.sh', 'w') as f:
            f.write(code)


def check_unit_exception(unit):
    if unit['transpilation_result'].returncode:
        return '\n'.join(unit['transpilation_result'].stderr.split('\n')[-3:-1])
    elif not os.path.isfile(unit['eo_file']):
        return 'exception: was generated empty EO file'
    elif os.stat(unit['eo_file']).st_size == 0:
        return 'exception: the EO file was not generated'
    return ''


def prepare_c_code(data):
    for i, line in enumerate(data):
        if ('#include' in line):
            data[i] = f'// {line}'


def create_parser():
    _parser = argparse.ArgumentParser(description='the script for translating C files to the EO files')

    _parser.add_argument('path_to_c_files', metavar='PATH',
                         help='the relative path from the scripts folder to the folder with c files')

    _parser.add_argument('-s', '--skips_file_name', metavar='FILE_NAME', default='',
                         help='the name of the file with a set of skips for transpile')

    _parser.add_argument('-n', '--not_prepare_c_code', action='store_const', const=True, default=False,
                         help='the script will not change the c code in the input files')
    return _parser


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    parser = create_parser()
    namespace = parser.parse_args()
    Transpiler(os.path.abspath(namespace.path_to_c_files), namespace.skips_file_name,
               not namespace.not_prepare_c_code).transpile()
