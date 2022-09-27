#! /usr/bin/python3

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

import sys
import time
import argparse
import subprocess
from os import chdir
from pathlib import Path

# Our scripts
import settings
import tools


class ClangTidy(object):

    def __init__(self, path_to_code_files: Path):
        self.filters = None
        if path_to_code_files.is_file():
            self.filters = {path_to_code_files.name}
            path_to_code_files = path_to_code_files.parent
        self.path_to_code_files = path_to_code_files
        self.files_handled_count = 0
        self.files_count = 0
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_c2eo_build.mkdir(exist_ok=True)
        self.ignored_inspection_warnings = settings.get_setting('ignored_inspection_warnings') or []
        self.clang_tidy_checks = ','.join(settings.get_setting('clang_tidy_checks'))
        self.results: list[dict[str, str | Path | subprocess.CompletedProcess]] = []

    def inspect(self) -> bool:
        start_time = time.time()
        tools.pprint('\n', 'Inspection start', '\n')
        self.generate_compile_commands()
        patterns = set(settings.get_setting('code_file_patterns'))
        code_files = tools.search_files_by_patterns(self.path_to_code_files, patterns, filters=self.filters,
                                                    recursive=True, print_files=True)
        self.files_count = len(code_files)
        tools.pprint('\n', 'Inspect files:', '\n', slowly=True)
        tools.print_progress_bar(0, self.files_count)
        with tools.thread_pool() as threads:
            self.results = list(threads.imap_unordered(self.inspect_file, code_files))
        result = self.group_inspection_results()
        _is_failed = len(result[tools.WARNING]) + len(result[tools.EXCEPTION]) > 0
        tools.pprint_result('INSPECTION', self.files_count, int(time.time() - start_time), result, _is_failed)
        return _is_failed

    def generate_compile_commands(self) -> None:
        original_path = Path.cwd()
        chdir(self.path_to_c2eo_build)
        cmd = 'cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON'
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        chdir(original_path)
        if result.returncode != 0:
            tools.pprint_status_result(cmd, tools.EXCEPTION, result.stderr)
            exit('Failed during cmake execution')
        tools.pprint(result.stdout, slowly=True)

    def inspect_file(self, file: Path) -> dict[str, str | Path | subprocess.CompletedProcess]:
        transpile_cmd = f'clang-tidy -p {self.path_to_c2eo_build} --checks=\'{self.clang_tidy_checks}\' {file}'
        result = subprocess.run(transpile_cmd, shell=True, capture_output=True, text=True)
        self.files_handled_count += 1
        tools.print_progress_bar(self.files_handled_count, self.files_count)
        return {'name': file.stem, 'file': file.name, 'inspection_result': result}

    def group_inspection_results(self) -> dict[str, set[str] | dict[str, [dict[str, set[str]]]]]:
        result = {tools.PASS: {unit['file'] for unit in self.results}, tools.NOTE: {}, tools.WARNING: {},
                  tools.ERROR: {}, tools.EXCEPTION: {}}
        tools.pprint('\n', 'Getting results', '\n', slowly=True, on_the_next_line=True)
        for unit in self.results:
            if unit['inspection_result'].returncode:
                log_data = ''.join(unit['inspection_result'].stderr)
                if log_data not in result[tools.EXCEPTION]:
                    result[tools.EXCEPTION][log_data] = {}
                result[tools.EXCEPTION][log_data][unit['file']] = set()
                continue

            for line in unit['inspection_result'].stdout.splitlines():
                if any(warning in line for warning in self.ignored_inspection_warnings):
                    continue

                for status in [tools.NOTE, tools.WARNING, tools.ERROR]:
                    if f'{status.lower()}:' in line:
                        place, _, message = line.partition(f'{status.lower()}:')
                        message = message.strip()
                        if message not in result[status]:
                            result[status][message] = {}
                        if unit['file'] not in result[status][message]:
                            result[status][message][unit['file']] = set()
                        if unit['file'] in place:
                            result[status][message][unit['file']].add(place.split(':', 1)[1][:-2])
        for status in [tools.WARNING, tools.ERROR, tools.EXCEPTION]:
            result[tools.PASS] -= {file for value in result[status].values() for file in value.keys()}
        return result


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='script for checking code files using Clang-Tidy',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('-p', '--path_to_code_files', default=settings.get_setting('path_to_code_files'),
                         metavar='PATH', help='the relative path from the scripts folder to the code files folder')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    is_failed = ClangTidy(Path(namespace.path_to_code_files)).inspect()
    if is_failed:
        exit('clang-tidy checks failed')
