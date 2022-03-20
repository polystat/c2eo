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
import clean_before_transpilation


class Transpiler(object):

    def __init__(self, path_to_c_files, filters):
        self.filters = filters
        if os.path.isfile(path_to_c_files):
            path_to_c_files = os.path.dirname(path_to_c_files)
        self.path_to_c_files = os.path.join(path_to_c_files, '')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_c2eo_transpiler = settings.get_setting('path_to_c2eo_transpiler')
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.run_sh_code = settings.get_meta_code('run.sh')
        self.plug_code = settings.get_meta_code('plug')

    def transpile(self):
        tools.pprint('\nTranspilation start\n')
        clean_before_transpilation.main(self.path_to_c_files)
        c_files = tools.search_files_by_pattern(self.path_to_c_files, '*.c', filters=self.filters,
                                                recursive=True, print_files=True)
        original_path = os.getcwd()
        os.chdir(self.path_to_c2eo_build)
        eo_c_files = tools.thread_pool().map(self.start_transpilation, c_files)
        self.start_collecting(eo_c_files)
        if len(c_files) == 1:
            self.generate_run_sh(eo_c_files[0])
        tools.pprint('Transpilation done\n')
        os.chdir(original_path)
        return c_files, eo_c_files

    def start_transpilation(self, path_to_c_file):
        eo_c_file = self.prepare_eo_c_file(path_to_c_file)
        output_name = tools.get_file_name(eo_c_file).replace('-eo', '')
        transpile_cmd = f'{self.path_to_c2eo_transpiler}c2eo {eo_c_file} {output_name}.eo > /dev/null'
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
        eo_assembly_files = tools.search_files_by_pattern('.', '*.eo')
        self.remove_unused_eo_files(eo_assembly_files)
        empty_files = list(filter(lambda file: os.stat(file).st_size == 0, eo_assembly_files))
        tools.thread_pool().map(self.generate_plug_for_empty_eo_file, empty_files)
        difference = tools.thread_pool().map(self.move_transpilation_files, eo_c_files)
        difference = list(filter(lambda x: x, difference))  # Filter None values
        if difference:
            tools.pprint(f'\nDetect changes in src files:')
            tools.print_only_file_names(difference)
            tools.pprint('Move these files to src dir\n')
        else:
            tools.pprint('\nNot found any changes in src files')

    def generate_plug_for_empty_eo_file(self, file):
        file_name = tools.get_file_name(file)
        plug = regex.sub('<file_name>', file_name, self.plug_code)
        with open(file, 'w') as f:
            f.write(plug)

    def remove_unused_eo_files(self, eo_assembly_files):
        eo_assembly_names = set(map(tools.get_file_name, eo_assembly_files))
        src_eo_names = tools.search_files_by_pattern(self.path_to_eo_src, '*.eo')
        src_eo_names = set(map(tools.get_file_name, src_eo_names))
        for name in src_eo_names - eo_assembly_names:
            os.remove(f'{self.path_to_eo_src}{name}.eo')

    def move_transpilation_files(self, eo_c_file):
        name = tools.get_file_name(eo_c_file).replace('-eo', '')
        transpilation_file = f'{name}.eo'
        eo_c_dir = os.path.dirname(eo_c_file)
        for extension in ['.glob', '.stat']:
            file = transpilation_file.replace('.eo', extension)
            if os.path.isfile(file):
                shutil.move(file, eo_c_dir)
        shutil.copy(transpilation_file, eo_c_dir)
        src_file = f'{self.path_to_eo_src}{name}.eo'
        if not tools.compare_files(transpilation_file, src_file):
            shutil.move(transpilation_file, src_file)
            return transpilation_file
        if os.path.isfile(transpilation_file):
            os.remove(transpilation_file)

    def generate_run_sh(self, eo_c_file):
        file_name = tools.get_file_name(eo_c_file).replace('-eo', '')
        code = regex.sub('<object_name>', file_name, self.run_sh_code)
        with open(f'{self.path_to_eo_project}run.sh', 'w') as f:
            f.write(code)


if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    tools.move_to_script_dir(sys.argv[0])
    Transpiler(path_to_files, None).transpile()
