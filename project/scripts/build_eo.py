#! /usr/bin/python3

import os
import sys
import subprocess

# Our scripts
import settings


def main(path_to_eo_project=None):
    if path_to_eo_project is None:
        path_to_eo_project = settings.get_setting('path_to_eo_project')
    original_path = os.getcwd()
    os.chdir(path_to_eo_project)
    subprocess.run('mvn clean compile', shell=True)
    os.chdir(original_path)


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    if len(sys.argv) == 1:
        main()
    else:
        main(sys.argv[1])
