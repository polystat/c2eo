#! /usr/bin/python3

import os
import sys


def main(path_to_c2eo):
    print("\nBuild c2eo")
    original_path = os.getcwd()
    os.chdir(path_to_c2eo)
    os.system('cmake ..')
    os.system('make')
    os.chdir(original_path)
    print()


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main(os.path.dirname(sys.argv[1]))
