#! /usr/bin/python3

import os
import sys
import glob
import subprocess

if __name__ == '__main__':
    path_to_c_files = os.path.abspath(sys.argv[1])
    c_files = glob.glob(os.path.join(path_to_c_files, '*.c'))
    path_to_c2eo = os.path.abspath('../bin/c2eo')
    result_dir = os.path.join(path_to_c_files, 'c2eo-result')
    if not os.path.exists(result_dir):
        os.makedirs(result_dir, exist_ok=True)
    for f in glob.glob(os.path.join(result_dir, '*')):
        os.remove(f)
    os.chdir(result_dir)
    for c_file in c_files:
        c_file_name = os.path.basename(c_file)
        eo_file_name = c_file_name.replace('.c', '.eo')
        subprocess.run(f'{path_to_c2eo} {c_file} {eo_file_name}', shell=True)
