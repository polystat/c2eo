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
from transpile_c2eo import Transpiler


class Tests(object):

    def __init__(self, path_to_tests=None, config=None):
        if path_to_tests is None:
            path_to_tests = settings.get_setting('path_to_tests')
        if config is None:
            config = settings.get_setting('config')
        self.filters = settings.get_config(config)
        self.path_to_tests = path_to_tests
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.run_sh_cmd = settings.get_meta_code('run.sh', read_as_lines=True)[2].rstrip()
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.transpilation_units = []
        self.test_handled_count = 0

    def test(self):
        update_eo_version.main()
        if not build_c2eo.main(self.path_to_c2eo_build):
            exit(-1)

        self.transpilation_units = Transpiler(self.path_to_tests, self.filters).transpile()
        if not self.transpilation_units:
            exit(-2)

        self.get_result_for_tests()
        with tools.thread_pool() as threads:
            results = threads.map(compare_test_results, self.transpilation_units)
        passed, errors, exceptions = group_comparison_results(results)
        print_tests_result(sorted(passed), sorted(errors), exceptions)

    def get_result_for_tests(self):
        tools.pprint('\nRunning C tests:\n', slowly=True)
        with tools.thread_pool() as threads:
            threads.map(self.get_result_for_c_file, self.transpilation_units)
        print()
        tools.pprint()

        if not EOBuilder().build():
            exit(-3)

        tools.pprint('\nRunning EO tests:\n', slowly=True)
        self.test_handled_count = 0
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        with tools.thread_pool() as threads:
            threads.map(self.get_result_for_eo_file, self.transpilation_units)
        os.chdir(original_path)
        print()
        tools.pprint()
        result = subprocess.run(f'killall java', shell=True, capture_output=True, text=True)
        if result.returncode == 0:
            tools.pprint('\nkillall java\n', slowly=True)
        else:
            tools.pprint(f'\n{result.stderr}', slowly=True)

    def get_result_for_c_file(self, unit):
        compiled_file = os.path.join(unit['result_path'], f'{unit["name"]}.out')
        unit['result_c_file'] = os.path.join(unit['result_path'], f'{unit["name"]}-c.txt')
        compile_cmd = f'clang {unit["c_file"]} -o {compiled_file} -Wno-everything > /dev/null 2>>{unit["result_c_file"]}'
        try:
            subprocess.run(compile_cmd, shell=True, check=True)
        except subprocess.CalledProcessError as exc:
            return exc
        else:
            subprocess.run(f'{compiled_file} >> {unit["result_c_file"]} 2>&1', shell=True)
        finally:
            self.test_handled_count += 1
            tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))

    def get_result_for_eo_file(self, unit):
        command = regex.sub(self.run_sh_replace, unit['full_name'], self.run_sh_cmd)
        unit['result_eo_file'] = os.path.join(unit['result_path'], f'{unit["name"]}-eo.txt')
        try:
            subprocess.run(f'{command} >> {unit["result_eo_file"]} 2>&1', shell=True, timeout=10)
        except subprocess.TimeoutExpired:
            with open(unit["result_eo_file"], 'w') as f:
                f.write('exception: execution time exceeded')
        finally:
            self.test_handled_count += 1
            tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))


def compare_test_results(unit):
    with open(unit['result_c_file'], 'r') as f:
        c_data = f.readlines()
    with open(unit['result_eo_file'], 'r') as f:
        eo_data = f.readlines()
    is_except, (is_equal, log_data) = compare_files(c_data, eo_data)
    with open(os.path.join(unit['result_path'], f'{unit["name"]}.log'), 'w') as f:
        f.writelines(log_data)
    return unit["name"], is_except, is_equal, log_data


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
        c_line = c_line.replace(',', '.')
        eo_line = eo_line.replace(',', '.')
        if is_both_float and math.isclose(float(c_line), float(eo_line), abs_tol=0.0001):
            log_data.append(ok_line)
        else:
            is_equal = False
            error_line = tools.colorize_text(f'\tLine {i}: {c_line} != {eo_line}', 'red') + '\n'
            log_data.append(error_line)
    return is_equal, log_data


def group_comparison_results(results):
    passed = []
    exceptions = {}
    errors = []
    tools.pprint('\nGetting results', slowly=True)
    for unit_name, is_except, is_equal, log_data in results:
        if is_except:
            log_data = ' '.join([x.replace('\n', '') for x in log_data])
            if log_data not in exceptions:
                exceptions[log_data] = []
            exceptions[log_data].append(unit_name)
        elif is_equal:
            passed.append(unit_name)
        else:
            errors.append((unit_name, log_data))
    return passed, errors, exceptions


def print_tests_result(passed, errors, exceptions):
    tools.pprint(f'\n{"-" * 60}', slowly=True)
    tools.pprint('TEST RESULTS', slowly=True)
    tools.pprint(f'{"-" * 60}', slowly=True)
    tools.pprint(', '.join(passed), slowly=True, status='PASS')
    for test_name, log_data in errors:
        print()
        tools.pprint_error(sorted(test_name), log_data, max_lines=30)
    for log_data, test_names in exceptions.items():
        print()
        tools.pprint_exception(', '.join(sorted(test_names)), log_data, max_lines=10)
    tools.pprint(f'\n{"-" * 60}', slowly=True)
    len_exceptions = sum(len(value) for value in exceptions.values())
    tests_count = len(passed) + len(errors) + len_exceptions
    tools.pprint(f'Tests run: {tests_count}, Passed: {len(passed)},'
                 f' Errors: {len(errors)}, Exceptions: {len_exceptions}', slowly=True)


if __name__ == '__main__':
    start_time = time.time()
    tools.move_to_script_dir(sys.argv[0])
    Tests(tools.get_or_none(sys.argv, 1), tools.get_or_none(sys.argv, 2)).test()
    end_time = time.time()
    time_span = int(end_time - start_time)
    tools.pprint('Total time:  {:02}:{:02} min.'.format(time_span // 60, time_span % 60), slowly=True)
    tools.pprint(f'{"-" * 60}\n', slowly=True)
