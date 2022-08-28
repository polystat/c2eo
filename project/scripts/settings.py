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

import yaml
import requests
from pathlib import Path
from yaml.loader import SafeLoader

# Our scripts
import tools

settings_file = 'data/settings.yml'


def get_setting(setting_name: str) -> str | list[str] | Path:
    if setting_name == 'latest_eo_version':
        return get_latest_eo_version()

    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    setting = data[setting_name]
    if 'path' in setting_name:
        setting = Path(setting)
    return setting


def set_setting(setting_name: str, value) -> None:
    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    data[setting_name] = value
    with open(settings_file, 'w') as f:
        f.write(yaml.dump(data))


def get_latest_eo_version() -> str:
    tools.pprint('Check latest EO version')
    url = 'https://search.maven.org/solrsearch/select?q=g:"org.eolang"a:"eo-parent"&rows=1&wt=json'
    data = requests.get(url).json()
    latest_version = str(data['response']['docs'][0]['latestVersion'])
    tools.pprint(f'Latest EO version: "{latest_version}"')
    return latest_version


def get_meta_code(name: str, read_as_lines: bool = False) -> str | list[str]:
    path = get_setting('path_to_meta')
    file = path / f'{name}.txt'
    with open(file, 'r') as f:
        return f.readlines() if read_as_lines else f.read()


def get_skips(name: str) -> dict:
    path = get_setting('path_to_skips')
    file = path / f'{name}.txt'
    skips = {}
    with open(file, 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue

            if line := line.rstrip():
                filters, comment = line.split(':', maxsplit=1)
                for _filter in filters.split(','):
                    skips[_filter.strip()] = comment.strip()
    return skips
