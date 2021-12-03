# Useful functions for our project

import os
import glob
from os.path import join


def search_files_by_pattern(path, file_pattern, recursive=False, print_files=False):
    if recursive:
        path = join(path, '**')
    print(f'\nLooking for "{file_pattern}" files in "{path}"')
    found_files = glob.glob(f'{join(path, file_pattern)}', recursive=recursive)
    # Keep only file basename
    print(f'Found {len(found_files)} files')
    if print_files:
        file_names = sorted(list(map(lambda x: os.path.basename(x), found_files)))
        print(file_names)
    return found_files


def split_path(path_to_file, with_end_sep=False):
    path, file = os.path.split(path_to_file)
    if with_end_sep:
        path += os.sep
    file_name, extension = os.path.splitext(file)
    return path, file_name, extension


def get_file_name(path):
    file = os.path.basename(path)
    name = os.path.splitext(file)[0]
    return name


def clear_dir_by_pattern(path, file_pattern, recursive=False, print_files=False):
    found_files = search_files_by_pattern(path, file_pattern, recursive=recursive, print_files=print_files)
    for file in found_files:
        os.remove(file)
    print('Files removed')


def colorize_text(text, color):
    colors = {'blue': '\033[36m', 'white': '\033[37m', 'red': '\033[31m', 'green': '\033[32m'}
    return f'{colors[color]}{text}{colors["white"]}'
