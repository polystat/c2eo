import datetime
import os
import re
import subprocess
import sys

import argparse
from git_config import get_config

libs_set = set()
control = {
    'Package': 'c2eo',
    'Version': 'X.X-X',
    'Provides': 'c2eo',
    'Maintainer': 'Anonymous <anonymous@noreply.com>',
    'Architecture': 'all',
    'Section': 'misc',
    'Description': 'The translator of C to EOLANG.\n This is a translator of C/C++ to EOLANG for Debian and Ubuntu.',
    'Depends': 'dpkg, wget, python3',
    'Build-Depends': 'debhelper (>=9)',
    'Pre-Depends': 'wget (>= 1.0)',
    'Origin': 'https://github.com/polystat/c2eo',
    'Multi-Arch': 'foreign'
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
    'Description': 'repository for c2eo releases',
    'SignWith': 'A0398E7D2B2846BF006D31C9F7C91591CC543ECA'
}


def get_user() -> str:
    config = get_config()
    return "{} <{}>".format(config["user"]["name"], config["user"]["email"])


def try_shell(param):
    if subprocess.run(param, shell=True).returncode:
        exit(0)


def make_deb(version, user, date):
    global libs_set, control
    for path in libs_set:
        try_shell(f'cp {path} usr/lib')
        try_shell(f'cp {path[:-3]} usr/lib')
    try_shell('cp ../../project/bin/c2eo usr/bin')
    try_shell('cp ../../license.txt DEBIAN/copyright')
    with open('DEBIAN/control', 'w') as control_file:
        for key in control.keys():
            print(f'{key}: {control[key]}', file=control_file)
    with open('DEBIAN/changelog', 'w') as changelog:
        print('c2eo ({}.{}-{}) unstable; urgency=medium'.format(*version.split('.')), file=changelog)
        print(file=changelog)
        # while True:
        #     changes = input('describe the new feature: ')
        #     if not changes:
        #         break
        #     print('  *', changes, file=changelog)

        # with datetime try_shell('git log $(git describe --tags --abbrev=0)..HEAD --merges --oneline --format="  * %h
        # %s by %an <%aE>   %cd" >> DEBIAN/changelog')
    try_shell('git log $(git describe --tags --abbrev=0)..HEAD --merges --oneline --format="  * %h %s by %an <%aE>"'
              ' >> DEBIAN/changelog')
    with open('DEBIAN/changelog', 'a') as changelog:
        print(file=changelog)
        print(' --', user, ' ', date, file=changelog)
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
        exit()


if __name__ == '__main__':
    path_to_script = sys.argv[0]
    path_to_script = os.path.abspath(path_to_script)
    path_to_script = os.path.dirname(path_to_script)
    os.chdir(path_to_script)

    try_shell('rm -rf c2eo*')
    try_shell('rm -rf repo*')

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
        default='X.X-X',
        help='specify the new version'
    )
    args = parser.parse_args()

    date = datetime.datetime.now().astimezone().strftime('%a, %d %b %Y %H:%M:%S %z')
    user = get_user()

    control['Version'] = args.version
    control['Maintainer'] = user

    try_shell(f'git checkout {args.branch}')

    os.chdir('../project')
    if not os.path.exists('build'):
        os.mkdir('build')
    os.chdir('build')
    make_bin()

    try_shell('ldd ../bin/c2eo > ../../llvm-clang/libs.txt')
    copy_libs()

    os.chdir('../../release/')
    os.makedirs('repository/conf', exist_ok=True)
    os.mkdir(f'c2eo-{args.version}/')
    os.chdir(f'./c2eo-{args.version}/')
    os.makedirs('usr/bin', exist_ok=True)
    os.makedirs('usr/lib', exist_ok=True)
    os.makedirs('DEBIAN', exist_ok=True)

    make_deb(args.version, user, date)
    os.chdir('..')
    try_shell(f'fakeroot dpkg-deb --build c2eo-{args.version}')
    # try_shell(f'lintian c2eo-{version}.deb') ---> checker

    os.chdir('repository')
    make_repo()
