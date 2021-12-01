import yaml
import requests
from yaml.loader import SafeLoader


settings_file = 'data/settings.yml'


def get_setting(setting_name):
    if setting_name == 'latest_eo_version':
        return get_latest_eo_version()
    else:
        with open(settings_file) as f:
            data = yaml.load(f, Loader=SafeLoader)
        return data[setting_name]


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


def get_meta_code(meta_name):
    with open(f'data/{meta_name}.txt', 'r') as f:
        return f.read()


# TODO
# def get_configuration(configuration_name):
#     with open('data/{meta_name}.txt', 'r') as f:
#         return f.read()
