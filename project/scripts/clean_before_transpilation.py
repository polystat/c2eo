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

# Our scripts
import tools
import settings


def main(path_to_tests=None):
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    patterns = settings.get_setting('patterns_for_cleaning')
    tools.clear_dir_by_patterns(path_to_tests, patterns, recursive=True)
    tools.remove_empty_dirs(path_to_tests)
    path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
    tools.clear_dir_by_patterns(path_to_c2eo_build, ['*.eo'])


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
