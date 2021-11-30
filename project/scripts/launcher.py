#! /usr/bin/python3
# launcher.py Main script for transpilation run

import os
import sys
import glob
import shutil


def main():
    path_to_c_file = os.path.abspath(sys.argv[1])
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir

    assembly_path = '../assembly/'
    start_transpilation(assembly_path, path_to_c_file)

    result_path = '../../result/eo/c2eo/src/'
    start_collecting(assembly_path, result_path)
    print('Transpilation done\n')
    return


def start_transpilation(path_to_assembly, path_to_c_file):
    clear_dir(path_to_assembly, '*')
    print('\nTranspilation start\n')
    output_name = 'static01'
    os.system(f'./c2eo {path_to_c_file} {output_name}')
    print('=' * 35)
    return


def start_collecting(path_from, path_to):
    clear_dir(path_to, '*')
    os.system('./collector.py')
    print('Move global.eo to src dir')
    shutil.move(os.path.join(path_from, 'global.eo'), path_to)
    return


def clear_dir(path, file_pattern):
    found_files = search_files_by_pattern(path, file_pattern)
    for file in found_files:
        os.remove(file)
    print('Files removed')
    return


def search_files_by_pattern(path, file_pattern):
    print(f'\nLooking for "{file_pattern}" files in "{path}" dir')
    pattern = os.path.join(path, file_pattern)
    found_files = glob.glob(pattern)
    # Keep only file basename
    file_names = list(map(lambda x: os.path.basename(x), found_files))
    print(f'Found {len(found_files)} files: {file_names}')
    return found_files


if __name__ == '__main__':
    main()
