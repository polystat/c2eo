#! /usr/bin/python3

import os
import sys
import subprocess

# Our scripts
import tools
import settings


class EOBuilder(object):

    def __init__(self):
        self.path_to_eo_project = settings.get_setting('path_to_eo_project')
        self.current_version = settings.get_setting('current_eo_version')
        self.path_to_foreign_objects = settings.get_setting('path_to_foreign_objects')
        self.path_to_eo = settings.get_setting('path_to_eo')
        self.path_to_eo_parse = settings.get_setting('path_to_eo_parse')

    def build(self):
        original_path = os.getcwd()
        os.chdir(self.path_to_eo_project)
        if self.is_good_for_recompilation():
            tools.pprint('\nRecompilation eo project start\n')
            result = subprocess.run('mvn compile', shell=True)
        else:
            tools.pprint('Full eo project compilation start\n')
            result = subprocess.run('mvn clean compile', shell=True)
        os.chdir(original_path)
        return not result.returncode

    def is_good_for_recompilation(self):
        if not os.path.exists(self.path_to_foreign_objects):
            tools.pprint('Compile dir not found', status='WARN')
            return False
        else:
            tools.pprint('Compile dir found', status='PASS')

        if not self.is_actual_object_version():
            tools.pprint('Old version detected', status='WARN')
            return False
        else:
            tools.pprint('Latest version detected', status='PASS')

        eo_src_files = tools.search_files_by_pattern(self.path_to_eo, '*.eo', recursive=True)
        eo_src_files = set(map(lambda x: x.replace(self.path_to_eo, '', 1).replace('.eo', '', 1), eo_src_files))
        project_eo_files = tools.search_files_by_pattern(self.path_to_eo_parse, '*.xmir',
                                                         recursive=True, filters=['!org/eolang'])
        project_eo_files = set(map(lambda x: x.replace(self.path_to_eo_parse, '', 1).replace('.xmir', '', 1),
                                   project_eo_files))
        difference = project_eo_files - eo_src_files
        if difference:
            tools.pprint('EO project files are incompatible', status='WARN')
            tools.pprint(f'The following files may have been deleted: {difference}\n')
            return False
        else:
            tools.pprint('EO project files are compatible', status='PASS')

        return True

    def is_actual_object_version(self):
        tools.pprint('\nCheck version of compiled eo objects')
        data = tools.read_file_as_dictionary(self.path_to_foreign_objects)
        for package in data:
            if package['version'] not in ['*.*.*', '0.0.0']:
                compare = tools.version_compare(self.current_version, package['version'])
                if compare <= 0:
                    return True
        return False


if __name__ == '__main__':
    tools.move_to_script_dir(sys.argv[0])
    EOBuilder().build()
