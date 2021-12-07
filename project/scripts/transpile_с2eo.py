#! /usr/bin/python3
# Main script for transpilation run

import os
import sys
import shutil
import subprocess
import re as regex

# Our scripts
import tools
import settings
import collect_transpiled_code


class Transpiler(object):

    def __init__(self, path_to_c_files, filters):
        self.filters = filters
        self.path_to_c_files = os.path.join(path_to_c_files, '')
        self.path_to_assembly = settings.get_setting('path_to_assembly')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_c2eo = settings.get_setting('path_to_c2eo')

    def transpile(self):
        print('\nTranspilation start\n')
        tools.clear_dir_by_pattern(self.path_to_assembly, '*')
        tools.clear_dir_by_pattern(self.path_to_c_files, '*-eo.c', recursive=True)
        c_files = tools.search_files_by_pattern(self.path_to_c_files, '*.c', filters=self.filters,
                                                recursive=True, print_files=True)
        eo_names = tools.thread_pool().map(self.start_transpilation, c_files)
        self.start_collecting()
        if len(c_files) == 1:
            self.generate_run_sh(c_files[0])
        print('Transpilation done\n')
        return c_files, eo_names

    def start_transpilation(self, path_to_c_file):
        path_to_eo_c_file = self.prepare_eo_c_file(path_to_c_file)
        output_name = tools.get_file_name(path_to_eo_c_file).replace('-eo', '')
        transpile_cmd = f'{self.path_to_c2eo}c2eo {path_to_eo_c_file} {output_name} > /dev/null'
        subprocess.run(transpile_cmd, shell=True)
        path = os.path.dirname(path_to_c_file)
        shutil.copy(f'{self.path_to_assembly}{output_name}.glob', path)
        stat_file = f'{self.path_to_assembly}{output_name}.stat'
        if os.path.exists(stat_file):
            shutil.copy(stat_file, path)
        return os.path.abspath(path_to_eo_c_file)

    def prepare_eo_c_file(self, path_to_c_file):
        path, file_name, _ = tools.split_path(path_to_c_file, with_end_sep=True)
        replaced_path = f'{os.path.split(self.path_to_c_files[:-1])[0]}{os.sep}'
        output_name = tools.make_name_from_path(path.replace(replaced_path, ''))
        prepared_eo_c_file = f'{path}{output_name}-eo.c'
        with open(f'{path_to_c_file}', 'r') as f:
            data = f.readlines()
        for i, line in enumerate(data):
            if '#' in line or 'printf' in line:
                data[i] = f'// {line}'
        with open(prepared_eo_c_file, 'w') as f:
            f.writelines(data)
        return prepared_eo_c_file

    def start_collecting(self):
        tools.clear_dir_by_pattern(self.path_to_eo_src, '*')
        collect_transpiled_code.main()
        print('Move global.eo to src dir')
        shutil.move(f'{self.path_to_assembly}global.eo', self.path_to_eo_src)

    def generate_run_sh(self, c_file):
        file_name = tools.get_file_name(c_file)
        code = settings.get_meta_code('run.sh')
        code = regex.sub('<object_name>', file_name, code)
        with open(f'{self.path_to_eo_project}run.sh', 'w') as f:
            f.write(code)


if __name__ == '__main__':
    transpiler = Transpiler(os.path.abspath(sys.argv[1]), [])
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    transpiler.transpile()
