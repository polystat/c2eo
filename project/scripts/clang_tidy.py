#! /usr/bin/python3
# Check code with clang-tidy

import os
import sys
import subprocess
import time

# Our scripts
import settings
import tools


class ClangTidy(object):

    def __init__(self, path_to_code_files):
        if path_to_code_files is None:
            path_to_code_files = settings.get_setting('path_to_code_files')
        if os.path.isfile(path_to_code_files):
            path_to_code_files = os.path.dirname(path_to_code_files)
        self.path_to_code_files = path_to_code_files
        self.files_handled_count = 0
        self.files_count = 0
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.ignored_inspection_warnings = settings.get_setting('ignored_inspection_warnings')
        if not self.ignored_inspection_warnings:
            self.ignored_inspection_warnings = []
        self.clang_tidy_checks = ','.join(settings.get_setting('clang_tidy_checks'))
        self.results = []

    def inspect(self):
        tools.pprint('\nInspection start\n')
        self.generate_compile_commands()
        patterns = settings.get_setting('code_file_patterns')
        code_files = tools.search_files_by_patterns(self.path_to_code_files, patterns,
                                                    recursive=True, print_files=True)
        self.files_count = len(code_files)
        tools.pprint('\nInspect files:\n', slowly=True)
        tools.print_progress_bar(0, self.files_count)
        with tools.thread_pool() as threads:
            self.results = [result for result in threads.map(self.inspect_file, code_files)]
        data = self.group_transpilation_results()
        print_inspection_results(data)
        is_warnings = False
        for unit in self.results:
            if unit['inspection_result'].returncode != 0:
                tools.pprint_exception(unit['file'], unit['inspection_result'].stderr)
                is_warnings = True
        if len(data['warning']) > 0:
            is_warnings = True
        return is_warnings

    def generate_compile_commands(self):
        original_path = os.getcwd()
        os.chdir(self.path_to_c2eo_build)
        cmd = f'cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        os.chdir(original_path)
        if result.returncode != 0:
            tools.pprint_exception(cmd, result.stderr)
            exit('Failed during cmake execution')
        tools.pprint(result.stdout, slowly=True)

    def inspect_file(self, file):
        transpile_cmd = f'clang-tidy -p {self.path_to_c2eo_build} --checks=\'{self.clang_tidy_checks}\' {file}'
        result = subprocess.run(transpile_cmd, shell=True, capture_output=True, text=True)
        self.files_handled_count += 1
        tools.print_progress_bar(self.files_handled_count, self.files_count)
        return {'name': tools.get_file_name(file), 'file': os.path.basename(file), 'inspection_result': result}

    def group_transpilation_results(self):
        data = {'note': {}, 'warning': {}}
        for unit in self.results:
            result = unit['inspection_result']
            for line in result.stdout.split('\n'):
                if any(warning in line for warning in self.ignored_inspection_warnings):
                    continue

                for level in ['note', 'warning']:
                    if f'{level}:' in line:
                        place, _, message = line.partition(f'{level}:')
                        message = message.strip()
                        if message not in data[level]:
                            data[level][message] = set()
                        if unit['name'] in place:
                            data[level][message].add(place.split('/')[-1][:-2])
                        else:
                            data[level][message].add(unit['file'])
        return data


def print_inspection_results(data):
    print()
    tools.pprint()
    for level in ['note', 'warning']:
        for name, places in data[level].items():
            tools.pprint(name, slowly=True, status=level.upper())
            tools.pprint(f'{", ".join(sorted(places, key=str.casefold))}\n', slowly=True, status='')


if __name__ == '__main__':
    start_time = time.time()
    tools.move_to_script_dir(sys.argv[0])
    is_any_warnings = ClangTidy(tools.get_or_none(sys.argv, 1)).inspect()
    end_time = time.time()
    time_span = int(end_time - start_time)
    tools.pprint('Total time:  {:02}:{:02} min.'.format(time_span // 60, time_span % 60), slowly=True)
    tools.pprint(f'{"-" * 60}\n', slowly=True)
    if is_any_warnings:
        exit('Clang-tidy has several warnings')
