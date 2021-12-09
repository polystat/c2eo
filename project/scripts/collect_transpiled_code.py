#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import sys

# Our scripts
import settings


def main(name):
    path_to_assembly = settings.get_setting('path_to_assembly')
    code = ''
    code += settings.get_meta_code('aliases').replace('c2eo.src', f'c2eo.src.{name}', 1)
    file = f'{path_to_assembly}{name}.glob'
    code += read_code_from_static_file(file.replace('.glob', '.stat'), name)
    code += read_code_from_file(file, indent='  ')
    with open(file.replace('.glob', '.eo'), 'w') as f:
        f.write(code)
    return


def read_code_from_static_file(file, name):
    code = ''
    if os.path.exists(file):
        code += f'  [] > {name}\n'
        code += read_code_from_file(file, indent='    ')
    return code


def read_code_from_file(file, indent=''):
    code = ''
    with open(file, 'r') as f:
        for line in f:
            if line != '\n':
                code += indent
            code += line
    return code


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main(sys.argv[1])
