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
    print(path_to_script)
    path_to_script = os.path.dirname(path_to_script)
    print(path_to_script)
    if os.path.exists(path_to_script):
        os.chdir(path_to_script)
    parser = argparse.ArgumentParser(description='Release maker')
    parser.add_argument(
        '--branch',
        type=str,
        default='yar',
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
    print(args)
    subprocess.run(f'git checkout {args.branch}', shell=True)
    subprocess.run(f'mv c2eo-{get_version()} c2eo-{args.version}', shell=True)

    # input, output = args.input, args.output
    # print(input, output)
