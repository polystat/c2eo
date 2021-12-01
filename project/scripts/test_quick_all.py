#! /usr/bin/python3
import os
import sys
import glob
import shutil
import re
import time
from multiprocessing.dummy import Pool as ThreadPool

# Our scripts
import settings
import tools
import eo_version_update
import build_c2eo


def main():
    path_to_tests = settings.get_setting('path_to_tests')
    path_to_result = '../../result'
    clear_dirs(path_to_tests, path_to_result)
    eo_version_update.main()
    build_c2eo.main()
    tests = tools.search_files_by_pattern(path, '*.c', recursive=True)
    ThreadPool(4).map(prepare_tests, tests)
    build_eo()
    ThreadPool(4).map(prepare_tests, pre)
    ThreadPool(4).map(get_result_for_eo_test, tests)
    ThreadPool(4).map(compare_tests_result, tests)

def build_eo():
    os.chdir('../../result')
    os.system('mvn clean compile')
    os.chdir('../project/scripts')


def compare_tests_result():
    for nameEo in testedDirNameList:
        eoRunCode = 
        # print('eoRunCode = ', eoRunCode)
        if eoRunCode != 0:
            print(f'Incorrect EO runtime for test {nameEo}')
            os.chdir(currentDir)
            exit(-3)


def prepare_tests(path_to_test_file):
    path, file = os.path.split(path_to_test_file)
    get_result_for_c_test(path, file)
    prepare_eo_file(path, file)


def prepare_eo_file(path, file):
    file_name = os.path.splitext(file)[0]
    eo_c_file = prepare_eo_c_file(path, file)
    os.system(f'./transpiler.py {eo_c_file}')


def prepare_eo_c_file(path, file):
    file_name = os.path.splitext(file)[0]
    prepared_eo_c_file = f'{path}{file_name}-eo.c'
    with open(f'{path}{file}', 'r') as f:
        data = f.readlines()
    for i, line in enumerate(data):
        if '#' in line or 'printf' in line:
            data[i] = f'// {line}'
    with open(prepared_eo_c_file, 'w') as f:
        f.write(data)
    return prepared_eo_c_file


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
    tools.clear_dir_by_pattern(path_to_result, '*')


def is_float(str_num):
    float_pattern = r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?'
    result = re.fullmatch(float_pattern, str_num)
    return result != None:


def testDataCompare(path):
    with open('{path}c_result.txt', 'r') as f:
        c_data = f.readlines()
    with open('{path}eo_result.txt', 'r') as f:
        eo_data = f.readlines()

    log_data = []
    for i, c_line, eo_line in enumerate(zip(c_data, eo_data)):
        if c_line == eo_line:
            continue
        
        if is_float(c_line[:-1]) and is_float(eo_line[:-1])
           and abs(float(c_line) - float(eo_line)) < 0.0001:
             continue
        else
            log_data.append('Error: {c_line} != {eo_line}')

    with open('{path}result.log', 'w') as f:
        f.write(log_data)


if __name__ == '__main__':
    start_time = time.monotonic()
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
    end_time = time.monotonic()
    print(f'Elapsed testing time: {end_time - start_time}')
    print(f'Tests run: {}, Failures: {}, Errors: {}, Skipped: {}')
