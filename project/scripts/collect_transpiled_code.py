#! /usr/bin/python3
# Collect all generated eo files into one global.

import os
import sys
import re as regex

# Our scripts
import tools
import settings


def main(name):
    path_to_assembly = settings.get_setting('path_to_assembly')
    file = f'{path_to_assembly}{name}.glob'
    code = read_code_from_static_file(file.replace('.glob', '.stat'), name)
    code += read_code_from_global_file(file)
    aliases_code = get_aliases_code(name, code)
    result = aliases_code + code
    with open(file.replace('.glob', '.eo'), 'w') as f:
        f.write(result)
    return


def get_aliases_code(name, main_code):
    aliases_code = settings.get_meta_code('aliases', read_as_lines=True)
    aliases_code[0] = aliases_code[0].replace('c2eo.src', f'c2eo.src.{name}', 1)
    aliases = []
    for i, line in enumerate(aliases_code[1:], start=1):
        result = regex.search(r'(?<=\.)[\w-]+$(?=\n)', line)
        if result:
            aliases.append((i, result.group(0)))
    length = len(aliases_code)
    useful_lines = [0, length - 2, length - 1]
    for i, alias in aliases:
        result = regex.search(rf'(?<=[^>][ (]){alias}(?=[ )\n])', main_code)
        if result:
            useful_lines.append(i)
    code = ''
    for i in useful_lines:
        code += aliases_code[i]
    return code


def read_code_from_static_file(file, name):
    code = ''
    if os.path.exists(file):
        code += f'  [] > {name}\n'
        code += read_code_from_file(file, indent='    ')
    return code


def read_code_from_global_file(file):
    code = ''
    if os.path.exists(file):
        code += read_code_from_file(file, indent='  ')
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
    tools.move_to_script_dir(sys.argv[0])
    main(sys.argv[1])
