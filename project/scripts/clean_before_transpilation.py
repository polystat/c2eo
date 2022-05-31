#! /usr/bin/python3

import sys

# Our scripts
import tools
import settings


def main(path_to_tests=None):
    if path_to_tests is None:
        path_to_tests = settings.get_setting('path_to_tests')
    patterns = settings.get_setting('patterns_for_cleaning')
    tools.clear_dir_by_patterns(path_to_tests, patterns, recursive=True)
    tools.remove_empty_dirs(path_to_tests)
    path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
    tools.clear_dir_by_patterns(path_to_c2eo_build, ['*.eo'])


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
