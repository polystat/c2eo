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
import argparse
import subprocess
import re as regex
from os import chdir
from shutil import copy
from pathlib import Path
from build_eo import EOBuilder

# Our scripts
import tools
import settings


def execute(path_to_eo_file: Path):
    if not path_to_eo_file.is_file():
        exit('Require eo file')
    path_to_eo_file.resolve()
    path_to_eo_src = settings.get_setting('path_to_eo_src')
    tools.clear_dir_by_patterns(path_to_eo_src, {'*.eo'}, recursive=True)
    tools.remove_empty_dirs(path_to_eo_src)
    tools.clear_dir_by_patterns(settings.get_setting('path_to_eo_external'), {'*.eo'})
    copy(path_to_eo_file, path_to_eo_src)
    EOBuilder([]).build()
    package_name = ''
    with open(path_to_eo_file, 'r') as f:
        for line in f:
            if '+package' in line:
                package_name = line.replace('+package', '').strip().replace('-', '_')
                break
    run_sh_cmd = settings.get_meta_code('run.sh', read_as_lines=True)[2].rstrip()
    run_sh_replace = f'c2eo.src.{settings.get_setting("run_sh_replace")}'
    cmd = regex.sub(run_sh_replace, package_name, run_sh_cmd).split()
    chdir(settings.get_setting('path_to_eo_project'))
    print(*cmd)
    subprocess.run(cmd)


def create_parser() -> argparse.ArgumentParser:
    _parser = argparse.ArgumentParser(description='the script for executing eo files',
                                      formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    _parser.add_argument('-p', '--path_to_eo_file', metavar='PATH',
                         help='the relative path from the scripts folder to the eo file')
    return _parser


if __name__ == '__main__':
    assert sys.version_info >= (3, 10)
    tools.move_to_script_dir(Path(sys.argv[0]))
    parser = create_parser()
    namespace = parser.parse_args()
    execute(Path(namespace.path_to_eo_file))
