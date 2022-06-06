#! /usr/bin/python3

import os
import sys
import glob
import subprocess

if __name__ == '__main__':
    path_to_files = os.path.abspath(sys.argv[1])
    if not os.path.exists(path_to_files):
        print('This path does not exist')
        exit('This path does not exist')

    path_to_files = os.path.join(path_to_files, '**')
    code_lines = {'c': 0, 'i': 0, 'eo': 0, 'h': 0}
    if len(sys.argv) == 3 and sys.argv[2] in code_lines.keys():
        code_lines = {sys.argv[2]: 0}
    for extension in code_lines.keys():
        files = glob.glob(os.path.join(path_to_files, f'*.{extension}'), recursive=True)
        if extension == 'c':
            files = list(filter(lambda f: '-eo.c' not in f, files))
        lines_count = 0
        for file in files:
            result = subprocess.run(f'wc -l {file}', shell=True, text=True, capture_output=True)
            lines_count += int(result.stdout.split()[0])
        lines_count = '{0:7,}'.format(lines_count).replace(',', ' ')
        print(f'*.{extension:2} | files: {len(files):3} | lines: {lines_count}')
