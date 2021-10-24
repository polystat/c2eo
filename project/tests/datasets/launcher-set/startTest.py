#! /usr/bin/python3
import os

if __name__ == '__main__':
    testCount = 0
    print('Test Started')
    currentDir = os.getcwd()
    os.chdir('./swapBCA/')
    os.system('python3 startLauncher.py')
    testCount += 1
    os.chdir(currentDir)
    os.chdir('./swapBC/')
    os.system('python3 startLauncher.py')
    testCount += 1
    os.chdir(currentDir)
    os.chdir('./testEmpty/')
    os.system('python3 startLauncher.py')
    testCount += 1
    os.chdir(currentDir)
    print(f'The End. {testCount} tests passed')
