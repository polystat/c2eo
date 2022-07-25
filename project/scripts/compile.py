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

# Our scripts
import tools
import settings
from build_eo import EOBuilder
from transpile_c2eo import Transpiler


class Compiler(object):

    def __init__(self, path_to_tests=None, skips_file_name=None):
        if path_to_tests is None:
            path_to_tests = settings.get_setting('path_to_tests')
        if skips_file_name is None:
            skips_file_name = settings.get_setting('skips')
        self.skips = settings.get_skips(skips_file_name)
        self.path_to_tests = path_to_tests
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.transpilation_units = []

    def compile(self):
        self.transpilation_units = Transpiler(self.path_to_tests, False).transpile()
        if self.transpilation_units:
            EOBuilder().build()
            tools.pprint('COMPILE RESULTS', slowly=True)
            tools.pprint_separation_line()
            tools.pprint(f'{len(self.transpilation_units)} files', slowly=True, status=tools.PASS)


if __name__ == '__main__':
    start_time = time.time()
    tools.move_to_script_dir(sys.argv[0])
    Compiler(tools.get_or_none(sys.argv, 1), tools.get_or_none(sys.argv, 2)).compile()
    end_time = time.time()
    time_span = int(end_time - start_time)
    tools.pprint('Total time:  {:02}:{:02} min.'.format(time_span // 60, time_span % 60), slowly=True)
    tools.pprint_separation_line()
