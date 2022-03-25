#! /usr/bin/python3
# Main script for transpilation run

import os
import sys
import shutil
import subprocess
import re as regex
from collections import Counter

# Our scripts
import tools
import settings
import clean_before_transpilation


def is_unique_files(c_files):
    names = list(map(lambda x: tools.get_file_name(x), c_files))
    is_unique = True
    counter = Counter(names)
    for key, value in counter.items():
        if value > 1:
            is_unique = False
            collisions = list(filter(lambda x: f'{key}.c' in x, c_files))
            collisions = list(map(lambda x: f'{x}\n', collisions))
            tools.print_exception_test('Detects follow files name collisions:', collisions)
    return is_unique


class Transpiler(object):

    def __init__(self, path_to_c_files, filters):

        if os.path.isfile(path_to_c_files):
            path_to_c_files = os.path.dirname(path_to_c_files)
        self.filters = filters
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_c2eo_transpiler = settings.get_setting('path_to_c2eo_transpiler')
        self.path_to_c_files = os.path.abspath(os.path.join(path_to_c_files, ''))
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_eo_src = os.path.abspath(settings.get_setting('path_to_eo_src'))
        self.plug_code = settings.get_meta_code('plug')
        self.plug_replace = settings.get_setting('plug_replace')
        self.result_dir_name = settings.get_setting('result_dir_name')
        self.run_sh_code = settings.get_meta_code('run.sh')
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.compilation_units = []
        self.replaced_path = f'{os.path.split(self.path_to_c_files[:-1])[0]}{os.sep}'

    def transpile(self):
        tools.pprint('\nTranspilation start\n')
        clean_before_transpilation.main(self.path_to_c_files)
        c_files = tools.search_files_by_pattern(self.path_to_c_files, '*.c',
                                                filters=self.filters, recursive=True, print_files=True)
        if not is_unique_files(c_files):
            return self.compilation_units
        original_path = os.getcwd()
        os.chdir(self.path_to_c2eo_build)
        self.compilation_units = [unit for unit in tools.thread_pool().map(self.start_transpilation, c_files)]
        self.remove_unused_eo_files()
        self.generate_plug_for_empty_eo_file()
        self.move_transpiled_files()
        if len(c_files) == 1:
            self.generate_run_sh(self.compilation_units[0])
        tools.pprint('Transpilation done\n')
        os.chdir(original_path)
        return self.compilation_units

    def start_transpilation(self, c_file):
        path, name, _ = tools.split_path(c_file, with_end_sep=True)
        rel_c_path = path.replace(self.replaced_path, '')
        full_name = f'{tools.make_name_from_path(rel_c_path)}.{name}'
        prepared_c_file, result_path = self.prepare_c_file(path, name, c_file)
        transpile_cmd = f'{self.path_to_c2eo_transpiler}c2eo {prepared_c_file} {full_name}.eo > /dev/null'
        eo_file = f'{name}.eo'
        subprocess.run(transpile_cmd, shell=True)
        os.rename(f'{full_name}.eo', eo_file)
        return {'c_file': c_file, 'rel_c_path': rel_c_path, 'full_name': full_name,
                'eo_file': os.path.abspath(eo_file), 'rel_eo_file': os.path.join(rel_c_path, eo_file),
                'name': name,  'result_path': result_path, 'prepared_c_file': prepared_c_file}

    def prepare_c_file(self, path, file_name, c_file):
        with open(f'{c_file}', 'r') as f:
            data = f.readlines()
        for i, line in enumerate(data):
            if '#' in line or 'printf' in line:
                data[i] = f'// {line}'
        result_path = os.path.join(path, self.result_dir_name)
        prepared_c_file = os.path.join(result_path, f'{file_name}-eo.c')
        if not os.path.exists(result_path):
            os.mkdir(result_path)
        with open(prepared_c_file, 'w') as f:
            f.writelines(data)
        return prepared_c_file, result_path

    def remove_unused_eo_files(self):
        eo_assembly_names = set(map(lambda x: x['rel_eo_file'], self.compilation_units))
        src_eo_names = tools.search_files_by_pattern(self.path_to_eo_src, '*.eo', recursive=True)
        src_eo_names = set(map(lambda x: x.replace(self.path_to_eo_src, ''), src_eo_names))
        for name in src_eo_names - eo_assembly_names:
            os.remove(f'{self.path_to_eo_src}{name}')
        for unit in self.compilation_units:
            unit['src_eo_path'] = os.path.join(self.path_to_eo_src, unit['rel_c_path'])
            unit['src_eo_file'] = os.path.join(unit['src_eo_path'], f'{unit["name"]}.eo')
        tools.remove_empty_dirs(self.path_to_eo_src)

    def generate_plug_for_empty_eo_file(self):
        empty_units = list(filter(lambda unit: os.stat(unit['eo_file']).st_size == 0, self.compilation_units))
        for empty_unit in empty_units:
            plug = regex.sub(self.plug_replace, empty_unit['full_name'], self.plug_code)
            with open(empty_unit['eo_file'], 'w') as f:
                f.write(plug)

    def move_transpiled_files(self):
        difference = []
        for unit in self.compilation_units:
            shutil.copy(unit['eo_file'], unit['result_path'])
            if not tools.compare_files(unit['eo_file'], unit['src_eo_file']):
                if not os.path.exists(unit['src_eo_path']):
                    os.makedirs(unit['src_eo_path'], exist_ok=True)
                shutil.move(unit['eo_file'], unit['src_eo_file'])
                difference.append(unit['eo_file'])
            if os.path.isfile(unit['eo_file']):
                os.remove(unit['eo_file'])
        difference = list(filter(lambda x: x, difference))  # Filter None values
        if difference:
            tools.pprint(f'\nDetect changes in src files:')
            tools.print_only_file_names(difference)
            tools.pprint('Move these files to src dir\n')
        else:
            tools.pprint('\nNot found any changes in src files')

    def generate_run_sh(self, full_name):
        code = regex.sub(self.run_sh_replace, full_name, self.run_sh_code)
        with open(f'{self.path_to_eo_project}run.sh', 'w') as f:
            f.write(code)


if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    tools.move_to_script_dir(sys.argv[0])
    Transpiler(path_to_files, None).transpile()
