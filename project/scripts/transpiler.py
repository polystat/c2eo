#! /usr/bin/python3
# Main script for transpilation run

import os
import sys
import shutil
from multiprocessing.dummy import Pool as ThreadPool

# Our scripts
import settings
import tools
import collect_transpiled_code


class Transpiler(object):

    def __init__(self, path_to_c_files):
        self.path_to_c_files = path_to_c_files
        self.assembly_path = settings.get_setting('path_to_assembly')
        self.result_path = settings.get_setting('path_to_eo_src')

    def transpile(self):
        tools.clear_dir_by_pattern(self.assembly_path, '*')
        tools.clear_dir_by_pattern(self.path_to_c_files, '*-eo.c', recursive=True)
        files = tools.search_files_by_pattern(self.path_to_c_files, '*.c', recursive=True, print_files=True)
        eo_c_files = ThreadPool(4).map(self.start_transpilation, files)
        self.start_collecting()
        print('Transpilation done\n')
        return eo_c_files

    def start_transpilation(self, path_to_c_file):
        print('\nTranspilation start\n')
        path_to_eo_c_file = self.prepare_eo_c_file(path_to_c_file)
        output_name = tools.get_file_name(path_to_eo_c_file).replace('-eo', '')
        os.system(f'../bin/c2eo {path_to_eo_c_file} {output_name} > /dev/null')
        return path_to_eo_c_file

    def prepare_eo_c_file(self, path_to_c_file):
        path, file = os.path.split(path_to_c_file)
        replaced_path = f'{os.path.split(self.path_to_c_files)[0]}{os.sep}'
        file_name = path.replace(replaced_path, '').replace(os.sep, '_')
        prepared_eo_c_file = f'{os.path.join(path, file_name)}-eo.c'
        with open(f'{path_to_c_file}', 'r') as f:
            data = f.readlines()
        for i, line in enumerate(data):
            if '#' in line or 'printf' in line:
                data[i] = f'// {line}'
        with open(prepared_eo_c_file, 'w') as f:
            f.writelines(data)
        return prepared_eo_c_file

    def start_collecting(self):
        tools.clear_dir_by_pattern(self.result_path, '*')
        collect_transpiled_code.main()
        print('Move global.eo to src dir')
        shutil.move(os.path.join(self.assembly_path, 'global.eo'), self.result_path)


if __name__ == '__main__':
    transpiler = Transpiler(os.path.abspath(sys.argv[1]))
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    transpiler.transpile()
