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
import glob
import subprocess

if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    if not os.path.exists(path_to_files):
        print('This path does not exist')
        exit('This path does not exist')

    path_to_files = os.path.join(path_to_files, '**')
    code_lines = {'c': 0, 'i': 0, 'eo': 0, 'h': 0}
    if len(sys.argv) == 3 and sys.argv[2] in code_lines.keys():
        code_lines = {sys.argv[2]: 0}
    for extension in code_lines.keys():
        files = glob.glob(os.path.join(path_to_files, f'*.{extension}'), recursive=True)
        if extension == 'c':
            files = list(filter(lambda f: '-eo.c' not in f, files))
        lines_count = 0
        for file in files:
            result = subprocess.run(f'wc -l {file}', shell=True, text=True, capture_output=True)
            lines_count += int(result.stdout.split()[0])
        lines_count = '{0:7,}'.format(lines_count).replace(',', ' ')
        print(f'*.{extension:2} | files: {len(files):3} | lines: {lines_count}')
