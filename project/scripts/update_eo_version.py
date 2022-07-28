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
import re as regex

# Our scripts
import tools
import settings


def main():
    tools.pprint()
    current_version = settings.get_setting('current_eo_version')
    latest_version = settings.get_setting('latest_eo_version')

    is_latest_version, latest_version = is_update_needed(current_version, latest_version)
    if is_latest_version:
        return

    path_to_eo_project = settings.get_setting('path_to_eo_project')
    found_files = tools.search_files_by_patterns(path_to_eo_project, ['pom.xml'], recursive=True)
    update_version_in_files(found_files, latest_version)
    settings.set_setting('current_eo_version', latest_version)
    tools.pprint('EO version updated\n')


def is_update_needed(current_version, latest_version):
    compare = tools.version_compare(current_version, latest_version)
    is_latest_version = False
    if compare == 1:
        latest_version = current_version
        tools.pprint(f'Manual update latest EO version to {latest_version}', status=tools.WARNING)
    elif compare == 0:
        is_latest_version = True
        tools.pprint('We use latest EO version', status=tools.PASS)
        tools.pprint()
    else:
        tools.pprint(f'We use old EO version: "{current_version}"', status=tools.WARNING)
        tools.pprint(f'Start updating files')

    return is_latest_version, latest_version


def update_version_in_files(files, latest_version):
    tools.pprint('Updating version')
    count_changed_files = 0
    pattern = r'<eolang\.version>.*<\/eolang\.version>'
    latest_version_declaration = f'<eolang.version>{latest_version}</eolang.version>'
    for file in files:
        with open(file, 'r') as f:
            data = f.read()

        result = regex.search(pattern, data)
        if (not result) or (latest_version_declaration in result.group()):
            continue

        new_data = regex.sub(pattern, latest_version_declaration, data)
        with open(file, 'w') as f:
            f.write(new_data)
        count_changed_files += 1

    tools.pprint(f'{count_changed_files} files updated')
    return count_changed_files


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    main()
