#! /usr/bin/python3
# Прохождение всех тестов в заданном каталоге
import os
import sys
import glob
#import shutil

if __name__ == '__main__':
    # Фиксация текущего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    #print(f'Splitter Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

   # Получение списка каталогов в каждом из которых содержится один тест
    cleanDirList = os.listdir('./tests')
    print(cleanDirList)

    print('Cleanup Started')
    testResults = [] # Информация о результататах
    for cleanDir in cleanDirList:
        cleanDir = tmpDir + '/tests/' + cleanDir
        # Очистка каталога от ненужных файлов
        for file in list(glob.glob(os.path.join(cleanDir, '*.txt'))):
            os.remove(file)
        for file in list(glob.glob(os.path.join(cleanDir, '*-eo.c'))):
            os.remove(file)
        for file in list(glob.glob(os.path.join(cleanDir, '*.eo'))):
            os.remove(file)
        for file in list(glob.glob(os.path.join(cleanDir, '*.out'))):
            os.remove(file)
        for file in list(glob.glob(os.path.join(cleanDir, '*.log'))):
            os.remove(file)
        print(f'Derwctory {cleanDir} is clean')

    # print(testResults)
    for testResult in testResults:
        print(testResult)

    print(f'The End. {testCount} tests evaluated, {passCount} tests passed')

    os.chdir(currentDir)
