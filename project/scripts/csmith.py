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
import argparse
import subprocess

# Our scripts
import tools
import settings


class Csmith(object):

    def __init__(self, path_to_generate, files_count):
        self.csmith_args = ' '.join([f'--{arg}' for arg in settings.get_setting('csmith_args')])
        self.path_to_csmith = os.path.abspath(os.path.join(settings.get_setting('path_to_csmith'), 'csmith'))
        self.path_to_generate = path_to_generate
        self.files_count = files_count
        self.generated_files_count = 0

    def generate(self):
        os.makedirs(self.path_to_generate, exist_ok=True)
        os.chdir(self.path_to_generate)
        tools.pprint('\nRunning generating files:\n', slowly=True)
        tools.print_progress_bar(0, self.files_count)
        with tools.thread_pool() as threads:
            results = threads.map(self.generate_file, range(self.files_count))
        print()
        tools.pprint('\nGenerated files: ')
        for file_name, code in sorted(results, key=lambda x: x[0]):
            print(f'{file_name}:\n')
            print(code)

    def generate_file(self, number):
        result = subprocess.run(f'{self.path_to_csmith} {self.csmith_args}', shell=True, text=True, capture_output=True)
        file_name = f'{number + 1:0{len(str(self.files_count))}}.c'
        with open(file_name, 'w') as f:
            f.write(result.stdout)
        self.generated_files_count += 1
        tools.print_progress_bar(self.generated_files_count, self.files_count)
        return file_name, result.stdout


def create_parser():
    _parser = argparse.ArgumentParser(description='the script for generating csmith testsuite for c2eo transpiler')

    _parser.add_argument('path_to_generate', metavar='PATH',
                         help='the relative path from the scripts folder to the generating folder')

    _parser.add_argument('-c', '--count_of_files', metavar='COUNT', default=1, type=int,
                         help='the count of generating files')

    return _parser


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    parser = create_parser()
    namespace = parser.parse_args()
    Csmith(namespace.path_to_generate, namespace.count_of_files).generate()
