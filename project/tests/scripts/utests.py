import os
import unittest
import datetime
import system_vars
import global_tests
import static_tests
import sys

if __name__ == '__main__':
    argc = len(sys.argv)
    argv = sys.argv
    system_vars.path = argv[0].replace('utests.py', '')

    while not os.path.exists(system_vars.path + system_vars.directory):
        os.makedirs(system_vars.path + system_vars.directory)

    with open(system_vars.path + system_vars.logfile1, 'w') as log1:
        log1.write('')
    with open(system_vars.path + system_vars.logfile2, 'w') as log2:
        log2.write('')

    system_vars.full_log = datetime.datetime.now().strftime("%y-%m-%d#%H-%M.log")
    sys.stdout = open(system_vars.path + system_vars.full_log, 'w')

    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(global_tests.GlobalTestCase))
    suite.addTest(unittest.makeSuite(static_tests.StaticTestCase))
    unittest.TextTestRunner(verbosity=2).run(suite)

    sys.stdout.close()
