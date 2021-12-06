import yaml
import os.path
import requests
from yaml.loader import SafeLoader


settings_file = 'data/settings.yml'
config_path = 'data/config/'


def get_setting(setting_name):
    if setting_name == 'latest_eo_version':
        return get_latest_eo_version()

    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    setting = data[setting_name]
    if 'path' in setting_name:
        setting = os.path.join(setting, '')
    return setting


def set_setting(setting_name, value):
    with open(settings_file) as f:
        data = yaml.load(f, Loader=SafeLoader)
    data[setting_name] = value
    with open(settings_file, 'w') as f:
        f.write(yaml.dump(data))


def get_latest_eo_version():
    print('Check latest EO version')
    url = 'https://search.maven.org/solrsearch/select?q=g:"org.eolang"a:"eo-parent"&rows=1&wt=json'
    data = requests.get(url).json()
    latest_version = data['response']['docs'][0]['latestVersion']
    print(f'Latest EO version: "{latest_version}"')
    return latest_version


def get_meta_code(meta_name, read_as_lines=False):
    with open(f'data/{meta_name}.txt', 'r') as f:
        if read_as_lines:
            return f.readlines()
        else:
            return f.read()


def get_config(config_name):
    config = f'{config_path}{config_name}'
    if os.path.exists(config):
        with open(config, 'r') as f:
            return f.readlines()
    return []
