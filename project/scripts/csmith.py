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
import shutil
import argparse
import subprocess
from os import chdir
from pathlib import Path

# Our scripts
import tools
import settings


class Csmith(object):

    def __init__(self, path_to_generate: Path, files_count: int):
        self.csmith_args = ' '.join([f'--{arg}' for arg in settings.get_setting('csmith_args')])
        self.path_to_csmith = (settings.get_setting('path_to_csmith') / 'csmith').resolve()
        self.path_to_csmith_runtime = settings.get_setting('path_to_csmith_runtime')
        self.path_to_generate = path_to_generate
        self.files_count = files_count
        self.generated_files_count = 0

    def generate(self) -> None:
        tools.pprint('\n', 'Making the dir:', slowly=True)
        if self.path_to_generate.exists():
            tools.clear_dir_by_patterns(self.path_to_generate, {'*.c', '*.h'})
        else:
            self.path_to_generate.mkdir(exist_ok=True)
        tools.pprint('\n', 'Copying runtime files into the dir:', slowly=True)
        for file in tools.search_files_by_patterns(self.path_to_csmith_runtime, {'*.h'}, print_files=True):
            shutil.copy(file, self.path_to_generate)
        chdir(self.path_to_generate)
        tools.pprint('\n', 'Running generating files:', '\n', slowly=True)
        tools.print_progress_bar(0, self.files_count)
        with tools.thread_pool() as threads:
            list(threads.imap_unordered(self.generate_file, range(self.files_count)))

    def generate_file(self, number: int) -> (str, list[str]):
        result = subprocess.run(f'{self.path_to_csmith} {self.csmith_args}', shell=True, text=True, capture_output=True)
        file_name = Path(f'{number + 1:0{len(str(self.files_count))}}.c')
        file_name.write_text(result.stdout)
        self.generated_files_count += 1
        tools.print_progress_bar(self.generated_files_count, self.files_count)
        return file_name, result.stderr if result.returncode else result.stdout


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for generating csmith testsuite for c2eo transpiler',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('path_to_generate', metavar='PATH',
                         help='the relative path from the scripts folder to the generating folder')

    _parser.add_argument('-c', '--count_of_files', metavar='COUNT', default=1, type=int,
                         help='the count of generating files')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    Csmith(Path(namespace.path_to_generate), namespace.count_of_files).generate()
