#! /usr/bin/python3
# Прохождение всех тестов в заданном каталоге
import os
# import sys
import time
from datetime import timedelta

if __name__ == '__main__':
    start_time = time.monotonic()
    # Фиксация текущего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    #print(f'Splitter Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    # Получение списка каталогов в каждом из которых содержится один тест
    testedFileList = os.listdir('./tests')
    print(testedFileList)

    testCount = 0
    passCount = 0
    print('Test Started')
    testResults = [] # Информация о результататах
    for testedFile in testedFileList:
        value = os.system(tmpDir + '/testOne.py ' + testedFile)
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

    end_time: float = time.monotonic()
    delta = timedelta(seconds=end_time - start_time)
    print(f'testAll execution time is {delta}')

    os.chdir(currentDir)
