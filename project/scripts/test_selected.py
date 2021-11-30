#! /usr/bin/python3
# Прохождение тестов из каталога tests, заданных в конфигурационном файле
# расположенном в каталоге configuration
import os
import sys
# import glob
# import shutil
import time
from datetime import timedelta

if __name__ == '__main__':
    start_time = time.monotonic()
    # Фиксация текущего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    # print(f'Splitter Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    # Проверка числа аргументов командной строки
    argc = len(sys.argv)
    argv = sys.argv
    # При одном аргументе запускается задается одна конфигурация
    # из каталога configuration
    if argc == 2:
        print(f'Config is in directory {argv[1]}')
    else:
        # Пока выход
        print(f'1) Numer of files =  {argc} Incorrect')
        exit(1)

    os.system('./../../bin/eo_version_update.py') # Update EO version in pom.xml
    os.chdir('../../bin')
    os.system('cmake ..') # Rebuild project
    os.system('make')
    os.chdir('../tests/any')

    # Проверка, что данный конфигурационный файл существует
    configFileName = tmpDir + '/configuration/' + argv[1]
    if os.path.isfile(configFileName):
        print(f'Configuration  file is: {configFileName}')
    else:
        # Пока выход
        print(f'2) Configuration  file is: {configFileName} is absent')
        exit(2)

    # Открытие конфигурационного файла и взятие из него списка каталогов с тестовыми файлами
    configFile = open(configFileName)
    print('Test Started')
    testResults = []  # Информация о результататах
    passCount = 0
    testCount = 0
    testedDirName = configFile.readline()[0:-1]

    while testedDirName:
        # Проверка на существование данного каталога
        testedDir = tmpDir + '/tests/' + testedDirName
        if os.path.exists(testedDir):
            print(f'Tested Directory is: {testedDir}')
            # Далее идет запуск теста
            value = os.system(tmpDir + '/testOne.py ' + testedDirName)
            testCount += 1
            if value == 0:
                message = 'PASS'
                passCount += 1
            else:
                message = 'FAIL'
            testResults.append(str(testCount) + ': ' +
                               testedDirName + '  ' + message)
        else:
            # Пропуск каталога с информацией о некорректном имени
            print(
                f'Line {testCount}: Tested Directory {testedDir} is absent. Test Ignored')
        testedDirName = configFile.readline()[0:-1]

    # print(testResults)
    for testResult in testResults:
        print(testResult)

    print(f'The End. {testCount} tests evaluated, {passCount} tests passed')

    end_time: float = time.monotonic()
    delta = timedelta(seconds=end_time - start_time)
    print(f'testSelected execution time is {delta}')

    os.chdir(currentDir)
