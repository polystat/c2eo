#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import sys

# Our scripts
import tools
import settings


def main(global_file):
    make_final_eo_file(global_file)
    return


def make_final_eo_file(file):
    code = ''
    file_name = tools.get_file_name(file)
    code += settings.get_meta_code('aliases')
    code.replace('c2eo.src', f'c2eo.src.{file_name}', 1)
    code += read_code_from_file(file, file_name)
    code += read_code_from_static_file(file.replace('.glob', '.stat'), file_name)
    with open(file.replace('.glob', '.eo'), 'w') as f:
        f.write(code)


def read_code_from_static_file(file, name):
    code = ''
    if os.path.exists(file):
        code += f'  [] > {name}\n'
        code += read_code_from_file(file, indent='    ')
    return code


def read_code_from_file(file, indent):
    code = ''
    with open(file, 'r') as f:
        for line in f:
            if line != '\n':
                code += indent
            code += line
    return code


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
