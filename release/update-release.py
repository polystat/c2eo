import argparse
import datetime

from git_config import get_config


def get_user():
    config = get_config()
    return "{} <{}>".format(config["user"]["name"], config["user"]["email"])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Release maker')
    parser.add_argument(
        '--branch',
        type=str,
        default='master',
        help=''
    )
    parser.add_argument(
        '--version',
        type=str,
        default="1.0-2",  # inc(get_version()),
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
    # input, output = args.input, args.output
    # print(input, output)
