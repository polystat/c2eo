# Useful functions for our project

import os
import glob
import time
import re
from multiprocessing.dummy import Pool as ThreadPool


def search_files_by_pattern(path, file_pattern, filters=None, recursive=False, print_files=False):
    if filters is None:
        filters = []
    if recursive:
        path = os.path.join(path, '**')
    print(f'\nLooking for "{file_pattern}" files in "{path}"')
    found_files = glob.glob(f'{os.path.join(path, file_pattern)}', recursive=recursive)
    print(f'Found {len(found_files)} files')
    found_files = filter_files(found_files, filters)
    # Keep only file basename
    if print_files:
        print()
        print_only_file_names(found_files)
        print()
    return found_files


def filter_files(files, filters):
    if len(filters) == 0:
        return files

    files = set(files)
    print(f'Apply filter: {filters} to found files')
    excluded_files = set()
    filtered_files = set()
    for f in filters:
        if '!' in f:
            excluded_files |= set(filter(lambda file: f[1:] in file, files))
        else:
            filtered_files |= set(filter(lambda x: f in x, files))

    if len(excluded_files) != 0:
        files -= excluded_files

    if len(filtered_files) != 0:
        files = filtered_files

    print(f'{len(files)} files left:\n')
    return list(files)


def print_only_file_names(files):
    names = list(map(lambda x: os.path.splitext(os.path.basename(x))[0], files))
    print(sorted(names))


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


def thread_pool():
    cpu_count = os.cpu_count()
    if cpu_count is None:
        cpu_count = 1
    return ThreadPool(cpu_count)


def print_slowly(*lines):
    for line in lines:
        if len(lines) > 1:
            print(line.rstrip('\n'))
        else:
            print(line)
        time.sleep(0.05)


def is_float(str_num):
    float_pattern = r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?'
    result = re.fullmatch(float_pattern, str_num)
    return result is not None
