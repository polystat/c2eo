#! /usr/bin/python3

import sys

# Our scripts
import tools
import settings


def main(path_to_tests=None):
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    for pattern in ['*-eo.c', '*.eo', '*.out', '*.log', '*.txt', '*.i']:
        tools.clear_dir_by_pattern(path_to_tests, pattern, recursive=True)
    path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
    tools.clear_dir_by_pattern(path_to_c2eo_build, '*.eo')


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
