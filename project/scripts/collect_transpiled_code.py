#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import sys

# Our scripts
import tools
import settings


def main():
    print('\nStart collecting files\n')
    path_to_files = settings.get_setting('path_to_assembly')

    result_code = settings.get_meta_code('aliases')
    result_code += read_code_from_global_files(path_to_files, '*.glob')
    print()

    print_code('global.eo:', result_code)
    with open(f'{path_to_files}global.eo', 'w') as f:
        f.write(result_code)
    print('Collecting files done')
    return


def read_code_from_global_files(path, pattern):
    code = ''
    for file in tools.search_files_by_pattern(path, pattern):
        name = tools.get_file_name(file)
        code += f'[arg] > {name}\n'
        code += read_code_from_static_file(f'{path}{name}.stat')
        code += read_code_from_file(file, indent='  ').replace(f'{name}.', '')
    return code


def read_code_from_static_file(file):
    code = ''
    if os.path.exists(file):
        code += read_code_from_file(file, indent='  ')
    return code


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
