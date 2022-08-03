"""
The MIT License (MIT)

Copyright (c) 2021-2022 c2eo team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
import csv
import json
import glob
import time
import re as regex
from multiprocessing.dummy import Pool as ThreadPool

# Reset
Color_Off = '\033[0m'

# Bold
BBlack = '\033[1;30m'
BRed = '\033[1;31m'
BGreen = '\033[1;32m'
BYellow = '\033[1;33m'
BBlue = '\033[1;34m'
BPurple = '\033[1;35m'
BCyan = '\033[1;36m'
BWhite = '\033[1;37m'

# High Intensity
IBlack = '\033[0;90m'
IRed = '\033[0;91m'
IGreen = '\033[0;92m'
IYellow = '\033[0;93m'
IBlue = '\033[0;94m'
IPurple = '\033[0;95m'
ICyan = '\033[0;96m'
IWhite = '\033[0;97m'

# Bold High Intensity
BIWhite = '\033[1;97m'

INFO = 'INFO'
WARNING = 'WARNING'
ERROR = 'ERROR'
EXCEPTION = 'EXCEPTION'
PASS = 'PASS'
NOTE = 'NOTE'
SKIP = 'SKIP'

statuses = {INFO: f'{BBlue}{INFO}{IWhite}', WARNING: f'{BPurple}{WARNING}{IWhite}', ERROR: f'{BRed}{ERROR}{IWhite}',
            EXCEPTION: f'{BRed}{EXCEPTION}{IWhite}', PASS: f'{BGreen}{PASS}{IWhite}', NOTE: f'{BYellow}{NOTE}{IWhite}',
            SKIP: f'{BCyan}{SKIP}{IWhite}'}

separation_line = f'{BIWhite}{"-" * 108}{IWhite}'


def apply_filters_to_files(files, filters=None, print_files=False):
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
    result = list(result)
    pprint(f'{len(result)} files left')
    if print_files:
        pprint_only_file_names(result)
    return result


def clear_dir_by_patterns(path, file_patterns, recursive=False, print_files=False):
    found_files = search_files_by_patterns(path, file_patterns, recursive=recursive, print_files=print_files)
    for file in found_files:
        os.remove(file)
    pprint('Files removed')


def compare_files(file1, file2):
    if not (os.path.isfile(file1) and os.path.isfile(file2)):
        return False
    with open(file1, 'r', encoding='ISO-8859-1') as f1:
        data1 = f1.read()
    with open(file2, 'r', encoding='ISO-8859-1') as f2:
        data2 = f2.read()
    return data1 == data2


def cpu_count():
    count = os.cpu_count()
    if count is None:
        return 1
    return count


def get_or_none(array, index):
    return array[index] if index < len(array) else None


def get_status(status):
    return statuses[status]


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
    names = path.split()
    return '.'.join(names)


def move_to_script_dir(path_to_script):
    path_to_script = os.path.dirname(path_to_script)
    if os.path.exists(path_to_script):
        os.chdir(path_to_script)


def pprint(*data, slowly=False, status=INFO, end='\n', on_the_next_line=False):
    if on_the_next_line:
        print()
    if not data:
        data = ['']
    for token in data:
        if type(token) == list:
            token = ''.join(list(map(str, token)))
        for line in str(token).split('\n'):
            if status:
                print(f'{IWhite}[{get_status(status)}] {line}', end=end)
            else:
                print(f'{IWhite}{line}', end=end)

            if slowly:
                time.sleep(0.01)


def pprint_header(header):
    pprint_separation_line()
    pprint(header, slowly=True)
    pprint_separation_line()


def pprint_status_result(name, status, log_data, max_lines=None):
    pprint(name, slowly=True, status=status)
    if max_lines:
        pprint_truncated_data(log_data, max_lines)
    else:
        pprint(log_data, slowly=True, status='')


def pprint_only_file_names(files):
    names = list(map(lambda x: get_file_name(x), files))
    pprint(', '.join(sorted(names, key=str.casefold)))
    pprint()


def pprint_result(header, total_tests, total_time, result, is_failed):
    pprint_header(f'{header} RESULTS')
    summary = [f'Total tests: {total_tests}']
    for status in result:
        if status == PASS:
            if result[status]:
                pprint_status_result(', '.join(sorted(result[status], key=str.casefold)), status, '')
            summary.append(f'Passed: {len(result[status])}')
        elif status in [NOTE, WARNING, EXCEPTION, SKIP] or (status == ERROR and type(result[status]) == dict):
            count = 0
            for message, files in sorted(result[status].items(), key=lambda x: x[0].casefold()):
                file_places = []
                for file, places in sorted(files.items(), key=lambda x: x[0].casefold()):
                    if len(places):
                        count += len(places)
                        file_places.append(f'{file}: [{", ".join(sorted(places))}]')
                    else:
                        file_places.append(f'{file}')
                        count += 1
                file_places = ', '.join(file_places)
                if status == EXCEPTION and message.count('\n') > 2:
                    pprint_status_result(file_places, status, message.rstrip(), max_lines=10)
                else:
                    pprint_status_result(' '.join(message.rstrip().split('\n')), status, file_places)
                print()
            summary.append(f'{str(status).capitalize()}s: {count}')
        elif status == ERROR:
            for test_name, log_data in sorted(result[status], key=lambda x: x[0].casefold()):
                pprint_status_result(test_name, ERROR, log_data)
                print()
            summary.append(f'{str(status).capitalize()}s: {len(result[status])}')
    pprint()
    pprint_separation_line()
    pprint(f'{BRed}{header} FAILED{IWhite}') if is_failed else pprint(f'{BGreen}{header} SUCCESS{IWhite}')
    summary = ', '.join(summary)
    time_header = f'Total time: {total_time // 60:02}:{ total_time % 60:02} min'
    pprint_header(f'{summary}\n{time_header}')


def pprint_separation_line():
    pprint(separation_line, slowly=True)


def pprint_truncated_data(data, max_lines):
    if type(data) == str:
        data = data.split('\n')
        data = data[:max_lines]
        data = '\n'.join(data)
    else:
        data = data[:max_lines]
    pprint(data, slowly=True, status='')


def print_progress_bar(i, n):
    cell_count = 20
    cell_size = n / cell_count
    filled_cell_count = int(i / (float(n) / cell_count)) if n > 0 else cell_count
    indicator = ' ▏▎▍▌▋▊▉█'
    last = len(indicator) - 1
    current_cell = ''
    if cell_count != filled_cell_count:
        current_cell = indicator[int(i % cell_size / cell_size * last)]
    bar = f'{indicator[last] * filled_cell_count}{current_cell}{" " * (cell_count - filled_cell_count - 1)}'
    percentage = f'{round(i / n * 100.0, 2) if n > 0 else 100.0:5.2f}%'
    print(f'\r[{get_status(INFO)}] {percentage}|{bar}| {i}/{n}', end='')


def read_file_as_dictionary(path):
    _, _, extension = split_path(path)
    data = []
    if '.csv' == extension:
        with open(path, newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                data.append(row)
    elif '.json' == extension:
        with open(path) as f:
            data = json.load(f)
    else:
        pprint('unsupported file extension', status=EXCEPTION)
    return data


def remove_empty_dirs(path):
    is_removed = True
    while is_removed:
        is_removed = False
        folders = list(os.walk(path))[1:]
        for folder in folders:
            if not folder[1] and not folder[2]:
                os.rmdir(folder[0])
                is_removed = True


def search_files_by_patterns(path, file_patterns, filters=None, recursive=False, print_files=False):
    if recursive:
        path = os.path.join(path, '**')
    pprint(f'\nLooking for "{file_patterns}" files in "{path}"')
    found_files = []
    for pattern in file_patterns:
        found_files.extend(glob.glob(os.path.join(path, pattern), recursive=recursive))
    pprint(f'Found {len(found_files)} files')
    if print_files:
        pprint_only_file_names(found_files)
    found_files = apply_filters_to_files(found_files, filters, print_files=print_files)
    return found_files


def split_path(path_to_file, with_end_sep=False):
    path, file = os.path.split(path_to_file)
    if with_end_sep:
        path += os.sep
    file_name, extension = os.path.splitext(file)
    return path, file_name, extension


def thread_pool():
    return ThreadPool(cpu_count())


def version_compare(ver1, ver2):
    for v1, v2 in zip(ver1.split('.'), ver2.split('.')):
        if int(v1) > int(v2):
            return 1
        elif int(v1) < int(v2):
            return -1
    return 0
