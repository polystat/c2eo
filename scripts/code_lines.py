#! /usr/bin/python3
# SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
# SPDX-License-Identifier: MIT

import sys
import subprocess
from pathlib import Path

if __name__ == '__main__':
    path_to_files = Path(sys.argv[1]).resolve()
    if not path_to_files.exists():
        exit('This path does not exist')

    code_lines = {'c': 0, 'i': 0, 'eo': 0, 'h': 0}
    if len(sys.argv) == 3 and sys.argv[2] in code_lines.keys():
        code_lines = {sys.argv[2]: 0}
    for extension in code_lines.keys():
        files = path_to_files.rglob(f'*.{extension}')
        if extension == 'c':
            files = {f for f in files if not f.match('-eo.c')}
        lines_count = 0
        files_count = 0
        for file in files:
            if result := subprocess.run(f'wc -l {file}', shell=True, text=True, capture_output=True).stdout.split():
                lines_count += int(result[0])
                files_count += 1
        print(f'*.{extension:2} | files: {files_count:5,} | lines: {lines_count:7,}')
