#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import glob
import shutil
import sys
import re


def main():
    print('\nStart collecting files\n')
    path_to_files = '../assembly'
    with open('meta.txt', 'r') as f:  # Read part of default code for global result
        result_code = f.read()

    result_code += read_code_from_global_files(path_to_files)
    print()
    result_code += read_code_from_static_files(path_to_files)

    print_code("global.eo:", result_code)
    with open(os.path.join(path_to_files, 'global.eo'), 'w') as f:
        f.write(result_code)
    print('Collecting files done')
    return


def read_code_from_global_files(dir):
    code = ''
    for file in search_files_by_pattern(dir, '*.glob'):
        code += read_code_from_file(file, indent='  ')
    return code


def read_code_from_static_files(dir):
    code = ''
    for file in search_files_by_pattern(dir, '*.stat'):
        name = get_only_file_name(file)
        code += f'  [] > {name}\n'
        code += read_code_from_file(file, indent='    ')
    return code


def get_only_file_name(path):
    file = os.path.basename(path)
    name = os.path.splitext(file)[0]
    return name


def search_files_by_pattern(dir, file_pattern):
    print(f'Looking for "{file_pattern}" files')
    path = os.path.join(dir, file_pattern)
    found_files = glob.glob(path)
    # Keep only file basename
    file_names = list(map(lambda x: os.path.basename(x), found_files))
    print(f'Found {len(found_files)} files: {file_names}')
    return found_files


def read_code_from_file(file, indent):
    code = ''
    with open(file, 'r') as f:
        for line in f:
            if line != '\n':
                code += indent
            code += line
    return code


def print_code(title, code):
    print(f'\n{title}')
    print('-' * 50)
    print(code)
    print('-' * 50)


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
