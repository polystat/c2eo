# Useful functions for our project

import os
import csv
import json
import glob
import time
import re as regex
from multiprocessing.dummy import Pool as ThreadPool


def apply_filters_to_files(files, filters=None):
    if filters is None:
        return files
    pprint(f'Apply filters: {filters} to found files')
    inclusion_filters = set(filter(lambda f: f[0] != '!', filters))
    result = set() if inclusion_filters else set(files)
    for inclusion_filter in inclusion_filters:
        result |= set(filter(lambda file: inclusion_filter in file, files))
    exclusion_filters = set(filter(lambda f: f[0] == '!', filters))
    for exclusion_filter in exclusion_filters:
        result = set(filter(lambda file: exclusion_filter[1:] not in file, result))
    pprint(f'{len(result)} files left\n')
    return list(result)


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


def get_or_none(array, index):
    return array[index] if index < len(array) else None


def get_status(status):
    statuses = {'INFO': '\033[36mINFO\033[37m', 'WARN': '\033[35mWARN\033[37m',
                'ERROR': '\033[31mERROR\033[37m', 'EXCEPTION': '\033[31mEXCEPTION\033[37m',
                'PASS': '\033[32mPASS\033[37m'}
    return statuses.get(status)


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


def move_to_script_dir(path_to_script):
    path_to_script = os.path.dirname(path_to_script)
    if os.path.exists(path_to_script):
        os.chdir(path_to_script)


def pprint(*lines, slowly=False, status='INFO'):
    if not lines:
        lines = ['']
    for line in lines:
        for token in str(line).split('\n'):
            if status:
                print(f'[{get_status(status)}] {token}')
            else:
                print(token)

            if slowly:
                time.sleep(0.01)


def print_only_file_names(files):
    names = list(map(lambda x: get_file_name(x), files))
    pprint(sorted(names))
    pprint()


def read_file_as_dictionary(path):
    _, _, extension = split_path(path)
    data = []
    if '.csv' == extension:
        with open('names.csv', newline='') as csvfile:
            reader = csv.DictReader(csvfile)
        for row in reader:
            data.append(row)
    elif '.json' == extension:
        with open(path) as f:
            data = json.load(f)
    else:
        pprint('Unsupported file extension', status='EXCEPTION')
    return data


def search_files_by_pattern(path, file_pattern, filters=None, recursive=False, print_files=False):
    if recursive:
        path = os.path.join(path, '**')
    pprint(f'\nLooking for "{file_pattern}" files in "{path}"')
    found_files = glob.glob(f'{os.path.join(path, file_pattern)}', recursive=recursive)
    pprint(f'Found {len(found_files)} files')
    found_files = apply_filters_to_files(found_files, filters)
    if print_files:
        print_only_file_names(found_files)
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
