#! /usr/bin/python3

import os
import sys
import subprocess

# Our scripts
import tools
import settings


def main(path_to_c2eo_build=None):
    if path_to_c2eo_build is None:
        path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
    tools.pprint()
    original_path = os.getcwd()
    os.chdir(path_to_c2eo_build)
    result = subprocess.run('cmake ..', shell=True, capture_output=True, text=True)
    success = True
    if result.returncode == 0:
        tools.pprint(result.stdout, slowly=True)
    else:
        tools.pprint_exception('cmake ..', result.stderr)
        success = False

    if success:
        subprocess.run('make', shell=True)
        tools.pprint()

    os.chdir(original_path)
    return success


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
