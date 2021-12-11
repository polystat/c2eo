# Useful functions for our project

import os
import glob
import time
import re as regex
from multiprocessing.dummy import Pool as ThreadPool


def clear_dir_by_pattern(path, file_pattern, recursive=False, print_files=False):
    found_files = search_files_by_pattern(path, file_pattern, recursive=recursive, print_files=print_files)
    for file in found_files:
        os.remove(file)
    pprint('Files removed')


def colorize_text(text, color):
    colors = {'blue': '\033[36m', 'white': '\033[37m',
              'red': '\033[31m', 'green': '\033[32m',
              'yellow': '\033[33m', 'purple': '\033[35m'}
    return f'{colors[color]}{text}{colors["white"]}'


def compare_files(file1, file2):
    if not os.path.isfile(file1) or not os.path.isfile(file2):
        return False
    with open(file1, 'r') as f1:
        data1 = f1.read()
    with open(file2, 'r') as f2:
        data2 = f2.read()
    return data1 == data2


def filter_files(files, filters):
    if len(filters) == 0:
        return files

    files = set(files)
    pprint(f'Apply filter: {filters} to found files')
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

    pprint(f'{len(files)} files left\n')
    return list(files)


def get_status(s):
    status = {'INFO': '\033[36mINFO\033[37m', 'WARNING': '\033[35mWARNING\033[37m',
              'ERROR': '\033[31mERROR\033[37m', 'EXCEPTION': '\033[31mEXCEPTION\033[37m',
              'OK': '\033[32mOK\033[37m'}
    return status.get(s)


def get_file_name(path):
    file = os.path.basename(path)
    name = os.path.splitext(file)[0]
    return name


def is_float(str_num):
    float_pattern = r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?'
    result = regex.fullmatch(float_pattern, str_num)
    return result is not None


def make_name_from_path(path):
    path = path.replace(os.sep, ' ').replace('.', '')
    names = filter(lambda x: x != '', path.split(' '))
    return '_'.join(names)


def pprint(*lines, slowly=False, status='INFO'):
    if not lines:
        lines = ['']
    for line in lines:
        for token in line.split('\n'):
            print(f'{get_status(status)} {token}')
            if slowly:
                time.sleep(0.01)


def print_only_file_names(files):
    names = list(map(lambda x: os.path.splitext(os.path.basename(x))[0], files))
    pprint(sorted(names))


def search_files_by_pattern(path, file_pattern, filters=None, recursive=False, print_files=False):
    if filters is None:
        filters = []
    if recursive:
        path = os.path.join(path, '**')
    pprint(f'\nLooking for "{file_pattern}" files in "{path}"')
    found_files = glob.glob(f'{os.path.join(path, file_pattern)}', recursive=recursive)
    pprint(f'Found {len(found_files)} files')
    found_files = filter_files(found_files, filters)
    # Keep only file basename
    if print_files:
        pprint()
        print_only_file_names(found_files)
        pprint()
    return found_files


def split_path(path_to_file, with_end_sep=False):
    path, file = os.path.split(path_to_file)
    if with_end_sep:
        path += os.sep
    file_name, extension = os.path.splitext(file)
    return path, file_name, extension


def thread_pool():
    cpu_count = os.cpu_count()
    if cpu_count is None:
        cpu_count = 1
    return ThreadPool(cpu_count)


def version_compare(ver1, ver2):
    for v1, v2 in zip(ver1.split('.'), ver2.split('.')):
        if int(v1) > int(v2):
            return 1
        elif int(v1) < int(v2):
            return -1
    return 0
