#! /usr/bin/python3
# SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
# SPDX-License-Identifier: MIT

import sys
import subprocess
import clean_before_transpilation
from os import chdir
from pathlib import Path

if __name__ == '__main__':
    path_to_c_files = Path(sys.argv[1]).resolve()
    if not path_to_c_files.exists():
        exit('This path does not exist')

    clean_before_transpilation.main(path_to_c_files, '*.alias *-eo.c *.eo *.out *.log *.txt *.i')
    path_to_c2eo = Path('../bin/c2eo').resolve()
    c_files = path_to_c_files.rglob('*.c')
    for c_file in c_files:
        result_dir = c_file.parent / 'c2eo-result'
        result_dir.mkdir(exist_ok=True, parents=True)
        chdir(result_dir)
        subprocess.run(f'{path_to_c2eo} {c_file} {c_file.with_suffix(".eo")}', shell=True)
