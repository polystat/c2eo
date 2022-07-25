#! /usr/bin/python3

"""
The MIT License (MIT)

Copyright (c) 2021-2022 c2eo team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
import sys
import time
import math
import subprocess
import re as regex

# Our scripts
import tools
import settings
from build_eo import EOBuilder
from transpile_c2eo import Transpiler


class Tests(object):

    def __init__(self, path_to_tests=None, skips_file_name=None):
        if path_to_tests is None:
            path_to_tests = settings.get_setting('path_to_tests')
        if skips_file_name is None:
            skips_file_name = settings.get_setting('skips_for_test')
        self.skips = settings.get_skips(skips_file_name)
        self.path_to_tests = path_to_tests
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.run_sh_cmd = settings.get_meta_code('run.sh', read_as_lines=True)[2].rstrip()
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.transpilation_units = []
        self.test_handled_count = 0

    def test(self):
        start_time = time.time()
        self.transpilation_units = Transpiler(self.path_to_tests).transpile()
        if self.transpilation_units:
            self.get_result_for_tests()
            with tools.thread_pool() as threads:
                results = threads.map(self.compare_test_results, self.transpilation_units)
            result = group_comparison_results(results)
            _fails_count = len(result[tools.ERROR]) + sum(map(len, result[tools.EXCEPTION].values()))
            tools.pprint_result('TEST', len(self.transpilation_units), int(time.time() - start_time), result,
                                _fails_count)
            return _fails_count

    def get_result_for_tests(self):
        tools.pprint('\nRunning C tests:\n', slowly=True)
        with tools.thread_pool() as threads:
            threads.map(self.get_result_for_c_file, self.transpilation_units)
        tools.pprint(on_the_next_line=True)
        EOBuilder().build()
        tools.pprint('\nRunning EO tests:\n', slowly=True)
        self.test_handled_count = 0
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        with tools.thread_pool() as threads:
            threads.map(self.get_result_for_eo_file, self.transpilation_units)
        os.chdir(original_path)
        tools.pprint(on_the_next_line=True)

    def get_result_for_c_file(self, unit):
        compiled_file = os.path.join(unit['result_path'], f'{unit["name"]}.out')
        unit['result_c_file'] = os.path.join(unit['result_path'], f'{unit["name"]}-c.txt')
        compile_cmd = f'clang {unit["c_file"]} -o {compiled_file} -Wno-everything > /dev/null' \
                      f' 2>>{unit["result_c_file"]}'
        try:
            subprocess.run(compile_cmd, shell=True, check=True)
        except subprocess.CalledProcessError as exc:
            return exc
        else:
            process = subprocess.Popen(f'{compiled_file} >> {unit["result_c_file"]} 2>&1', shell=True)
            timeout = 10
            try:
                process.communicate(timeout=timeout)
            except subprocess.TimeoutExpired:
                subprocess.run(f'pkill -TERM -P {process.pid}', shell=True)
                with open(unit['result_c_file'], 'w') as f:
                    f.write(f'exception: execution time of C file exceeded {timeout} seconds\n')
        finally:
            self.test_handled_count += 1
            tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))

    def get_result_for_eo_file(self, unit):
        command = regex.sub(self.run_sh_replace, unit['full_name'], self.run_sh_cmd)
        unit['result_eo_file'] = os.path.join(unit['result_path'], f'{unit["name"]}-eo.txt')
        process = subprocess.Popen(f'{command} >> {unit["result_eo_file"]} 2>&1', shell=True)
        timeout = 60
        try:
            process.communicate(timeout=timeout)
        except subprocess.TimeoutExpired:
            subprocess.run(f'pkill -TERM -P {process.pid}', shell=True)
            with open(unit['result_eo_file'], 'w') as f:
                f.write(f'exception: execution time EO file exceeded {timeout} seconds\n')
        finally:
            self.test_handled_count += 1
            tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))

    def compare_test_results(self, unit):
        for _filter, comment in self.skips.items():
            if _filter in unit['name']:
                return unit, True, False, False, comment

        with open(unit['result_c_file'], 'r') as f:
            c_data = f.readlines()
        with open(unit['result_eo_file'], 'r') as f:
            eo_data = f.readlines()
        is_except, is_equal, log_data = compare_files(c_data, eo_data)
        with open(os.path.join(unit['result_path'], f'{unit["name"]}.log'), 'w') as f:
            f.writelines(log_data)
        return unit, False, is_except, is_equal, log_data


def compare_files(c_data, eo_data):
    if is_exception(c_data):
        return True, False, c_data

    if is_exception(eo_data):
        return True, False, eo_data

    if len(c_data) != len(eo_data):
        log_data = ['Results have different length!\n', '\nC result:\n']
        log_data.extend(c_data)
        log_data.append('\nEO result:\n')
        log_data.extend(eo_data)
        return False, False, log_data

    is_equal, log_data = compare_lines(c_data, eo_data)
    return False, is_equal, log_data


def is_exception(lines):
    return len(lines) > 0 and ('exception' in lines[0].casefold() or 'error' in lines[0].casefold())


def compare_lines(c_data, eo_data):
    is_equal = True
    log_data = []
    for i, (c_line, eo_line) in enumerate(zip(c_data, eo_data)):
        c_line = c_line.rstrip()
        eo_line = eo_line.rstrip()
        ok_line = f'\t{tools.BGreen}Line {i}: {c_line} == {eo_line}{tools.IWhite}\n'
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
            error_line = f'\t{tools.BRed}Line {i}: {c_line} != {eo_line}{tools.IWhite}\n'
            log_data.append(error_line)
    return is_equal, log_data


def group_comparison_results(results):
    result = {tools.PASS: [], tools.ERROR: [], tools.EXCEPTION: {}, tools.SKIP: {}}
    tools.pprint('Getting results\n', slowly=True)
    for unit, is_skip, is_except, is_equal, log_data in results:
        if is_skip:
            if log_data not in result[tools.SKIP]:
                result[tools.SKIP][log_data] = []
            result[tools.SKIP][log_data].append(unit['name'])
        elif is_except:
            log_data = ''.join(log_data)
            if log_data not in result[tools.EXCEPTION]:
                result[tools.EXCEPTION][log_data] = []
            result[tools.EXCEPTION][log_data].append(unit['name'])
        elif is_equal:
            result[tools.PASS].append(unit['name'])
        else:
            result[tools.ERROR].append((unit['name'], log_data))
    return result


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    fails_count = Tests(tools.get_or_none(sys.argv, 1), tools.get_or_none(sys.argv, 2)).test()
    if fails_count:
        exit(f'{fails_count} tests failed')
