#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import glob
import shutil
import sys
import re


def main():
    print('Start collecting files')
    path_to_files = '../assembly'
    with open('meta.txt', 'r') as f:  # Read part of default code for global result
        result_code = f.read()

    result_code += read_code_from_global_files(path_to_files)
    result_code += read_code_from_static_files(path_to_files)

    print(result_code)
    with open(os.path.join(path_to_files, 'global.eo'), 'w') as f:
        f.write(result_code)
    return


def read_code_from_global_files(dir):
    code = ''
    for file in search_files_by_pattern(dir, '*.glob'):
        code += read_code_from_file(file)
    return code


def read_code_from_static_files(dir):
    code = ''
    for file in search_files_by_pattern(dir, '*.stat'):
        name = get_only_file_name(file)
        print(f'Static object name: "{name}"')
        code += f'  [] > {name}\n'
        code += read_code_from_file(file)
    return code


def get_only_file_name(path):
    file = os.path.basename(path)
    name = os.path.splitext(file)[0]
    return name


def search_files_by_pattern(dir, file_pattern):
    print(f'Looking for "{file_pattern}" files')
    path = os.path.join(dir, file_pattern)
    found_files = glob.glob(path)
    print(f'Found {len(found_files)} files: {found_files}')
    return found_files


def read_code_from_file(file):
    with open(file, 'r') as f:
        code = f.read()
    # Add indentation to the code, except empty lines
    code = re.sub(r'^(?=[^\n])', '  ', data)
    return code


if __name__ == '__main__':
    origin_path = os.getcwd()
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
    os.chdir(origin_path)
