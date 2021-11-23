#! /usr/bin/python3

import requests
import os
import re
import glob
import sys


def main():
    latest_version = get_latest_version()
    file = 'latest_eo_version.txt'
    current_version = get_current_version(file)
    compare = version_compare(current_version, latest_version)

    if compare == 0:
        print('We use latest EO version\n')
        return
    elif compare == 1:
        latest_version = current_version
        print(f'Manual update latest EO version to {latest_version}\n')
    else:
        print(f'We use old EO version: "{current_version}"\nStart updating files')

    path_to_files = '../../**'
    file_pattern = 'pom.xml'
    found_files = search_files_by_pattern(path_to_files, file_pattern)

    count_changed_files = update_version_in_files(found_files, latest_version)
    with open(file, 'w') as f:
        data = f.write(latest_version)
    print('EO version updated\n')
    return


def version_compare(ver1, ver2):
    tokens1 = ver1.split('.')
    tokens2 = ver2.split('.')
    for i in range(len(tokens1)):
        if int(tokens1[i]) > int(tokens2[i]):
            return 1
        elif int(tokens1[i]) < int(tokens2[i]):
            return -1
    return 0


def get_latest_version():
    print('Check latest EO version')
    url = 'https://search.maven.org/solrsearch/select?q=g:"org.eolang"a:"eo-parent"&rows=1&wt=json'
    data = requests.get(url).json()
    latest_version = data['response']['docs'][0]['latestVersion']
    print(f'Latest EO version: "{latest_version}"')
    return latest_version


def get_current_version(file):
    if not os.path.isfile(file):
        return 'not found'

    with open(file, 'r') as f:
        current_version = f.read()
    return current_version


def search_files_by_pattern(path, file_pattern):
    print(f'Looking for "{file_pattern}" files')
    pattern = os.path.join(path, file_pattern)
    found_files = glob.glob(pattern, recursive=True)
    print(f'Found {len(found_files)} files')
    return found_files


def update_version_in_files(files, latest_version):
    print('Updating version')
    count_changed_files = 0
    pattern = r'<eolang\.version>.*<\/eolang\.version>'
    latest_version_declaration = f'<eolang.version>{latest_version}</eolang.version>'
    for file in files:
        with open(file, 'r') as f:
            data = f.read()

        result = re.search(pattern, data)
        if (not result) or (latest_version_declaration in result.group()):
            continue

        new_data = re.sub(pattern, latest_version_declaration, data)
        with open(file, 'w') as f:
            f.write(new_data)
        count_changed_files += 1

    print(f'{count_changed_files} files updated')
    return count_changed_files


if __name__ == '__main__':
    os.chdir(os.path.dirname(sys.argv[0]))  # Go to current script dir
    main()
