#! /usr/bin/python3

import os
import sys
import re
import time
import math
import subprocess
from multiprocessing.dummy import Pool as ThreadPool

# Our scripts
import settings
import tools
from transpiler import Transpiler
import update_eo_version
import build_c2eo
import build_eo
import clean_before_transpilation


class Tests(object):

    def __init__(self, path_to_tests=None, config=None):
        if path_to_tests is None:
            path_to_tests = settings.get_setting('path_to_tests')
        if config is None:
            config = settings.get_setting('config')
        self.filters = settings.get_config(config)
        self.path_to_tests = path_to_tests
        self.path_to_c2eo = settings.get_setting('path_to_c2eo')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')

    def test(self):
        clean_before_transpilation.main(self.path_to_tests, self.path_to_eo_src)
        update_eo_version.main()
        build_c2eo.main(self.path_to_c2eo)
        c_files, eo_c_files = Transpiler(self.path_to_tests, self.filters).transpile()
        self.get_result_for_tests(c_files, eo_c_files)
        results = ThreadPool(4).map(compare_test_results, eo_c_files)
        passed, errors, exceptions = group_comparison_results(results)
        print_tests_result(sorted(passed), sorted(errors), sorted(exceptions))

    def get_result_for_tests(self, c_files, eo_c_files):
        ThreadPool(4).map(get_result_for_c_test, c_files)
        build_eo.main(self.path_to_eo_project)
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        ThreadPool(4).map(get_result_for_eo_test, eo_c_files)
        os.chdir(original_path)


def get_result_for_c_test(path_to_test):
    path, file_name, _ = tools.split_path(path_to_test, with_end_sep=True)
    compiled_file = f'{path}{file_name}.out'
    result_file = f'{path}c_result.txt'
    compile_cmd = f'clang {path_to_test} -o {compiled_file} -Wno-everything > /dev/null 2>>{result_file}'
    try:
        subprocess.run(compile_cmd, shell=True, check=True)
    except subprocess.CalledProcessError as exc:
        return exc
    else:
        subprocess.run(f'{compiled_file} >> {result_file} 2>&1', shell=True)


def get_result_for_eo_test(eo_c_file):
    path, file_name, _ = tools.split_path(eo_c_file, with_end_sep=True)
    file_name = file_name.replace('-eo', '')
    command = f'java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.src.{file_name}'
    result_file = f'{path}eo_result.txt'
    subprocess.run(f'{command} >> {result_file} 2>&1', shell=True)


def compare_test_results(path_to_eo_c_file):
    path, file_name, _ = tools.split_path(path_to_eo_c_file, with_end_sep=True)
    with open(f'{path}c_result.txt', 'r') as f:
        c_data = f.readlines()
    with open(f'{path}eo_result.txt', 'r') as f:
        eo_data = f.readlines()
    file_name = file_name.replace('-eo', '')
    is_except, (is_equal, log_data) = compare_files(c_data, eo_data)
    with open(f'{path}result.log', 'w') as f:
        f.writelines(log_data)
    return file_name, is_except, is_equal, log_data


def compare_files(c_data, eo_data):
    if is_exception(c_data):
        return True, (False, c_data)

    if is_exception(eo_data):
        return True, (False, eo_data)

    if len(c_data) != len(eo_data):
        log_data = ['Results have different length!\n', '\nC result:\n']
        log_data.extend(c_data)
        log_data.append('\nEO result:\n')
        log_data.extend(eo_data)
        return False, (False, log_data)

    return False, compare_lines(c_data, eo_data)


def is_exception(lines):
    return len(lines) > 0 and ('exception' in lines[0].lower() or 'error' in lines[0].lower())


def compare_lines(c_data, eo_data):
    is_equal = True
    log_data = []
    for i, (c_line, eo_line) in enumerate(zip(c_data, eo_data)):
        ok_line = tools.colorize_text(f'\tLine {i}: {c_line[:-1]} == {eo_line[:-1]}', 'green')
        if c_line == eo_line:
            log_data.append(ok_line)
            continue

        is_both_float = is_float(c_line[:-1]) and is_float(eo_line[:-1])
        if is_both_float and math.isclose(float(c_line), float(eo_line), abs_tol=0.0001):
            log_data.append(ok_line)
        else:
            is_equal = False
            error_line = tools.colorize_text(f'\tLine {i}: {c_line[:-1]} != {eo_line}', 'red')
            log_data.append(error_line)
    return is_equal, log_data


def is_float(str_num):
    float_pattern = r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?'
    result = re.fullmatch(float_pattern, str_num)
    return result is not None


def group_comparison_results(results):
    passed = []
    exceptions = []
    errors = []

    for test_name, is_except, is_equal, log_data in results:
        if is_except:
            exceptions.append((test_name, log_data))
        elif is_equal:
            passed.append((test_name, log_data))
        else:
            errors.append((test_name, log_data))
    return passed, errors, exceptions


def print_tests_result(passed, errors, exceptions):
    info = tools.colorize_text('INFO', 'blue')
    print_slowly(f'\n[{info}] {"-" * 60}')
    print_slowly(f'[{info}]  TEST RESULTS')
    print_slowly(f'[{info}] {"-" * 60}')
    print()
    for test_name, _ in passed:
        print_passed_test(test_name)

    for test_name, log_data in errors:
        print_error_test(test_name, log_data)

    for test_name, log_data in exceptions:
        print_exception_test(test_name, log_data)

    print_slowly(f'\n[{info}] {"-" * 60}')
    tests_count = len(passed) + len(errors) + len(exceptions)
    print_slowly(f'[{info}]  Tests run: {tests_count}, '
                 f'Passed: {len(passed)}, Errors: {len(errors)}, Exceptions: {len(exceptions)}')


def print_slowly(*lines):
    for line in lines:
        if len(lines) > 1:
            print(line.rstrip('\n'))
        else:
            print(line)
        time.sleep(0.05)


def print_passed_test(test_name, ):
    ok = tools.colorize_text('OK', 'green')
    print_slowly(f'[{ok}] {test_name}')


def print_error_test(test_name, log_data):
    error = tools.colorize_text('ERROR', 'red')
    print_slowly(f'\n[{error}] {test_name}')
    if len(log_data) > 30:
        log_data = log_data[:30]
        indent = '  ' * (len(log_data[-1]) - len(log_data[-1].lstrip()))
        log_data.append(f'{indent}...')
    print_slowly(*log_data)


def print_exception_test(test_name, log_data):
    exception = tools.colorize_text('EXCEPTION', 'red')
    print_slowly(f'\n[{exception}] {test_name}:')
    if len(log_data) > 10:
        log_data = log_data[:10]
        indent = '  ' * (len(log_data[-1]) - len(log_data[-1].lstrip()))
        log_data.append(f'{indent}...')
    print_slowly(*log_data)


if __name__ == '__main__':
    start_time = time.time()
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        Tests().test()
    elif len(sys.argv) == 2:
        Tests(sys.argv[1]).test()
    else:
        Tests(sys.argv[1], sys.argv[2]).test()
    end_time = time.time()
    time_span = int(end_time - start_time)
    inf = tools.colorize_text('INFO', 'blue')
    print('[{}]  Total time:  {:02}:{:02} min.'.format(inf, time_span // 60, time_span % 60))
    print(f'[{inf}] {"-" * 60}')
    print()
