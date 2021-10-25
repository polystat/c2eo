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
    print('Test Started')
    testResults = [] # Информация о результататах
    for testedFile in testedFileList:
        value = os.system(currentDir + '/startTest.py ' + testedFile)
        testCount += 1
        testResults.append(str(testCount) + ': ' + testedFile + '  ' + str(value))

    print(testResults)
    print(f'The End. {testCount} tests evaluated')

