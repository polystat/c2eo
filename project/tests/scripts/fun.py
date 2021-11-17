import os
import difflib
import sys
from collections import defaultdict
from typing import List

from system_vars import *


def compile_run():
    if os.system(' '.join(
            ['gcc', path + filename2, '-o', path + c_bin, '-w', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not compile c-code'
    if os.system(' '.join([f'./{path}{c_bin}', f'>{path}{c_out}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not run c-code'
    if os.system(' '.join([path + launcher, f'{path}{filename1}', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not transpile c-code to eo-code'
    # to log
    print('\nRESULT:')
    with open(f'{path}{resultDir}/eo/c2eo/src/global.eo', 'r') as glob:
        print(glob.read())
    if os.system(' '.join(
            ['mvn -f', path + resultDir, 'clean compile', f'>>{path}{logfile1}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not compile eo-code using mvn'
    if os.system(' '.join(['java', '-cp', f'{path}{resultDir}/target/classes:'
                                          f'{path}{resultDir}/target/eo-runtime.jar',
                           'org.eolang.Main', 'c2eo.app', f'>{path}{eo_out}', f'2>>{path}{logfile2}'])) != 0:
        return False, 'can not run eo-code'
    return True, 'too easily'


def generate1(c_types, declaration, names):
    code = '\n'.join([declaration, 'int main() {',
                      *[f'\t{names[i]};' for i in range(len(names))],
                      '\treturn 0;', '}'])
    with open(path + filename1, 'w') as fout:
        print(code, file=fout)
    # to log
    print('\nCODE#1:')
    print(code)


def generate2(c_types, declaration, names):
    code = '\n'.join(['#include "stdio.h"', declaration, 'int main() {',
                      *[f'\tprintf("%{c_types[i][0]}\\n", {names[i]});' for i in range(len(names))],
                      '\treturn 0;', '}'])
    with open(path + filename2, 'w') as fout:
        print(code, file=fout)
    # to log
    print('\nCODE#2:')
    print(code)


def generate_vars(c_types, values=None, static=None):
    N = len(c_types)
    if (values and N != len(values)) or \
            (static and N != len(static)):
        return '', []
    names: List[str] = []
    counter = defaultdict(int)
    declaration = ''
    for i in range(N):
        if static and static[i]:
            declaration += 'static' + ' '
        declaration += c_types[i][1] + ' '
        name = c_types[i][0]
        if c_types[i][1] == '_Bool':
            name = 'b'
        counter[name] += 1
        names.append(name + str(counter[name]))
        declaration += names[-1] + ' '
        if values:
            declaration += '= ' + str(values[i]) + ';\n'
    return declaration, names


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
            print('\nDIFF:')
            for line in diff:
                print(line, end='')
            return (len(diff) == 0), 'there are some diffs'


def showname(name=None):
    sys.stdout.flush()
    print()
    print('#' * 16)
    print(sys._getframe(1).f_globals['__name__'],
          sys._getframe(1).f_code.co_name)
    if name:
        print(name)
