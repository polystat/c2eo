#! /usr/bin/python3
# Main script for transpilation run

import os
import sys
import shutil

# Our scripts
import tools
import collector


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
    tools.clear_dir_by_pattern(path_to_assembly, '*')
    print('\nTranspilation start\n')
    output_name = 'static01'
    os.system(f'./c2eo {path_to_c_file} {output_name}')
    print('=' * 35)
    return


def start_collecting(path_from, path_to):
    tools.clear_dir_by_pattern(path_to, '*')
    collector.main()
    print('Move global.eo to src dir')
    shutil.move(os.path.join(path_from, 'global.eo'), path_to)
    return


if __name__ == '__main__':
    main()
