# Useful functions for our project

import os
import glob


def search_files_by_pattern(path, file_pattern, recursive=False):
    print(f'\nLooking for "{file_pattern}" files in "{path}" dir')
    pattern = os.path.join(path, file_pattern)
    found_files = glob.glob(pattern, recursive=recursive)
    # Keep only file basename
    file_names = list(map(lambda x: os.path.basename(x), found_files)).sort()
    print(f'Found {len(found_files)} files: {file_names}')
    return found_files


def clear_dir_by_pattern(path, file_pattern):
    found_files = search_files_by_pattern(path, file_pattern)
    for file in found_files:
        os.remove(file)
    print('Files removed')
    return