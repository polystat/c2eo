#! /usr/bin/python3

import os
import sys
import glob
import subprocess
import clean_before_transpilation

if __name__ == '__main__':
    path_to_c_files = os.path.abspath(sys.argv[1])
    if not os.path.exists(path_to_c_files):
        print('This path does not exist')
        exit('This path does not exist')

    clean_before_transpilation.main(path_to_c_files)
    path_to_c_files = os.path.join(path_to_c_files, '**')
    path_to_c2eo = os.path.abspath('../bin/c2eo')
    c_files = glob.glob(os.path.join(path_to_c_files, '*.c'), recursive=True)
    for c_file in c_files:
        result_dir = os.path.join(os.path.dirname(c_file), 'c2eo-result')
        if not os.path.exists(result_dir):
            os.makedirs(result_dir, exist_ok=True)
        os.chdir(result_dir)
        c_file_name = os.path.basename(c_file)
        eo_file_name = c_file_name.replace('.c', '.eo')
        subprocess.run(f'{path_to_c2eo} {c_file} {eo_file_name}', shell=True)
