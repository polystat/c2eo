# Useful functions for our project

import os
import glob


def search_files_by_pattern(path, file_pattern, recursive=False, print_files=False):
    if recursive:
        path = os.path.join(path, '**')
    print(f'\nLooking for "{file_pattern}" files in "{path}"')
    found_files = glob.glob(f'{os.path.join(path, file_pattern)}', recursive=recursive)
    # Keep only file basename
    print(f'Found {len(found_files)} files')
    if print_files:
        file_names = sorted(list(map(lambda x: os.path.basename(x), found_files)))
        print(file_names)
    return found_files


def get_file_name(path):
    file = os.path.basename(path)
    name = os.path.splitext(file)[0]
    return name


def clear_dir_by_pattern(path, file_pattern, recursive=False, print_files=False):
    found_files = search_files_by_pattern(path, file_pattern, recursive=recursive, print_files=print_files)
    for file in found_files:
        os.remove(file)
    print('Files removed')
