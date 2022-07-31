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
import argparse

# Our scripts
import tools
import settings
from build_eo import EOBuilder
from transpile import Transpiler


class Compiler(object):

    def __init__(self, path_to_files, skips_file_name, need_to_prepare_c_code=True):
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.skips = settings.get_skips(skips_file_name)
        self.path_to_tests = path_to_files
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.transpilation_units = []

    def compile(self):
        start_time = time.time()
        self.transpilation_units = Transpiler(self.path_to_tests, '', self.need_to_prepare_c_code).transpile()
        skips = self.remove_skip_files()
        if self.transpilation_units:
            EOBuilder().build()
            passes = set(unit['unique_name'] for unit in self.transpilation_units)
            passes -= set(file for value in skips.values() for file in value.keys())
            result = {tools.PASS: passes, tools.SKIP: skips}
            tools.pprint_result('COMPILE', len(self.transpilation_units), int(time.time() - start_time), result, 0)
        return self.transpilation_units

    def remove_skip_files(self):
        skips = {}
        for unit in self.transpilation_units:
            for _filter, comment in self.skips.items():
                if _filter in unit['name']:
                    if comment not in skips:
                        skips[comment] = {}
                    if os.path.exists(unit['src_eo_file']):
                        os.remove(unit['src_eo_file'])
                    skips[comment][unit['unique_name']] = set()
                    break
        return skips


def create_parser():
    _parser = argparse.ArgumentParser(description='the script for compiling translated files from C to EO')

    _parser.add_argument('-p', '--path_to_files', metavar='PATH',
                         help='the relative path from the scripts folder to the c files folder')

    _parser.add_argument('-s', '--skips_file_name', metavar='FILE_NAME', default='',
                         help='the name of the file with a set of skips for tests')

    _parser.add_argument('-n', '--not_prepare_c_code', action='store_const', const=True, default=False,
                         help='the script will not change the c code in the input files')
    return _parser


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    parser = create_parser()
    namespace = parser.parse_args()
    Compiler(namespace.path_to_files, namespace.skips_file_name, not namespace.not_prepare_c_code).compile()
