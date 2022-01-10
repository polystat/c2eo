#! /usr/bin/python3

import os
import sys
import time
import math
import subprocess
import re as regex

# Our scripts
import tools
import settings
import build_c2eo
import update_eo_version
from build_eo import EOBuilder
from transpile_с2eo import Transpiler


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
        self.run_sh_cmd = settings.get_meta_code('run.sh', read_as_lines=True)[2].rstrip()

    def test(self):
        update_eo_version.main()
        build_c2eo.main(self.path_to_c2eo)
        c_files, eo_c_files = Transpiler(self.path_to_tests, self.filters).transpile()
        self.get_result_for_tests(c_files, eo_c_files)
        results = tools.thread_pool().map(compare_test_results, eo_c_files)
        passed, errors, exceptions = group_comparison_results(results)
        print_tests_result(sorted(passed), sorted(errors), sorted(exceptions))

    def get_result_for_tests(self, c_files, eo_c_files):
        tools.thread_pool().map(get_result_for_c_test, c_files)
        if EOBuilder().build():
            tools.pprint('\nRunning tests')
            original_path = os.getcwd()
            os.chdir(self.path_to_eo_project)
            tools.thread_pool().map(self.get_result_for_eo_test, eo_c_files)
            os.chdir(original_path)
        else:
            exit(-1)

    def get_result_for_eo_test(self, eo_c_file):
        path, file_name, _ = tools.split_path(eo_c_file, with_end_sep=True)
        file_name = file_name.replace('-eo', '')
        command = regex.sub('<object_name>', file_name, self.run_sh_cmd)
        result_file = f'{path}eo_result.txt'
        subprocess.run(f'{command} >> {result_file} 2>&1', shell=True)


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
        c_line = c_line.rstrip()
        eo_line = eo_line.rstrip()
        ok_line = tools.colorize_text(f'\tLine {i}: {c_line} == {eo_line}', 'green') + '\n'
        if c_line == eo_line:
            log_data.append(ok_line)
            continue

        is_both_float = tools.is_float(c_line) and tools.is_float(eo_line)
        if is_both_float and math.isclose(float(c_line), float(eo_line), abs_tol=0.0001):
            log_data.append(ok_line)
        else:
            is_equal = False
            error_line = tools.colorize_text(f'\tLine {i}: {c_line} != {eo_line}', 'red') + '\n'
            log_data.append(error_line)
    return is_equal, log_data


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
    tools.pprint(f'\n{"-" * 60}', slowly=True)
    tools.pprint('TEST RESULTS', slowly=True)
    tools.pprint(f'{"-" * 60}', slowly=True)
    for test_name, _ in passed:
        tools.pprint(test_name, slowly=True, status='PASS')
    for test_name, log_data in errors:
        print_error_test(test_name, log_data)
    for test_name, log_data in exceptions:
        print_exception_test(test_name, log_data)
    tools.pprint(f'\n{"-" * 60}', slowly=True)
    tests_count = len(passed) + len(errors) + len(exceptions)
    tools.pprint(f'Tests run: {tests_count}, Passed: {len(passed)},'
                 f' Errors: {len(errors)}, Exceptions: {len(exceptions)}', slowly=True)


def print_error_test(test_name, log_data):
    tools.pprint(test_name, slowly=True, status='ERROR')
    print_truncated_data(log_data, 30)


def print_exception_test(test_name, log_data):
    tools.pprint(test_name, slowly=True, status='EXCEPTION')
    print_truncated_data(log_data, 10)


def print_truncated_data(data, max_lines):
    lines_count = min(max_lines, len(data))
    log_data = ''.join(data[:lines_count])
    tools.pprint(log_data, slowly=True, status=None)


if __name__ == '__main__':
    start_time = time.time()
    tools.move_to_script_dir(sys.argv[0])
    Tests(tools.get_or_none(sys.argv, 1), tools.get_or_none(sys.argv, 2)).test()
    end_time = time.time()
    time_span = int(end_time - start_time)
    tools.pprint('Total time:  {:02}:{:02} min.'.format(time_span // 60, time_span % 60), slowly=True)
    tools.pprint(f'{"-" * 60}\n', slowly=True)
