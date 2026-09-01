#! /usr/bin/python3
# SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
# SPDX-License-Identifier: MIT

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
