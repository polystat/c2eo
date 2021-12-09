#! /usr/bin/python3

import os
import sys

# Our scripts
import tools
import settings


def main(path_to_tests=None):
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    for pattern in ['*-eo.c', '*.eo', '*.out', '*.log', '*.glob', '*.stat', '*.txt']:
        tools.clear_dir_by_pattern(path_to_tests, pattern, recursive=True)


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        main()
    elif len(sys.argv) == 2:
        main(sys.argv[1])
