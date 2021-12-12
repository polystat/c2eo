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
import clean_before_transpilation


class Transpiler(object):

    def __init__(self, path_to_c_files, filters):
        self.filters = filters
        self.path_to_c_files = os.path.join(path_to_c_files, '')
        self.path_to_assembly = settings.get_setting('path_to_assembly')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_c2eo = settings.get_setting('path_to_c2eo')

    def transpile(self):
        tools.pprint('\nTranspilation start\n')
        clean_before_transpilation.main(self.path_to_c_files)
        c_files = tools.search_files_by_pattern(self.path_to_c_files, '*.c', filters=self.filters,
                                                recursive=True, print_files=True)
        eo_c_files = tools.thread_pool().map(self.start_transpilation, c_files)
        self.start_collecting(eo_c_files)
        if len(c_files) == 1:
            self.generate_run_sh(eo_c_files[0])
        tools.pprint('Transpilation done\n')
        return c_files, eo_c_files

    def start_transpilation(self, path_to_c_file):
        eo_c_file = self.prepare_eo_c_file(path_to_c_file)
        output_name = tools.get_file_name(eo_c_file).replace('-eo', '')
        transpile_cmd = f'{self.path_to_c2eo}c2eo {eo_c_file} {output_name} > /dev/null'
        subprocess.run(transpile_cmd, shell=True)
        return os.path.abspath(eo_c_file)

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

    def start_collecting(self, eo_c_files):
        for file in eo_c_files:
            name = tools.get_file_name(file).replace('-eo', '')
            collect_transpiled_code.main(name)
        self.remove_unused_eo_files()
        difference = tools.thread_pool().map(self.move_transpilation_files, eo_c_files)
        difference = filter(lambda x: x, difference)  # Filter None values
        if difference:
            tools.pprint(f'\nDetect changes in src files:')
            tools.print_only_file_names(difference)
            tools.pprint('Move these files to src dir\n')
        else:
            tools.pprint('\nNot found any changes src in files')

    def remove_unused_eo_files(self):
        eo_assembly_names = tools.search_files_by_pattern(self.path_to_assembly, '*.eo')
        eo_assembly_names = set(map(tools.get_file_name, eo_assembly_names))
        src_eo_names = tools.search_files_by_pattern(self.path_to_eo_src, '*.eo')
        src_eo_names = set(map(tools.get_file_name, src_eo_names))
        for name in src_eo_names - eo_assembly_names:
            os.remove(f'{self.path_to_eo_src}{name}.eo')

    def move_transpilation_files(self, eo_c_file):
        name = tools.get_file_name(eo_c_file).replace('-eo', '')
        assembly_file = f'{self.path_to_assembly}{name}.eo'
        eo_c_dir = os.path.dirname(eo_c_file)
        for extension in ['.glob', '.stat']:
            file = assembly_file.replace('.eo', extension)
            if os.path.isfile(file):
                shutil.move(file, eo_c_dir)
        shutil.copy(assembly_file, eo_c_dir)
        src_file = f'{self.path_to_eo_src}{name}.eo'
        if not tools.compare_files(assembly_file, src_file):
            shutil.move(assembly_file, src_file)
            return assembly_file
        if os.path.isfile(assembly_file):
            os.remove(assembly_file)

    def generate_run_sh(self, eo_c_file):
        file_name = tools.get_file_name(eo_c_file).replace('-eo', '')
        code = settings.get_meta_code('run.sh')
        code = regex.sub('<object_name>', file_name, code)
        with open(f'{self.path_to_eo_project}run.sh', 'w') as f:
            f.write(code)


if __name__ == '__main__':
    transpiler = Transpiler(os.path.abspath(sys.argv[1]), [])
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    transpiler.transpile()
