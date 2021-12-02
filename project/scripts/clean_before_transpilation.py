#! /usr/bin/python3

import os
import sys

# Our scripts
import tools
import settings


def main(path_to_tests=None, path_to_eo_project=None):
    if path_to_eo_project is None:
        path_to_eo_project = settings.get_setting('path_to_eo_project')
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    for pattern in ['*.txt', '*-eo.c', '*.eo', '*.out', '*.log']:
        tools.clear_dir_by_pattern(path_to_tests, pattern, recursive=True)
    tools.clear_dir_by_pattern(path_to_eo_project, '*.*')


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        main()
    elif len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        main(sys.argv[1], sys.argv[2])
