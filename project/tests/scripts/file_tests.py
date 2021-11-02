import os
import unittest
import datetime
import system_vars
import sys
import fun

directory = 'dataset'


class FileTests(unittest.TestCase):
    file_name = ""

    def test_something(self):
        sys.stdout.flush()
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
        with open(system_vars.path + system_vars.filename1, 'w') as fout:
            print(code, file=fout)
        # to log
        print("\nCODE#1:")
        print(code)
        code = code.replace('//', '')
        with open(system_vars.path + system_vars.filename2, 'w') as fout:
            print(code, file=fout)
        # to log
        print("\nCODE#2:")
        print(code)


if __name__ == '__main__':
    argc = len(sys.argv)
    argv = sys.argv
    system_vars.path = argv[0].replace('file_tests.py', '')

    while not os.path.exists(system_vars.path + system_vars.directory):
        os.makedirs(system_vars.path + system_vars.directory)

    with open(system_vars.path + system_vars.logfile1, 'w') as log1:
        log1.write('')
    with open(system_vars.path + system_vars.logfile2, 'w') as log2:
        log2.write('')

    system_vars.full_log = datetime.datetime.now().strftime("%y-%m-%d#%H-%M.log")
    sys.stdout = open(system_vars.path + system_vars.full_log, 'w')
    # todo: fix sys.stdout

    suite = unittest.TestSuite()

    for root, dir, files in os.walk(system_vars.path + directory):
        for file in files:
            if '.c' in file[-2:]:
                tests = FileTests
                tests.file_name = os.path.join(root, file)
                suite.addTest(unittest.makeSuite(tests))

    unittest.TextTestRunner(verbosity=2, failfast=True, buffer=True).run(suite)

    sys.stdout.close()
