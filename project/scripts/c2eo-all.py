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
import clean_before_transpilation
from os import chdir
from pathlib import Path

if __name__ == '__main__':
    path_to_c_files = Path(sys.argv[1]).resolve()
    if not path_to_c_files.exists():
        exit('This path does not exist')

    clean_before_transpilation.main(path_to_c_files, '*.alias *-eo.c *.eo *.out *.log *.txt *.i')
    path_to_c2eo = Path('../bin/c2eo').resolve()
    c_files = path_to_c_files.rglob('*.c')
    for c_file in c_files:
        result_dir = c_file.parent / 'c2eo-result'
        result_dir.mkdir(exist_ok=True, parents=True)
        chdir(result_dir)
        subprocess.run(f'{path_to_c2eo} {c_file} {c_file.with_suffix(".eo")}', shell=True)
