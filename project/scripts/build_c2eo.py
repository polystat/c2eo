#! /usr/bin/python3

import os
import sys


def main():
    print("\nBuild c2eo")
    os.chdir('../bin')
    os.system('cmake ..')
    os.system('make')
    print("c2eo builded")


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
