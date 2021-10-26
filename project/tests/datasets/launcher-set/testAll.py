#! /usr/bin/python3
# Прохождение всех тестов в заданном каталоге
import os
import sys

if __name__ == '__main__':
    # Фиксация текущего каталога
    currentDir = os.getcwd()
    # Получение списка каталогов в каждом из которых содержится один тест
    testedFileList = os.listdir('./tests')
    print(testedFileList)

    testCount = 0
    passCount = 0
    print('Test Started')
    testResults = [] # Информация о результататах
    for testedFile in testedFileList:
        value = os.system(currentDir + '/testOne.py ' + testedFile)
        testCount += 1
        if value == 0:
            message = 'PASS'
            passCount += 1
        else:
            message = 'FAIL'
        testResults.append(str(testCount) + ': ' + testedFile + '  ' + message)

    # print(testResults)
    for testResult in testResults:
        print(testResult)

    print(f'The End. {testCount} tests evaluated, {passCount} tests passed')

