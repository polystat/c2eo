from utests import unittest
from system_vars import *
import fun


class FileTests(unittest.TestCase):
    file_name = ''

    def test_file(self):
        fun.showname(self.file_name)
        print()
        print('#' * 16)
        print(self.file_name)
        with open(self.file_name, 'r') as fin:
            self.generate(fin.read())
        res, msg = fun.compile_run()
        self.assertTrue(res, msg=msg)
        res, msg = fun.compare()
        self.assertTrue(res, msg=msg)

    @staticmethod
    def generate(code):
        with open(path + filename1, 'w') as fout:
            print(code, file=fout)
        # to log
        print('\nCODE#1:')
        print(code)
        code = code.replace('//', '')
        with open(path + filename2, 'w') as fout:
            print(code, file=fout)
        # to log
        print('\nCODE#2:')
        print(code)
