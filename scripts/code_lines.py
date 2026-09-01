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
import subprocess
from pathlib import Path

if __name__ == '__main__':
    path_to_files = Path(sys.argv[1]).resolve()
    if not path_to_files.exists():
        exit('This path does not exist')

    code_lines = {'c': 0, 'i': 0, 'eo': 0, 'h': 0}
    if len(sys.argv) == 3 and sys.argv[2] in code_lines.keys():
        code_lines = {sys.argv[2]: 0}
    for extension in code_lines.keys():
        files = path_to_files.rglob(f'*.{extension}')
        if extension == 'c':
            files = {f for f in files if not f.match('-eo.c')}
        lines_count = 0
        files_count = 0
        for file in files:
            if result := subprocess.run(f'wc -l {file}', shell=True, text=True, capture_output=True).stdout.split():
                lines_count += int(result[0])
                files_count += 1
        print(f'*.{extension:2} | files: {files_count:5,} | lines: {lines_count:7,}')
