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
    if not os.path.exists(path_to_c2eo_build):
        os.mkdir(path_to_c2eo_build)
    os.chdir(path_to_c2eo_build)
    result = subprocess.run('cmake ..', shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        tools.pprint_status_result('cmake ..', tools.EXCEPTION, result.stderr)
        os.chdir(original_path)
        exit('Failed during cmake execution')

    tools.pprint(result.stdout, slowly=True)
    result = subprocess.run(f'make -j {tools.cpu_count()}', shell=True)
    os.chdir(original_path)
    if result.returncode != 0:
        exit('Failed during make execution')
    tools.pprint()


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main(tools.get_or_none(sys.argv, 1))
