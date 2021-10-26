import os
import difflib
import sys

from system_vars import *


def compile_run():
    if os.system(' '.join(
            ['gcc', path + filename2, '-o', path + c_bin, '-w', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not compile c-code'
    if os.system(' '.join([f'./{path}{c_bin}', f'>{path}{c_out}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not run c-code'
    if os.system(' '.join([path + launcher, f'{path}{filename1}', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not transpile c-code to eo-code'
    if os.system(' '.join(
            ['mvn -f', path + resultDir, 'clean compile', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not compile eo-code using mvn'
    # to log
    print("\nRESULT:")
    with open(f'{path}{resultDir}/eo/c2eo/src/global.eo', 'r') as glob:
        print(glob.read())
    if os.system(' '.join(['java', '-cp', f'{path}{resultDir}/target/classes:'
                                          f'{path}{resultDir}/target/eo-runtime.jar',
                           'org.eolang.Main', 'c2eo.app', f'>{path}{eo_out}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not run eo-code'
    return True, 'too easily'


def generate(c_type, value, static=False):
    code = '\n'.join([f'{"static " if static else ""}{c_type[1]} var = {value};',
                      'int main() {', '\t%svar%s;', '\treturn 0;', '}'])
    with open(path + filename1, 'w') as fout:
        print(code % ('', ''),
              file=fout)
    # to log
    print("\nCODE#1:")
    print(code % ('', ''))

    code = '\n'.join(['#include "stdio.h"', code])
    with open(path + filename2, 'w') as fout:
        print(code % (f'printf("%{c_type[0]}\\n", ', ')'),
              file=fout)
    # to log
    print("\nCODE#2:")
    print(code % (f'printf("%{c_type[0]}\\n", ', ')'))


def compare():
    with open(path + c_out, 'r') as f1:
        with open(path + eo_out, 'r') as f2:
            diff = difflib.unified_diff(
                f1.readlines(),
                f2.readlines(),
                fromfile=c_out,
                tofile=eo_out,
            )
            # to log
            diff = list(diff)
            print("\nDIFF:")
            for line in diff:
                print(line, end='')
            return (len(diff) == 0), 'there are some diffs'


def showname():
    sys.stdout.flush()
    print()
    print('#'*16)
    print(sys._getframe(1).f_globals['__name__'], sys._getframe(1).f_code.co_name)
