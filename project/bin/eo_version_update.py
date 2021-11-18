import requests
import os
import re


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


def search_files_by_name(path, file_name):
    print(f'Looking for {file_name} files')
    found_files = []
    for root, dirs, files in os.walk(path):
        for file in list(filter(lambda x: x == file_name, files)):
            found_files.append(os.path.join(root, file))
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
    latest_version = get_latest_version()
    file = 'latest_eo_version.txt'
    current_version = get_current_version(file)

    if current_version == latest_version:
        print('We use latest EO version')
        exit()

    print(f'We use old EO version: "{current_version}"')
    print('Start updating')

    path = '../..'
    file_name = 'pom.xml'
    found_files = search_files_by_name(path, file_name)

    count_changed_files = update_version_in_files(found_files, latest_version)
    with open(file, 'w') as f:
        data = f.write(latest_version)
