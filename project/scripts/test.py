#! /usr/bin/python3

import os
import sys
import re
import time

# Our scripts
import settings
import tools
from transpiler import Transpiler
import update_eo_version
import build_c2eo


def main(path_to_tests=''):
    if path_to_tests == '':
        path_to_tests = settings.get_setting('path_to_tests')
    path_to_src = settings.get_setting('path_to_eo_src')
    clear_dirs(path_to_tests, path_to_src)
    update_eo_version.main()
    build_c2eo.main(settings.get_setting('path_to_bin'))
    eo_c_files = Transpiler(path_to_tests).transpile()
    build_eo(settings.get_setting('path_to_eo_project'))


def build_eo(path_to_eo):
    os.chdir(path_to_eo)
    os.system('mvn clean compile')
    os.chdir('../project/scripts')


def prepare_tests(path_to_test_file):
    path, file = os.path.split(path_to_test_file)
    get_result_for_c_test(path, file)
    prepare_eo_file(path, file)


def prepare_eo_file(path, file):
    file_name = os.path.splitext(file)[0]
    eo_c_file = prepare_eo_c_file(path, file)
    os.system(f'./transpiler.py {eo_c_file}')


def get_result_for_c_test(path, file):
    file_name = os.path.splitext(file)[0]
    compiled_file = f'{path}{file_name}.out'
    command = f'clang {path_to_test_file} -o {output_file}'
    os.system(command)
    output_file = f'{path}c_result.txt'
    os.system(f'{output_file} > {output_file }')
    outName = ' -o ' + testedDir + 'a.out'


def get_result_for_eo_test(path, file):
    file_name = os.path.splitext(file)[0]
    command = 'java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.src.{file_name}_test'
    os.system(f'command > {path}eo_result.txt')


def clear_dirs(path_to_tests, path_to_result):
    for pattern in ['*.txt', '*-eo.c', '*.eo', '*.out', '*.log']:
        tools.clear_dir_by_pattern(path_to_tests, pattern)
    tools.clear_dir_by_pattern(path_to_result, '*.*')


def is_float(str_num):
    float_pattern = r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?'
    result = re.fullmatch(float_pattern, str_num)
    return result != None


def compare_test_results(path):
    path = os.path.split(path)[0]
    with open('{path}c_result.txt', 'r') as f:
        c_data = f.readlines()
    with open('{path}eo_result.txt', 'r') as f:
        eo_data = f.readlines()

    log_data = []
    for i, c_line, eo_line in enumerate(zip(c_data, eo_data)):
        if c_line == eo_line:
            continue
        
        if is_float(c_line[:-1]) and is_float(eo_line[:-1]) and abs(float(c_line) - float(eo_line)) < 0.0001:
             continue
        else:
            log_data.append('Error: {c_line} != {eo_line}')

    with open('{path}result.log', 'w') as f:
        f.write(log_data)


if __name__ == '__main__':
    start_time = time.monotonic()
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main(sys.argv[1]) if len(sys.argv) > 1 else main()
    end_time = time.monotonic()
    print(f'Elapsed testing time: {end_time - start_time}')
    print(f'Tests run: 0, Failures: 0, Errors: 0, Skipped: 0')
