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
import argparse
import subprocess
from os import chdir
from pathlib import Path

# Our scripts
import tools
import settings


def generate_codecov() -> None:
    chdir(settings.get_setting('path_to_c2eo_transpiler'))
    tools.pprint('Merging profdata\n')
    subprocess.run(f'llvm-profdata-14 merge -sparse *.profraw -o res.profdata', shell=True)
    tools.pprint('Convert res.profdata to report.txt')
    subprocess.run('llvm-cov-14 show ./c2eo ../src/transpiler/*.cpp -instr-profile=res.profdata > report.txt',
                   shell=True)
    tools.clear_dir_by_patterns(Path.cwd(), {'*.profraw', '*.profdata'})


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for generating codecov for c2eo transpiler')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    generate_codecov()
