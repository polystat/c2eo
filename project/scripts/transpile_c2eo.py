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

    def __init__(self, path_to_c_files, filters, need_to_prepare_c_code=True):
        if os.path.isfile(path_to_c_files):
            path_to_c_files = os.path.dirname(path_to_c_files)
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.filters = filters
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_c2eo_transpiler = settings.get_setting('path_to_c2eo_transpiler')
        self.path_to_c_files = os.path.join(os.path.abspath(path_to_c_files), '')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_eo_src = os.path.join(os.path.abspath(settings.get_setting('path_to_eo_src')), '')
        self.path_to_eo_external = os.path.join(os.path.abspath(settings.get_setting('path_to_eo_external')), '')
        self.plug_code = settings.get_meta_code('plug')
        self.plug_replace = settings.get_setting('plug_replace')
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
        tools.pprint('\nTranspilation start\n')
        clean_before_transpilation.main(self.path_to_c_files)
        c_files = tools.search_files_by_patterns(self.path_to_c_files, ['*.c'],
                                                 filters=self.filters, recursive=True, print_files=True)
        self.files_count = len(c_files)
        original_path = os.getcwd()
        os.chdir(self.path_to_c2eo_build)
        tools.pprint('\nTranspile files:\n', slowly=True)
        with tools.thread_pool() as threads:
            self.transpilation_units = [unit for unit in threads.map(self.start_transpilation, c_files)]
        print_transpilation_results(self.group_transpilation_results())
        self.check_c2eo_fails()
        self.remove_unused_eo_files()
        self.generate_plug_for_empty_eo_file()
        self.move_transpiled_files()
        self.move_aliases()
        if len(c_files) == 1:
            self.generate_run_sh(self.transpilation_units[0]['full_name'])
        tools.pprint('Transpilation done\n')
        os.chdir(original_path)
        return self.transpilation_units

    def start_transpilation(self, c_file):
        path, name, _ = tools.split_path(c_file, with_end_sep=True)
        rel_c_path = path.replace(self.replaced_path, "")
        full_name = f'{tools.make_name_from_path(rel_c_path)}.{name}'
        prepared_c_file, result_path = self.prepare_c_file(path, name, c_file)
        transpile_cmd = f'{self.path_to_c2eo_transpiler}c2eo {prepared_c_file} {full_name}.eo'
        eo_file = f'{full_name}.eo'
        result = subprocess.run(transpile_cmd, shell=True, capture_output=True, text=True)
        self.files_handled_count += 1
        tools.print_progress_bar(self.files_handled_count, self.files_count)
        return {'c_file': c_file, 'rel_c_path': rel_c_path, 'full_name': full_name, 'transpilation_result': result,
                'eo_file': os.path.abspath(eo_file), 'rel_eo_file': os.path.join(rel_c_path, f'{name}.eo'),
                'name': name,  'result_path': result_path, 'prepared_c_file': prepared_c_file}

    def prepare_c_file(self, path, file_name, c_file):
        with open(f'{c_file}', 'r', encoding='ISO-8859-1') as f:
            data = f.readlines()
        if self.need_to_prepare_c_code:
            prepare_c_code(data)
        result_path = os.path.join(path, self.result_dir_name)
        prepared_c_file = os.path.join(result_path, f'{file_name}-eo.c')
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

    def generate_plug_for_empty_eo_file(self):
        empty_units = list(filter(lambda x: os.stat(x['eo_file']).st_size == 0, self.transpilation_units))
        for empty_unit in empty_units:
            plug = regex.sub(self.plug_replace, empty_unit['full_name'], self.plug_code)
            with open(empty_unit['eo_file'], 'w') as f:
                f.write(plug)

    def group_transpilation_results(self):
        data = {'note': {}, 'warning': {}, 'error': {}, 'exception': {}}
        for unit in self.transpilation_units:
            result = unit['transpilation_result']
            for line in result.stderr.split('\n'):
                if any(warning in line for warning in self.ignored_transpilation_warnings):
                    continue

                for level in ['note', 'warning', 'error', 'exception']:
                    if f'{level}:' in line:
                        place, _, message = line.partition(f'{level}:')
                        message = message.strip()
                        if message not in data[level]:
                            data[level][message] = set()
                        if unit['name'] in place:
                            data[level][message].add(place.split('/')[-1][:-2])
                        else:
                            data[level][message].add(f'{unit["name"]}-eo.c')
        return data

    def check_c2eo_fails(self):
        is_transpilation_failed = False
        for unit in self.transpilation_units:
            result = unit['transpilation_result']
            if result.returncode:
                exception_message = '\n'.join(result.stderr.split('\n')[-3:-1])
                tools.pprint_exception(f'c2eo {unit["name"]}', exception_message)
                is_transpilation_failed = True
                print()
        if is_transpilation_failed:
            exit('c2eo failed on some c files')

    def move_transpiled_files(self):
        difference = []
        for unit in self.transpilation_units:
            shutil.copy(unit['eo_file'], os.path.join(unit['result_path'], f'{unit["name"]}.eo'))
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
            tools.print_only_file_names(difference)
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


def prepare_c_code(data):
    for i, line in enumerate(data):
        if ('#include' in line) or ('printf' in line):
            new_line = line.lstrip()
            whitespace_count = len(line) - len(new_line)
            indent = ' ' * whitespace_count
            data[i] = f'{indent}// {new_line}'
            if new_line.startswith('printf'):
                argument = line.rsplit(',', maxsplit=1)[1].replace(');', '').replace(',', '').strip()
                data[i] += f'{indent}{argument};\n'


def print_transpilation_results(data):
    print()
    tools.pprint()
    for level in ['note', 'warning', 'error', 'exception']:
        for name, places in data[level].items():
            tools.pprint(name, slowly=True, status=level.upper())
            tools.pprint(f'{", ".join(sorted(places, key=str.casefold))}\n', slowly=True, status='')


if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    tools.move_to_script_dir(sys.argv[0])
    prepare_code = tools.get_or_none(sys.argv, 2)
    Transpiler(path_to_files, None, prepare_code != 'f').transpile()
