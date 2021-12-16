import yaml
import os.path
import requests
from yaml.loader import SafeLoader

# Our scripts
import tools


settings_file = 'data/settings.yml'


def get_setting(setting_name):
    if setting_name == 'latest_eo_version':
        return get_latest_eo_version()

    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    setting = data[setting_name]
    if 'path' in setting_name and '.' not in setting:
        setting = os.path.join(setting, '')
    return setting


def set_setting(setting_name, value):
    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    data[setting_name] = value
    with open(settings_file, 'w') as f:
        f.write(yaml.dump(data))


def get_latest_eo_version():
    tools.pprint('Check latest EO version')
    url = 'https://search.maven.org/solrsearch/select?q=g:"org.eolang"a:"eo-parent"&rows=1&wt=json'
    data = requests.get(url).json()
    latest_version = data['response']['docs'][0]['latestVersion']
    tools.pprint(f'Latest EO version: "{latest_version}"')
    return latest_version


def get_meta_code(name, read_as_lines=False):
    path = get_setting('path_to_meta')
    file = os.path.join(path, f'{name}.txt')
    with open(file, 'r') as f:
        if read_as_lines:
            return f.readlines()
        else:
            return f.read()


def get_config(name):
    path = get_setting('path_to_config')
    file = os.path.join(path, f'{name}.txt')
    if os.path.isfile(file):
        with open(file, 'r') as f:
            return set(map(lambda line: line.strip(), f.readlines()))
    else:
        return None
