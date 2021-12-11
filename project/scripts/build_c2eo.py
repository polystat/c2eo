#! /usr/bin/python3

import os
import sys
import subprocess

# Our scripts
import tools
import settings


def main(path_to_c2eo=None):
    if path_to_c2eo is None:
        path_to_c2eo = settings.get_setting('path_to_c2eo')
    tools.pprint()
    original_path = os.getcwd()
    os.chdir(path_to_c2eo)
    subprocess.run('cmake ..', shell=True)
    subprocess.run('make', shell=True)
    os.chdir(original_path)
    tools.pprint()


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        main()
    else:
        main(sys.argv[1])
