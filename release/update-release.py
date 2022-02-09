import argparse
import datetime
import os
import subprocess

import sys
from git_config import get_config


def get_user() -> str:
    config = get_config()
    return "{} <{}>".format(config["user"]["name"], config["user"]["email"])


def inc(version: str) -> str:
    version = version.split('.')
    version[-1] = str(int(version[-1]) + 1)
    return '.'.join(version)


def get_version() -> str:
    for file in os.listdir("."):
        if file[:5] == 'c2eo-':
            return file[5:]
    return "?.?.0"


if __name__ == '__main__':
    path_to_script = sys.argv[0]
    path_to_script = os.path.dirname(path_to_script)
    os.chdir(path_to_script)
    parser = argparse.ArgumentParser(description='Release maker')
    parser.add_argument(
        '--branch',
        type=str,
        default='heap',
        help=''
    )
    parser.add_argument(
        '--version',
        type=str,
        default=inc(get_version()),
        help=''
    )
    parser.add_argument(
        '--date',
        type=str,
        default=datetime.datetime.now().astimezone().strftime('%a, %d %b %Y %H:%M:%S %z'),
        help=''
    )
    parser.add_argument(
        '--user',
        type=str,
        default=get_user(),
        help=''
    )
    args = parser.parse_args()
    subprocess.run(f'git checkout {args.branch}', shell=True)
    subprocess.run(f'mv c2eo-{get_version()} c2eo-{args.version}', shell=True)

    # making bin
    os.chdir('../project/build')  # or create
    subprocess.run('cmake ..', shell=True)
    subprocess.run('make', shell=True)

    # making deb
    os.chdir(path_to_script)
    # todo: subprocess.run(cp -rf /usr/lib/c2eo-lib ./c2eo-{args.version}/usr/lib/c2eo-lib)
    subprocess.run(f'cp ../project/bin/c2eo ./c2eo-{args.version}/usr/bin', shell=True)
    with open(f'./c2eo-{args.version}/DEBIAN/control', 'r') as control_file:
        control = control_file.read()
        control = control.replace('{}.{}-{}'.format(*get_version().split('.')),
                                  '{}.{}-{}'.format(*args.version.split('.')))
    with open(f'./c2eo-{args.version}/DEBIAN/control', 'w') as control_file:
        print(control, file=control_file)
    os.chdir(f'./c2eo-{args.version}')
    with open(f'./DEBIAN/changelog', 'w') as changelog:
        print('c2eo ({}.{}-{}) unstable; urgency=medium'.format(*args.version.split('.')), file=changelog)
        print(file=changelog)
        while True:
            changes = input('describe the new feature: ')
            if not changes:
                break
            print('  *', changes, file=changelog)
        print(file=changelog)
        print(' --', args.user, ' ', args.date, file=changelog)
    subprocess.run('md5deep -r usr > DEBIAN/md5sums', shell=True)
    os.chdir('..')
    subprocess.run(f'fakeroot dpkg-deb --build c2eo-{args.version}', shell=True)
    # subprocess.run(f'lintian c2eo-{args.version}.deb', shell=True) ---> checker

    # making repo
    os.chdir('repository/debian')
    for directory in ['db', 'dists', 'pool']:
        subprocess.run(f'rm -rf {directory}', shell=True)
    subprocess.run('reprepro export', shell=True)
    subprocess.run(f'reprepro -P -V -b . -C main includedeb c2eo-rep ../../c2eo-{args.version}.deb', shell=True)

    # hosting
    os.chdir('..')
    subprocess.run('az login', shell=True)
    subprocess.run('az webapp up -n c2eo -g demo --location=eastus2 --sku=FREE', shell=True)
