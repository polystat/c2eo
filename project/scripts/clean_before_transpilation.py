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
from pathlib import Path

# Our scripts
import tools
import settings


def main(path_to_files: Path, cleaning_patterns) -> None:
    cleaning_patterns = set(cleaning_patterns.split())
    if path_to_files.is_file():
        path_to_files = path_to_files.parent
    tools.clear_dir_by_patterns(path_to_files, cleaning_patterns, recursive=True)
    tools.remove_empty_dirs(path_to_files)
    tools.clear_dir_by_patterns(settings.get_setting('path_to_c2eo_transpiler'), {'*.eo', '*.alias'})


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for cleaning the folder from c2eo transpiled files',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('-p', '--path_to_files', metavar='PATH', default=settings.get_setting('path_to_tests'),
                         help='the relative path from the scripts folder to the c files folder')

    _parser.add_argument('-c', '--cleaning_patterns', default=settings.get_setting('cleaning_patterns'),
                         help='the string of file patterns for cleaning')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    main(Path(namespace.path_to_files), namespace.cleaning_patterns)
