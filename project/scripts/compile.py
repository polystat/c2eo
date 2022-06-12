#! /usr/bin/python3

import sys
import time

# Our scripts
import tools
import settings
import update_eo_version
from build_eo import EOBuilder
from transpile_c2eo import Transpiler


class Compiler(object):

    def __init__(self, path_to_tests=None, config=None):
        if path_to_tests is None:
            path_to_tests = settings.get_setting('path_to_tests')
        if config is None:
            config = settings.get_setting('config')
        self.filters = settings.get_config(config)
        self.path_to_tests = path_to_tests
        self.path_to_c2eo_build = settings.get_setting('path_to_c2eo_build')
        self.transpilation_units = []

    def compile(self):
        self.transpilation_units = Transpiler(self.path_to_tests, self.filters, False).transpile()
        if self.transpilation_units:
            EOBuilder().build()
            tools.pprint(f'\n{"-" * 60}', slowly=True)
            tools.pprint('COMPILE RESULTS', slowly=True)
            tools.pprint(f'{"-" * 60}', slowly=True)
            tools.pprint(f'{len(self.transpilation_units)} files', slowly=True, status='PASS')


if __name__ == '__main__':
    start_time = time.time()
    tools.move_to_script_dir(sys.argv[0])
    Compiler(tools.get_or_none(sys.argv, 1), tools.get_or_none(sys.argv, 2)).compile()
    end_time = time.time()
    time_span = int(end_time - start_time)
    tools.pprint('Total time:  {:02}:{:02} min.'.format(time_span // 60, time_span % 60), slowly=True)
    tools.pprint(f'{"-" * 60}\n', slowly=True)
