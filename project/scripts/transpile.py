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
import re as regex
from os import chdir
from pathlib import Path
from shutil import copyfile
from os import sep as os_sep
from subprocess import CompletedProcess

# Our scripts
import tools
import settings
import build_c2eo
import clean_before_transpilation


class Transpiler(object):

    def __init__(self, path_to_c_files: Path, skips_file_name: str, need_to_prepare_c_code: bool = True,
                 need_to_generate_codecov: bool = False):
        self.filters = None
        if path_to_c_files.is_file():
            self.filters = {path_to_c_files.name}
            path_to_c_files = path_to_c_files.parent
        self.skips = settings.get_skips(skips_file_name) if skips_file_name else {}
        self.need_to_generate_codecov = need_to_generate_codecov
        self.codecov_arg = ''
        if self.need_to_generate_codecov:
            self.codecov_arg = f'LLVM_PROFILE_FILE="{path_to_c_files.stem}_%p.profraw"'
        self.need_to_prepare_c_code = need_to_prepare_c_code
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.path_to_c2eo_transpiler = settings.get_setting('path_to_c2eo_transpiler')
        self.path_to_c_files = path_to_c_files.resolve()
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.path_to_eo_src = settings.get_setting('path_to_eo_src').resolve()
        self.path_to_eo_external = settings.get_setting('path_to_eo_external').resolve()
        self.path_to_eo_coperators = settings.get_setting('path_to_eo_coperators')
        self.path_to_eo_lib_coperators = settings.get_setting('path_to_eo_lib_coperators')
        self.path_to_eo_lib_pom = settings.get_setting('path_to_eo_lib_pom')
        self.plug_code = settings.get_meta_code('plug')
        self.result_dir_name = settings.get_setting('result_dir_name')
        self.run_sh_code = settings.get_meta_code('run.sh')
        self.run_sh_replace = settings.get_setting('run_sh_replace')
        self.transpilation_units: list[dict[str, str | Path | CompletedProcess | float]] = []
        self.replaced_path = self.path_to_c_files.parent
        self.files_handled_count = 0
        self.ignored_transpilation_warnings = settings.get_setting('ignored_transpilation_warnings') or []

    def transpile(self) -> (list[dict[str, str | Path | CompletedProcess | float]], dict[str, dict[str, set[str]]]):
        start_time = time.time()
        self.build_c2eo()
        tools.pprint('\n', 'Transpilation start', '\n', slowly=True)
        clean_before_transpilation.main(self.path_to_c_files, '*.alias  *-eo.c')
        c_files = tools.search_files_by_patterns(self.path_to_c_files, {'*.c'}, filters=self.filters, recursive=True,
                                                 print_files=True)
        with tools.thread_pool() as threads:
            self.transpilation_units = list(threads.imap_unordered(self.make_unit, c_files))
        generate_unique_names_for_units(self.transpilation_units)
        skip_result = self.check_skips()
        original_path = Path.cwd()
        chdir(self.path_to_c2eo_transpiler)
        tools.pprint('\n', 'Transpile files:', '\n', slowly=True)
        tools.print_progress_bar(0, len(self.transpilation_units))
        with tools.thread_pool() as threads:
            list(threads.imap_unordered(self.start_transpilation, self.transpilation_units))
        tools.print_progress_bar(self.files_handled_count, len(self.transpilation_units))
        result = self.group_transpilation_results()
        result[tools.SKIP] = skip_result
        tests_count = len(self.transpilation_units) + sum(map(len, skip_result.values()))
        is_failed = sum(map(len, result[tools.EXCEPTION].values())) > 0
        tools.pprint_result('TRANSPILE', tests_count, int(time.time() - start_time), result, is_failed)
        if is_failed:
            exit('transpilation failed')
        self.prepare_eo_project()
        self.remove_unused_eo_src_files()
        self.move_transpiled_files()
        self.move_aliases()
        tools.pprint('\n', 'Transpilation done', '\n')
        chdir(original_path)
        return self.transpilation_units, skip_result

    def build_c2eo(self) -> None:
        cmd = 'cmake '
        cmake_cxx_flags = ['-fsanitize=address']
        if self.need_to_generate_codecov:
            cmd += '-D CMAKE_CXX_COMPILER="/bin/clang++" '
            cmake_cxx_flags.extend(['-fprofile-instr-generate', '-fcoverage-mapping'])
        cmd += f'-D CMAKE_CXX_FLAGS="$CMAKE_CXX_FLAGS {" ".join(cmake_cxx_flags)}" ..'
        build_c2eo.main(self.path_to_c2eo_build, cmd)

    def make_unit(self, c_file: Path) -> dict[str, str | Path]:
        rel_c_path = Path(str(c_file.parent).replace(str(self.replaced_path), '').lstrip(os_sep))
        full_name = f'{tools.make_name_from_path(rel_c_path)}.{c_file.stem}'
        prepared_c_file, result_path = self.prepare_c_file(c_file)
        return {'c_file': c_file, 'rel_c_path': rel_c_path, 'full_name': full_name, 'prepared_c_file': prepared_c_file,
                'result_path': result_path, 'name': c_file.stem, 'unique_name': c_file.stem,
                'prepared_c_i_file': prepared_c_file.with_suffix('.c.i')}

    def check_skips(self) -> dict[str, dict[str, set[str]]]:
        tools.pprint('\n', 'Check skips', slowly=True)
        skip_units = []
        skips = {}
        for unit in self.transpilation_units:
            for _filter, comment in self.skips.items():
                if _filter in unit['name']:
                    if comment not in skips:
                        skips[comment] = {}
                    skips[comment][unit['unique_name']] = set()
                    skip_units.append(unit)
                    break
        self.transpilation_units = list(filter(lambda x: x not in skip_units, self.transpilation_units))
        return skips

    def start_transpilation(self, unit: dict[str, str | Path | CompletedProcess | float]) -> None:
        eo_file = Path(f'{unit["full_name"]}.eo')
        transpile_cmd = f'{self.codecov_arg} /usr/bin/time -f "%e" ./c2eo {unit["prepared_c_file"]} {eo_file}'
        result = subprocess.run(transpile_cmd, shell=True, capture_output=True, text=True)
        result.stdout, result.stderr = result.stdout.splitlines(), result.stderr.splitlines()
        unit['transpilation_time'] = float(result.stderr[-1])
        unit['transpilation_file_size'] = unit['prepared_c_file'].with_suffix(
            '.c.i').stat().st_size / 1024.0  # bytes -> kbytes
        unit['transpilation_speed'] = unit['transpilation_time'] / unit['transpilation_file_size']
        unit['transpilation_result'] = result
        unit['eo_file'] = eo_file.resolve()
        unit['rel_eo_file'] = unit['rel_c_path'] / f'{unit["name"]}.eo'
        if not result.returncode:
            add_return_code_to_eo_file(eo_file)
        self.files_handled_count += 1
        tools.print_progress_bar(self.files_handled_count, len(self.transpilation_units))

    def prepare_c_file(self, c_file: Path) -> (Path, Path):
        with open(c_file, 'r', encoding=tools.ISO_8859_1) as f:
            data = f.readlines()
        if self.need_to_prepare_c_code:
            prepare_c_code(data)
        result_path = c_file.parent / self.result_dir_name
        prepared_c_file = c_file.parent / f'{c_file.stem}-eo.c'
        result_path.mkdir(exist_ok=True, parents=True)
        with open(prepared_c_file, 'w') as f:
            f.writelines(data)
        return prepared_c_file, result_path

    def remove_unused_eo_src_files(self) -> None:
        src_eo_names = tools.search_files_by_patterns(self.path_to_eo_src, {'*.eo'}, recursive=True)
        src_eo_names = {Path(str(x).replace(str(self.path_to_eo_src), '').lstrip(os_sep)) for x in src_eo_names}
        transpiled_eo_names = {x['rel_eo_file'] for x in self.transpilation_units}
        for name in src_eo_names - transpiled_eo_names:
            (self.path_to_eo_src / name).unlink()
        for unit in self.transpilation_units:
            unit['src_eo_path'] = self.path_to_eo_src / unit['rel_c_path']
            unit['src_eo_file'] = unit['src_eo_path'] / f'{unit["name"]}.eo'
        tools.remove_empty_dirs(self.path_to_eo_src)

    def create_plug_file(self, unit: dict[str, str | Path | CompletedProcess | float], message: str) -> None:
        plug = regex.sub('<file_name>', unit['full_name'], self.plug_code)
        plug = regex.sub('<exception_message>', message, plug)
        unit['eo_file'].write_text(plug)

    def group_transpilation_results(self) -> dict[
        str, set[str] | dict[str, dict[str, set[str]]] | list[dict[str, str | Path | CompletedProcess | float]]]:
        result = {tools.PASS: set({unit['unique_name'] for unit in self.transpilation_units}), tools.NOTE: {},
                  tools.WARNING: {}, tools.ERROR: {}, tools.EXCEPTION: {}, tools.SKIP: {},
                  tools.TIME: self.transpilation_units}
        tools.pprint('\n', 'Getting results', '\n', slowly=True, on_the_next_line=True)
        for unit in self.transpilation_units:
            exception_message = check_unit_exception(unit)
            if exception_message:
                if exception_message not in result[tools.EXCEPTION]:
                    result[tools.EXCEPTION][exception_message] = {}
                if unit['unique_name'] not in result[tools.EXCEPTION][exception_message]:
                    result[tools.EXCEPTION][exception_message][unit['unique_name']] = set()

            for line in unit['transpilation_result'].stderr:
                line = line.lower()
                if any(warning in line for warning in self.ignored_transpilation_warnings):
                    continue

                for status in [tools.NOTE, tools.WARNING, tools.ERROR]:
                    if f'{status.lower()}:' in line:
                        place, _, message = line.partition(f'{status.lower()}:')
                        message = message.strip()
                        if message not in result[status]:
                            result[status][message] = {}
                        if unit['unique_name'] not in result[status][message]:
                            result[status][message][unit['unique_name']] = set()
                        if unit['unique_name'] in place:
                            result[status][message][unit['unique_name']].add(place.split(':', 1)[1][:-2])

        for status in [tools.EXCEPTION]:
            result[tools.PASS] -= set(file for value in result[status].values() for file in value.keys())
        return result

    def prepare_eo_project(self):
        self.path_to_eo_project.mkdir(parents=True, exist_ok=True)
        self.path_to_eo_src.mkdir(parents=True, exist_ok=True)
        self.path_to_eo_coperators.mkdir(parents=True, exist_ok=True)
        copyfile(self.path_to_eo_lib_pom, self.path_to_eo_project / self.path_to_eo_lib_pom.name)
        lib_coperators = tools.search_files_by_patterns(self.path_to_eo_lib_coperators, {'*.eo'})
        project_coperators = tools.search_files_by_patterns(self.path_to_eo_coperators, {'*.eo'})
        for eo_lib_file in lib_coperators:
            eo_project_file = self.path_to_eo_coperators / eo_lib_file.name
            if not tools.compare_files_content(eo_lib_file, eo_project_file):
                copyfile(eo_lib_file, eo_project_file)
        lib_coperators = set(map(lambda x: x.name, lib_coperators))
        project_coperators = set(map(lambda x: x.name, project_coperators))
        for file in project_coperators - lib_coperators:
            file.unlink()

    def move_transpiled_files(self) -> None:
        difference = []
        for unit in self.transpilation_units:
            eo_file = unit['result_path'] / f'{unit["name"]}.eo'
            unit['prepared_c_file'].replace(unit['result_path'] / unit['prepared_c_file'].name)
            unit['prepared_c_i_file'].replace(unit['result_path'] / unit['prepared_c_i_file'].name)
            if not tools.compare_files_content(unit['eo_file'], eo_file):
                copyfile(unit['eo_file'], eo_file)
            if not tools.compare_files_content(unit['eo_file'], unit['src_eo_file']):
                unit['src_eo_path'].mkdir(parents=True, exist_ok=True)
                unit['eo_file'].replace(unit['src_eo_file'])
                difference.append(unit['eo_file'])
            unit['eo_file'].unlink(missing_ok=True)
            unit['eo_file'] = eo_file
        difference = set(filter(lambda x: x, difference))  # Filter None values
        if difference:
            tools.pprint('\n', 'Detected changes in src files:')
            tools.pprint_only_file_names(difference)
            tools.pprint('Move these files to src dir', '\n')
        else:
            tools.pprint('\n', 'Not found any changes in src files')

    def move_aliases(self) -> None:
        aliases = tools.search_files_by_patterns(Path(), {'*.alias'}, print_files=True)
        self.path_to_eo_external.mkdir(exist_ok=True, parents=True)
        tools.clear_dir_by_patterns(self.path_to_eo_external, {'*.eo'})
        for alias in aliases:
            alias.replace(self.path_to_eo_external / alias.stem)


def generate_unique_names_for_units(units: list[dict[str, str | CompletedProcess]], words_in_name: int = 2) -> None:
    names = {}
    collision_names = {}
    for unit in units:
        if unit['unique_name'] in names:
            if unit['unique_name'] not in collision_names:
                collision_names[unit['unique_name']] = names[unit['unique_name']]
        else:
            names[unit['unique_name']] = []
        names[unit['unique_name']].append(unit)
    units = []
    for name, _units in collision_names.items():
        units.extend(_units)
        for unit in _units:
            unit['unique_name'] = str(Path(*unit['rel_c_path'].parts[-words_in_name:]) / unit['name'])
    if len(collision_names) > 0:
        generate_unique_names_for_units(units, words_in_name + 1)


def add_return_code_to_eo_file(eo_file: Path) -> None:
    with open(f'{eo_file}', 'r', encoding=tools.ISO_8859_1) as f:
        data = f.readlines()
    is_main = False
    aliases = {'+alias c2eo.coperators.as-uint8\n'}
    aliases_count = 0
    for i, line in enumerate(data):
        if line.startswith('+alias'):
            aliases.add(line)
            aliases_count += 1
        elif line == '  [param-start param-size] > main\n':
            is_main = True
        elif is_main and line == '          TRUE\n' and data[i - 1] != '          goto-return-label.forward TRUE\n':
            data[i] = '          write-as-int32 return 0\n          goto-return-label.forward TRUE\n'
            aliases.add('+alias c2eo.coperators.write-as-int32\n')
            break

    if data[-1].strip() == 'TRUE':
        data[-1] = '    printf "%d" (as-uint8 (read-as-int32 return))\n'
        aliases |= {'+alias c2eo.coperators.printf\n', '+alias c2eo.coperators.read-as-int32\n'}
    with open(f'{eo_file}', 'w', encoding=tools.ISO_8859_1) as f:
        f.writelines(sorted(aliases))
        f.writelines(data[aliases_count:])


def check_unit_exception(unit: dict[str, str | Path | CompletedProcess | float]) -> str:
    exception_message = ''
    if unit['transpilation_result'].returncode:
        exception_message = '\n'.join(unit['transpilation_result'].stderr[-3:-1])  # exception lines
    elif not unit['eo_file'].exists():
        exception_message = 'was generated empty EO file'
    elif unit['eo_file'].stat().st_size == 0:
        exception_message = 'the EO file was not generated'
    if not unit['eo_file'].exists():
        open(unit['eo_file'], 'a').close()
    return exception_message


def prepare_c_code(data: list[str]) -> None:
    for i, line in enumerate(data):
        if '#include' in line:
            new_line = line.lstrip()
            whitespace_count = len(line) - len(new_line)
            indent = ' ' * whitespace_count
            data[i] = f'{indent}// {new_line}'


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for translating C files to the EO files',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('path_to_c_files', metavar='PATH',
                         help='the relative path from the scripts folder to the folder with c files')

    _parser.add_argument('-s', '--skips_file_name', metavar='FILE_NAME', default='',
                         help='the name of the file with a set of skips for transpile')

    _parser.add_argument('-n', '--not_prepare_c_code', action='store_const', const=True, default=False,
                         help='the script will not change the c code in the input files')

    _parser.add_argument('-c', '--codecov', action='store_const', const=True, default=False,
                         help='the script will generate codecov files')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    Transpiler(Path(namespace.path_to_c_files).resolve(), namespace.skips_file_name,
               not namespace.not_prepare_c_code, namespace.codecov).transpile()
