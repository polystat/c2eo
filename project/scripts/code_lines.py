#! /usr/bin/python3

import os
import sys
import glob

if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    path_to_files = os.path.join(path_to_files, '**')
    code_lines = {'c': 0, 'i': 0, 'eo': 0}
    if len(sys.argv) == 3 and sys.argv[2] in code_lines.keys():
        code_lines = {sys.argv[2]: 0}
    for extension in code_lines.keys():
        files = glob.glob(os.path.join(path_to_files, f'*.{extension}'), recursive=True)
        if extension == 'c':
            files = list(filter(lambda f: '-eo.c' not in f, files))
        lines_count = sum([len(open(f).readlines()) for f in files])
        lines_count = '{0:7,}'.format(lines_count).replace(',', ' ')
        print(f'*.{extension:2} | files: {len(files):3} | lines: {lines_count}')
