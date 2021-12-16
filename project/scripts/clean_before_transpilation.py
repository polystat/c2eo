#! /usr/bin/python3

import sys

# Our scripts
import tools
import settings


def main(path_to_tests=None):
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    for pattern in ['*-eo.c', '*.eo', '*.out', '*.log', '*.glob', '*.stat', '*.txt']:
        tools.clear_dir_by_pattern(path_to_tests, pattern, recursive=True)
    tools.clear_dir_by_pattern(settings.get_setting('path_to_assembly'), '*')


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
