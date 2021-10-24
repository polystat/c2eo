#! /usr/bin/python3
import os

if __name__ == '__main__':
    currentDir = os.getcwd()
    os.system('clang testEmpty.c')
    os.system('./a.out > cResult.txt')
    os.system('../../../../bin/launcher.py testEmptyEo.c')

    os.chdir('../../../../../result/')
    os.system('ls')
    os.system('pwd')

    os.system('mvn clean compile')
    os.system('./run.sh > ' + currentDir + '/' + 'eoResult.txt')

    os.chdir(currentDir)
    os.system('ls')
    os.system('pwd')

    cFile =  open('cResult.txt', "r")
    eoFile = open('eoResult.txt', "r")

    iLine = 1
    erCount = 0
    cLine = 'start'
    eoLine = cLine
    while cLine and eoLine:
        cLine = cFile.readline()
        eoLine = eoFile.readline()
        if cLine != eoLine:
            print(f'  Noequal in line = {iLine}')   #: c({cLine}) != eo({eoLine})')
            erCount += 1
        else:
            print(f'  line = {iLine} is OK')        #: c({cLine}) == eo({eoLine})')
        iLine += 1

    if erCount > 0:
        print(f'Test FAIL: {erCount} errors')
    else:
        print(f'PASS')
