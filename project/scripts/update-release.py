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

import datetime
import os
import re
import subprocess
import sys

import argparse

import pgpy


def sign():
    key, _ = pgpy.PGPKey.from_file("../../private.pgp")
    with open("../../passpharase", 'r') as phrase:
        passpharase = phrase.read()
    passpharase = re.sub(r'\s', '', passpharase)
    with key.unlock(passpharase):
        with open("repository/dists/c2eo-rep/Release", 'r') as fin:
            with open("repository/dists/c2eo-rep/Release.gpg", 'w') as fout:
                print(key.sign(fin.read()), file=fout, end='')


libs_set = set()
control = {
    'c2eo': {
        'Package': 'c2eo',
        'Version': 'X.X.X',
        'Provides': 'c2eo',
        'Maintainer': 'Anonymous <anonymous@noreply.com>',
        'Architecture': 'all',
        'Section': 'misc',
        'Description': 'The translator of C to EOLANG.\n'
                       ' This is a translator of C/C++ to EOLANG for Debian and Ubuntu.',
        'Depends': 'llvm-libs(>=1)',
        'Origin': 'https://github.com/polystat/c2eo',
        'Multi-Arch': 'foreign'
    },
    'llvm-libs': {
        'Package': 'llvm-libs',
        'Version': '1.0.1',
        'Provides': 'llvm-libs',
        'Maintainer': 'Anonymous <anonymous@noreply.com>',
        'Architecture': 'all',
        'Section': 'misc',
        'Description': 'LLVM&CLANG libraries.\n'
                       ' A set of necessary precompiled libraries of the LLVM 12.0.1.',
        'Depends': 'gcc(>=11), clang(>=12)',
        'Origin': 'https://github.com/llvm/llvm-project/releases/',
        'Multi-Arch': 'foreign'
    }
}
distributions = {
    'Origin': 'Debian',
    'Suite': 'testing',
    'AlsoAcceptFor': 'unstable experimental',
    'Codename': 'c2eo-rep',
    'Version': '1.0',
    'Architectures': 'i386 amd64 source',
    'Components': 'main contrib non-free',
    'UDebComponents': 'main contrib non-free',
    'Description': 'repository for c2eo releases'
}


def get_user() -> str:
    return "Yaroslav Riabtsev <yairyabtsev@edu.hse.ru"


def try_shell(param, fatal=True, private=False):
    if not private:
        print(f'+ {param}')
    ret = subprocess.run(param, shell=True)
    if ret.returncode:
        if fatal:
            exit(1)
        else:
            return False
    return ret


def make_deb(date, deb_name):
    global libs_set, control
    if deb_name == 'c2eo':
        try_shell('cp ../../project/bin/c2eo usr/bin')
    else:
        for path in libs_set:
            try_shell(f'cp {path} usr/lib')
            try_shell(f'cp {path[:-3]} usr/lib')
    try_shell('cp ../../LICENSE.txt DEBIAN/copyright')
    with open('DEBIAN/control', 'w') as control_file:
        for key in control[deb_name].keys():
            print(f'{key}: {control[deb_name][key]}', file=control_file)
    with open('DEBIAN/changelog', 'w') as changelog:
        print(deb_name + ' ({}.{}-{}) unstable; urgency=medium\n'.format(*control[deb_name]['Version'].split('.')),
              file=changelog)
    if try_shell("git describe --tags --abbrev=0", fatal=False):
        last_commit = "git describe --tags --abbrev=0"
    else:
        last_commit = "git rev-list --max-parents=0 HEAD"
    try_shell(f'git log $({last_commit})..HEAD --merges --oneline --format="  * %h %s by %an <%aE>"'
              ' >> DEBIAN/changelog')
    with open('DEBIAN/changelog', 'a') as changelog:
        print(file=changelog)
        print(' --', control[deb_name]['Maintainer'], ' ', date, file=changelog)
    try_shell('md5deep -r usr > DEBIAN/md5sums')


def make_bin():
    try_shell('cmake ..')
    try_shell('make')


def make_repo():
    global distributions
    with open('conf/distributions', 'w') as distributions_file:
        for key in distributions.keys():
            print(f'{key}: {distributions[key]}', file=distributions_file)
    try_shell('reprepro export')
    try_shell('reprepro createsymlinks')
    try_shell('reprepro -P -V -b . -C main includedeb c2eo-rep ../llvm-libs*.deb')
    try_shell('reprepro -P -V -b . -C main includedeb c2eo-rep ../c2eo*.deb')


def copy_libs():
    global libs_set
    flag = True
    with open('../../release/todo.sh', 'w') as todo:
        with open('../../llvm-clang/libs.txt', 'r') as libs:
            for line in libs.readlines():
                lib_info = re.split(r'\s', line)
                lib = [e for e in lib_info if e]
                if len(lib) > 2:
                    path = os.path.abspath(lib[2])
                    if path[:6] == '/home/':
                        print(f'sudo cp {path} /usr/lib', file=todo)
                        print(f'sudo cp {path[:-3]} /usr/lib', file=todo)
                        flag = False
                    if path[-3:] == '.12':
                        libs_set.add(path)
    if not flag:
        print('run todo.sh as root and re-try')
        print('make sure that in CMakeLists.txt the correct path to the libraries is spelled out: '
              'link_directories("/usr/lib")')
        # exit()


def s3put():
    with open("../../config") as fin:
        config = fin.read()
    config = re.sub(r'\s', ' ', config)
    try_shell(f"s3cmd sync repository/dists repository/pool s3://c2eo.polystat.org/debian/ {config}",
              private=True)


if __name__ == '__main__':
    path_to_script = sys.argv[0]
    path_to_script = os.path.abspath(path_to_script)
    path_to_script = os.path.dirname(path_to_script)
    os.chdir(path_to_script)

    try_shell('rm -rf c2eo*')
    try_shell('rm -rf repo*')

    parser = argparse.ArgumentParser(description='Release maker')
    parser.add_argument(
        '--version',
        type=str,
        default='0.0.0',
        help='specify the new version'
    )
    args = parser.parse_args()

    date = datetime.datetime.now().astimezone().strftime('%a, %d %b %Y %H:%M:%S %z')
    user = get_user()

    control['c2eo']['Version'] = args.version
    control['c2eo']['Maintainer'] = user
    control['llvm-libs']['Maintainer'] = user

    os.chdir('../')
    os.makedirs('build', exist_ok=True)
    os.chdir('build')
    os.makedirs('../../release/', exist_ok=True)
    make_bin()

    try_shell('ldd ../bin/c2eo > ../../llvm-clang/libs.txt')
    copy_libs()

    os.chdir('../../release/')
    os.makedirs('repository/conf', exist_ok=True)

    debs = ['c2eo', 'llvm-libs']

    for deb in debs:
        os.makedirs(f'{deb}-{control[deb]["Version"]}/', exist_ok=True)
        os.chdir(f'./{deb}-{control[deb]["Version"]}/')
        os.makedirs('usr/bin', exist_ok=True)
        os.makedirs('usr/lib', exist_ok=True)
        os.makedirs('DEBIAN', exist_ok=True)
        make_deb(date, deb)
        os.chdir('..')
        try_shell(f'fakeroot dpkg-deb --build {deb}-{control[deb]["Version"]}')

    os.chdir('repository')
    make_repo()
    os.chdir('..')
    sign()

    s3put()
