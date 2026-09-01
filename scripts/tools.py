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
import copy
import math
from os import chdir
from pathlib import Path
from os import sep as os_sep
from os import cpu_count as os_cpu_count
from time import localtime, strftime, sleep
from multiprocessing.dummy import Pool as ThreadPool

ISO_8859_1 = 'ISO-8859-1'

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
TIME = 'TIME'

statuses = {INFO: f'{BBlue}{INFO}{IWhite}', WARNING: f'{BPurple}{WARNING}{IWhite}', ERROR: f'{BRed}{ERROR}{IWhite}',
            EXCEPTION: f'{BRed}{EXCEPTION}{IWhite}', PASS: f'{BGreen}{PASS}{IWhite}', NOTE: f'{BYellow}{NOTE}{IWhite}',
            SKIP: f'{BCyan}{SKIP}{IWhite}', TIME: f'{BIWhite}{TIME}{IWhite}'}

separation_line = f'{BIWhite}{"-" * 108}{IWhite}'


def apply_filters_to_files(files: set[Path], filters: set[str] = None, print_files: bool = False) -> set[Path]:
    if filters is None:
        return files

    pprint(f'Apply filters: {filters} to found files')
    if inclusion_filters := [f for f in filters if f[0] != '!']:
        result = set()
        for _filter in inclusion_filters:
            result |= set(x for x in files if _filter in str(x))
    else:
        result = copy.copy(files)
    for exclusion_filter in [x[1:] for x in filters if x[0] == '!']:
        result = {file for file in result if exclusion_filter not in str(file)}
    pprint(f'{len(result)} files left')
    if print_files:
        pprint_only_file_names(result)
    return result


def clear_dir_by_patterns(path: Path, file_patterns: set[str], recursive: bool = False,
                          print_files: bool = False) -> None:
    found_files = search_files_by_patterns(path, file_patterns, recursive=recursive, print_files=print_files)
    for file in found_files:
        file.unlink()
    pprint('Files removed')


def compare_files_content(file1: Path, file2: Path) -> bool:
    return file1.exists() and file2.exists() and file1.read_bytes() == file2.read_bytes()


def cpu_count() -> int:
    return os_cpu_count() or 1


def get_status(status: str) -> str:
    return statuses[status]


def is_equal_float_strs(str_num1: str, str_num2: str) -> bool:
    str_num1, str_num2 = str_num1.replace(',', '.'), str_num2.replace(',', '.')
    try:
        return math.isclose(float(str_num1), float(str_num2), rel_tol=1e-08, abs_tol=1e-4)
    except ValueError:
        return False


def is_equal_hex_strs(str_num1: str, str_num2: str) -> bool:
    try:
        if len(str_num1) == len(str_num2):
            return int(str_num1, 16) == int(str_num2, 16)
        else:
            str_num1, str_num2 = (str_num1, str_num2) if len(str_num1) >= len(str_num2) else (str_num2, str_num1)
            value_index = len(str_num1) - len(str_num2)
            prefix, value = set(str_num1[:value_index].lower()), str_num1[value_index:]
            return prefix == {'f'} and value == str_num2
    except ValueError:
        return False


def make_name_from_path(path: Path) -> str:
    name = str(path).lstrip(os_sep).replace(os_sep, ' ').replace('.', '')
    return '.'.join(name.split())


def move_to_script_dir(path_to_script: Path) -> None:
    if path_to_script.parent.exists():
        chdir(path_to_script.parent)


def pprint(*data: str | list, slowly: bool = False, status: str = INFO, end: str = '\n',
           on_the_next_line: bool = False) -> None:
    if on_the_next_line:
        print()
    for token in data or ['']:
        if type(token) == list:
            token = ''.join(map(str, token))
        for line in str(token).splitlines():
            status_str = f'[{get_status(status)}] ' if status else ''
            print(f'{IWhite}{status_str}{line}', end=end)
            if slowly:
                sleep(0.01)


def pprint_header(header: str) -> None:
    pprint_separation_line()
    pprint(header, slowly=True)
    pprint_separation_line()


def pprint_status_result(name: str, status: str, log_data: list[str] | str, max_lines: int = None) -> None:
    pprint(name, slowly=True, status=status)
    if max_lines:
        pprint_truncated_data(log_data, max_lines)
    else:
        pprint(log_data, slowly=True, status='')


def pprint_only_file_names(files: set[Path]) -> None:
    pprint(', '.join(sorted({x.name for x in files}, key=str.casefold)))
    pprint()


def pprint_result(header: str, total_tests: int, total_seconds: int,
                  result: dict[str, set[str | str, str] | dict[str, dict[str, set[str]]]], is_failed: bool) -> None:
    pprint_header(f'{header} RESULTS')
    summary = [f'Total tests: {total_tests}']
    for status in result:
        if status == PASS:
            if result[status]:
                pprint_status_result(', '.join(sorted(result[status], key=str.casefold)), status=status, log_data='')
            summary.append(f'Passed: {len(result[status])}')
            print()
        elif status in [NOTE, WARNING, EXCEPTION, SKIP] or (status == ERROR and type(result[status]) == dict):
            count = 0
            for message, files in sorted(result[status].items(), key=lambda x: x[0].casefold()):
                file_places = set()
                for file, places in sorted(files.items(), key=lambda x: x[0].casefold()):
                    if len(places):
                        count += len(places)
                        file_places.add(f'{file}: [{", ".join(sorted(places))}]')
                    else:
                        file_places.add(f'{file}')
                        count += 1
                file_places = ', '.join(file_places)
                if status == EXCEPTION and message.count('\n') > 2:
                    pprint_status_result(file_places, status, message.rstrip(), max_lines=10)
                else:
                    pprint_status_result(' '.join(message.rstrip().splitlines()), status, file_places)
                print()
            summary.append(f'{str(status).capitalize()}s: {count}')
        elif status == ERROR:
            for test_name, log_data in sorted(result[status], key=lambda x: x[0].casefold()):
                pprint_status_result(test_name, ERROR, log_data)
                print()
            summary.append(f'{str(status).capitalize()}s: {len(result[status])}')
        elif status == TIME:
            pprint_time_result(result[status])

    pprint()
    pprint_separation_line()
    pprint(f'{BRed}{header} FAILED{IWhite}' if is_failed else f'{BGreen}{header} SUCCESS{IWhite}')
    total_time = f'Total time: {total_seconds // 60:02}:{total_seconds % 60:02} min'
    finished_at = f'Finished at: {strftime("%a, %d %b %Y %H:%M:%S", localtime())}'
    pprint_header(f'{", ".join(summary)}\n{total_time}\n{finished_at}')


def pprint_time_result(transpilation_units):
    if 'eo_test_time' in transpilation_units[0]:
        data = [f'{u["unique_name"]}: {u["eo_test_time"]:.2f}s' for u in
                sorted(transpilation_units, key=lambda x: x["eo_test_time"])]
        pprint_status_result('Test time measurement for each test:', TIME, log_data=', '.join(data))
    else:
        data = [
            f'{u["unique_name"]}({u["transpilation_time"]:.3f}s {u["transpilation_file_size"]:.3f}kb {u["transpilation_speed"]:.3f}s/kb)'
            for u in sorted(transpilation_units, key=lambda x: x['transpilation_speed'])]
        pprint_status_result('Transpilation time measurement for each file:', TIME, log_data=', '.join(data))
    print()


def pprint_separation_line() -> None:
    pprint(separation_line, slowly=True)


def pprint_truncated_data(data: list[str] | str, max_lines: int) -> None:
    if type(data) == str:
        data = '\n'.join(data.splitlines()[:max_lines])
    else:
        data = data[:max_lines]
    pprint(data, slowly=True, status='')


def print_progress_bar(i: int, n: int) -> None:
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


def remove_empty_dirs(path: Path) -> None:
    [remove_empty_dirs(x) for x in path.iterdir() if x.is_dir()]
    if not any(path.iterdir()):
        path.rmdir()


def search_files_by_patterns(path: Path, file_patterns: set[str], filters: set[str] = None, recursive: bool = False,
                             print_files: bool = False) -> set[Path]:
    pprint(f'\nLooking for "{file_patterns}" files in "{path}"')
    found_files = set()
    for pattern in file_patterns:
        found_files |= set(path.rglob(pattern) if recursive else path.glob(pattern))
    pprint(f'Found {len(found_files)} files')
    if print_files:
        pprint_only_file_names(found_files)
    found_files = apply_filters_to_files(found_files, filters, print_files=print_files)
    return found_files


def thread_pool():
    return ThreadPool(cpu_count())


def version_compare(ver1: str, ver2: str) -> int:
    for v1, v2 in zip(ver1.split('.'), ver2.split('.')):
        if diff := int(v1) - int(v2):
            return diff
    return 0
