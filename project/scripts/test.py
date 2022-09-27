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

import sys
import time
import argparse
import subprocess
import re as regex
from os import chdir
from pathlib import Path
from subprocess import CompletedProcess

# Our scripts
import tools
import settings
from compile import Compiler


class Tests(object):

    def __init__(self, path_to_tests: Path, skips_file_name: str, need_to_prepare_c_code: bool = True,
                 need_to_generate_codecov: bool = False):
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.need_to_generate_codecov = need_to_generate_codecov
        self.skips_file_name = skips_file_name
        self.path_to_tests = path_to_tests
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src')
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_eo_coperators = settings.get_setting('path_to_eo_coperators')
        self.run_sh_cmd = settings.get_meta_code('run.sh', read_as_lines=True)[2].rstrip()
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.transpilation_units: list[dict[str, str | Path | CompletedProcess | float]] = []
        self.test_handled_count = 0

    def test(self) -> bool:
        start_time = time.time()
        self.transpilation_units, skip_result = Compiler(self.path_to_tests, self.skips_file_name,
                                                         self.need_to_prepare_c_code,
                                                         self.need_to_generate_codecov).compile()
        if self.transpilation_units:
            self.get_result_for_tests()
            with tools.thread_pool() as threads:
                results = list(threads.imap_unordered(compare_test_results, self.transpilation_units))
            result = group_comparison_results(results)
            result[tools.SKIP] = skip_result
            result[tools.TIME] = self.transpilation_units
            tests_count = len(self.transpilation_units) + sum(map(len, skip_result.values()))
            _is_failed = len(result[tools.ERROR]) + len(result[tools.EXCEPTION]) > 0
            tools.pprint_result('TEST', tests_count, int(time.time() - start_time), result, _is_failed)
            return _is_failed

    def get_result_for_tests(self) -> None:
        tools.pprint('\n', 'Running C tests:', '\n', slowly=True)
        tools.print_progress_bar(0, len(self.transpilation_units))
        with tools.thread_pool() as threads:
            list(threads.imap_unordered(self.get_result_for_c_file, self.transpilation_units))
        tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))
        tools.pprint(on_the_next_line=True)

        ctimes = [x.stat().st_ctime for x in tools.search_files_by_patterns(self.path_to_eo_coperators, {'*.eo'})]
        self.last_coperators_ctime = max(ctimes)
        tools.pprint('\n', 'Running EO tests:', '\n', slowly=True)
        original_path = Path.cwd()
        chdir(self.path_to_eo_project)
        self.test_handled_count = 0
        tools.print_progress_bar(0, len(self.transpilation_units))
        with tools.thread_pool() as threads:
            list(threads.imap_unordered(self.get_result_for_eo_file, self.transpilation_units))
        tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))
        chdir(original_path)
        tools.pprint(on_the_next_line=True)

    def get_result_for_c_file(self, unit: dict[str, str | Path | CompletedProcess | float]) -> None:
        unit['result_c_file'] = unit['result_path'] / f'{unit["name"]}-c.txt'
        if not unit['result_c_file'].exists() or unit['result_c_file'].stat().st_ctime < unit['c_file'].stat().st_ctime:
            compiled_file = unit['result_path'] / f'{unit["name"]}.out'
            compile_cmd = ['clang', unit['c_file'], '-o', compiled_file, '-Wno-everything']
            try:
                subprocess.run(compile_cmd, check=True, capture_output=True, text=True)
            except subprocess.CalledProcessError as exc:
                unit['result_c_file'].write_text(exc.stderr)
            else:
                process = subprocess.Popen([compiled_file], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
                timeout = 10
                try:
                    outs, errs = process.communicate(timeout=timeout)
                    unit['result_c_file'].write_text(outs + errs + str(process.returncode))
                except subprocess.TimeoutExpired:
                    process.kill()
                    exception_msg = f'exception: execution time of C file exceeded {timeout} seconds\n'
                    unit['result_c_file'].write_text(exception_msg)
        self.test_handled_count += 1
        tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))

    def get_result_for_eo_file(self, unit: dict[str, str | Path | CompletedProcess | float]) -> None:
        unit['result_eo_file'] = unit['result_path'] / f'{unit["name"]}-eo.txt'
        unit["eo_test_time"] = 0.0
        if unit['result_eo_file'].exists():
            result_eo_ctime = unit['result_eo_file'].stat().st_ctime
            eo_file_ctime = unit['eo_file'].stat().st_ctime
            is_old_result = result_eo_ctime < eo_file_ctime or result_eo_ctime < self.last_coperators_ctime
        else:
            is_old_result = True

        if is_old_result:
            cmd = ['/usr/bin/time', '-f', '%e']
            cmd.extend(regex.sub(self.run_sh_replace, unit['full_name'].replace('-', '_'), self.run_sh_cmd).split())
            process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            timeout = 60
            try:
                outs, errs = process.communicate(timeout=timeout)
                errs = errs.splitlines()
                unit['result_eo_file'].write_text(outs + '\n'.join(errs[:-1]))
                unit["eo_test_time"] = float(errs[-1])
            except subprocess.TimeoutExpired:
                process.kill()
                unit['result_eo_file'].write_text(f'exception: execution time EO file exceeded {timeout} seconds\n')
                unit["eo_test_time"] = float(timeout)
        self.test_handled_count += 1
        tools.print_progress_bar(self.test_handled_count, len(self.transpilation_units))


def compare_files(c_data: list[str], eo_data: list[str]) -> (bool, bool, list[str]):
    if is_exception(c_data):
        return True, False, c_data

    if is_exception(eo_data):
        return True, False, eo_data

    if len(c_data) != len(eo_data):
        log_data = ['Results have different length!\n', '\nC result:\n']
        log_data.extend(c_data)
        log_data.append('\n\nEO result:\n')
        log_data.extend(eo_data)
        return False, False, log_data

    is_equal, log_data = compare_lines(c_data, eo_data)
    return False, is_equal, log_data


def is_exception(lines: list[str]) -> bool:
    return len(lines) > 0 and ('exception' in lines[0].casefold() or 'error' in lines[0].casefold())


def compare_lines(c_data: list[str], eo_data: list[str]) -> (bool, list[str]):
    is_equal = True
    log_data = []
    for i, (c_line, eo_line) in enumerate(zip(c_data, eo_data), start=1):
        c_line, eo_line = c_line.rstrip(), eo_line.rstrip()
        c_args, eo_args = c_line.split(), eo_line.split()
        is_line_equal = len(c_args) == len(eo_args)
        if is_line_equal:
            for c_arg, eo_arg in zip(c_args, eo_args):
                if c_arg == eo_arg or tools.is_equal_float_strs(c_arg, eo_arg) or tools.is_equal_hex_strs(c_arg,
                                                                                                          eo_arg):
                    continue
                is_line_equal = False
                break

        if is_line_equal:
            log_data.append(f'\t{tools.BGreen}Line {i}: {c_line} == {eo_line}{tools.IWhite}\n')
        else:
            is_equal = False
            log_data.append(f'\t{tools.BRed}Line {i}: {c_line} != {eo_line}{tools.IWhite}\n')
    return is_equal, log_data


def group_comparison_results(results: list[dict[str, str | Path | CompletedProcess | float], bool, bool, list[str]]) -> \
        dict[str, set[str | str, str] | dict[str, dict[str, set[str]]]]:
    result = {tools.PASS: set(), tools.ERROR: set(), tools.EXCEPTION: {}}
    tools.pprint('\n', 'Getting results', '\n', slowly=True)
    for unit, is_except, is_equal, log_data in results:
        if is_except:
            log_data = ''.join(log_data)
            if log_data not in result[tools.EXCEPTION]:
                result[tools.EXCEPTION][log_data] = {}
            result[tools.EXCEPTION][log_data][unit['unique_name']] = set()
        elif is_equal:
            result[tools.PASS].add(unit['unique_name'])
        else:
            result[tools.ERROR].add((unit['unique_name'], ''.join(log_data)))
    return result


def compare_test_results(unit: dict[str, str | Path | CompletedProcess | float]) -> (
        dict[str, str | CompletedProcess], bool, bool, list[str]):
    with open(unit['result_c_file'], 'r') as f:
        c_data = f.readlines()
    with open(unit['result_eo_file'], 'r') as f:
        eo_data = f.readlines()
    is_except, is_equal, log_data = compare_files(c_data, eo_data)
    with open(unit['result_path'] / f'{unit["name"]}.log', 'w') as f:
        f.writelines(log_data)
    return unit, is_except, is_equal, log_data


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for testing the correctness of the execution of '
                                                  'translated files from C to EO',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('-p', '--path_to_tests', metavar='PATH', default=settings.get_setting('path_to_tests'),
                         help='the relative path from the scripts folder to the tests folder')

    _parser.add_argument('-s', '--skips_file_name', metavar='FILE_NAME',
                         help='the name of the file with a set of skips for tests')

    _parser.add_argument('-n', '--not_prepare_c_code', action='store_const', const=True, default=False,
                         help='the script will not change the c code in the input files')

    _parser.add_argument('-c', '--codecov', action='store_const', const=True, default=False,
                         help='the script will generate codecov files')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    is_failed = Tests(Path(namespace.path_to_tests), namespace.skips_file_name, not namespace.not_prepare_c_code,
                      namespace.codecov).test()
    if is_failed:
        exit('Testing failed')
