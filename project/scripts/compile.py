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
from pathlib import Path
from subprocess import CompletedProcess

# Our scripts
import tools
import settings
from build_eo import EOBuilder
from transpile import Transpiler


class Compiler(object):

    def __init__(self, path_to_files: Path, skips_file_name: str, need_to_prepare_c_code: bool = True,
                 need_to_generate_codecov: bool = False, ignore_exceptions: bool = False, random_tests_count: int = 0):
        self.need_to_generate_codecov = need_to_generate_codecov
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.ignore_exceptions = ignore_exceptions
        self.skips_file_name = skips_file_name
        self.path_to_tests = path_to_files
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.transpilation_units: list[dict[str, str | Path | CompletedProcess | float]] = []
        self.random_tests_count = random_tests_count

    def compile(self) -> Transpiler.transpile:
        start_time = time.time()
        self.transpilation_units, skip_result = Transpiler(self.path_to_tests, self.skips_file_name,
                                                           self.need_to_prepare_c_code, self.need_to_generate_codecov,
                                                           self.ignore_exceptions, self.random_tests_count).transpile()
        if self.transpilation_units:
            builder = EOBuilder(self.transpilation_units)
            if not builder.can_recompile:
                tools.clear_dir_by_patterns(self.path_to_tests, {'*-eo.txt'}, recursive=True)
            errors, error_result = builder.build()
            passes = {unit['unique_name'] for unit in self.transpilation_units} - errors
            result = {tools.PASS: passes, tools.ERROR: error_result, tools.SKIP: skip_result}
            tests_count = len(self.transpilation_units) + sum(map(len, skip_result.values()))
            tools.pprint_result('COMPILE', tests_count, int(time.time() - start_time), result, False)
        return self.transpilation_units, skip_result


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(
        description='This script transpilates C files into EO files and compiles them. It reuses transpile script and'
                    'starts to work after ending its work. It checks conditions for recompilation of the project and'
                    'then recompile or compile it.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('-p', '--path_to_files', metavar='PATH',
                         help='the relative path from the scripts folder to the c files folder')

    _parser.add_argument('-s', '--skips_file_name', metavar='FILE_NAME', default='',
                         help='the name of the file with a set of skips for tests')

    _parser.add_argument('-n', '--not_prepare_c_code', action='store_const', const=True, default=False,
                         help='the script will not change the c code in the input files')

    _parser.add_argument('-c', '--codecov', action='store_const', const=True, default=False,
                         help='the script will generate codecov files')

    _parser.add_argument('-i', '--ignore_exceptions', action='store_const', const=True, default=False,
                         help='the script will continue to work after the test exceptions')

    _parser.add_argument('-r', '--random_tests_count', type=int, default=0,
                         help='this parameter is used for large test suites when we cannot test all the tests at once,'
                              'instead we select a specified count of random tests.')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    Compiler(Path(namespace.path_to_files), namespace.skips_file_name, not namespace.not_prepare_c_code,
             namespace.codecov, namespace.ignore_exceptions, namespace.random_tests_count).compile()
