import datetime
import os
import re
import subprocess
import sys

import argparse
from git_config import get_config

libs_set = set()


def get_user() -> str:
    config = get_config()
    return "{} <{}>".format(config["user"]["name"], config["user"]["email"])


def inc(version: str) -> str:
    version = version.replace('-', '.')
    version = version.split('.')
    version[-1] = str(int(version[-1]) + 1)
    return '.'.join(version)


def get_version() -> str:
    #todo get it from file
    for file in os.listdir("."):
        if file[:5] == 'c2eo-' and file[-3:] != 'deb':
            return file[5:]
    return "?.?.1"


def try_shell(param):
    if subprocess.run(param, shell=True).returncode:
        exit(0)


def make_deb(version):
    for path in libs_set:
        try_shell(f'cp {path} usr/lib')
        try_shell(f'cp {path[:-3]} usr/lib')
    try_shell(f'cp ../../project/bin/c2eo usr/bin')
    with open('DEBIAN/control', 'r') as control_file:
        control = control_file.read()
        control = control.replace('{}.{}-{}'.format(*get_version().split('.')),
                                  '{}.{}-{}'.format(*version.split('.')))
    with open('DEBIAN/control', 'w') as control_file:
        print(control, file=control_file)
    with open(f'DEBIAN/changelog', 'w') as changelog:
        print('c2eo ({}.{}-{}) unstable; urgency=medium'.format(*version.split('.')), file=changelog)
        print(file=changelog)
        while True:
            changes = input('describe the new feature: ')
            if not changes:
                break
            print('  *', changes, file=changelog)
        print(file=changelog)
        print(' --', args.user, ' ', args.date, file=changelog)
    try_shell('md5deep -r usr > DEBIAN/md5sums')


def make_bin():
    try_shell('cmake ..')
    try_shell('make')


def make_repo(version):
    for directory in ['db', 'dists', 'pool']:
        try_shell(f'rm -rf {directory}')
    try_shell('reprepro export')
    try_shell(f'reprepro -P -V -b . -C main includedeb c2eo-rep ../../c2eo-{version}.deb')


def host_it():
    try_shell('az login')
    try_shell('az webapp up -n c2eo -g demo --location=eastus2 --sku=FREE')


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
        exit()


if __name__ == '__main__':
    path_to_script = sys.argv[0]
    path_to_script = os.path.abspath(path_to_script)
    path_to_script = os.path.dirname(path_to_script)
    os.chdir(path_to_script)

    parser = argparse.ArgumentParser(description='Release maker')
    parser.add_argument(
        '--branch',
        type=str,
        default='heap',
        help='name of the branch to check out (default is heap)'
    )
    parser.add_argument(
        '--version',
        type=str,
        default=inc(get_version()),
        help='specify the new version (by default, the last number increases)'
    )
    parser.add_argument(
        '--date',
        type=str,
        default=datetime.datetime.now().astimezone().strftime('%a, %d %b %Y %H:%M:%S %z'),
        help='use the date and time in the format %%a,  %%d%%b %%Y %%H:%%M:%%S%%z, example: Sun, 13 Feb 2022 23:35:51'
        ' +0300 (the current time is used by default)'
    )
    parser.add_argument(
        '--user',
        type=str,
        default=get_user(),
        help='use the parameter to specify the full name and email address, example: Yaroslav Ryabtsev <yairyabtsev'
        '@edu.hse.ru> (data from git_config is used by default)'
    )
    # parser.add_argument(
    #     '--key',
    #     type=str,
    #     default='yes',
    #     help=''
    # )
    args = parser.parse_args()
    try_shell(f'git checkout {args.branch}')

    # try_shell(f'mv c2eo-{get_version()} c2eo-{args.version}')

    os.chdir('../project')
    if not os.path.exists('build'):
        os.mkdir('build')
    os.chdir('build')
    make_bin()

    try_shell('ldd ../bin/c2eo > ../../llvm-clang/libs.txt')
    copy_libs()

    os.chdir(f'../../release/c2eo-{args.version}/')
    if not os.path.exists('usr'):
        os.mkdir('usr')
        os.mkdir('usr/bin')
        os.mkdir('usr/lib')
    make_deb(args.version)
    os.chdir('..')
    try_shell(f'fakeroot dpkg-deb --build c2eo-{args.version}')
    # try_shell(f'lintian c2eo-{version}.deb') ---> checker

    os.chdir('repository/debian')
    make_repo(args.version)

    # os.chdir('..')
    # if not os.path.exists('auto_index'):
    #     os.mkdir('auto_index')
    # try_shell(f'cp -rf debian/pool auto_index')
    # try_shell(f'cp -rf debian/dists auto_index')
    # host_it()
