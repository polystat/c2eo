#! /usr/bin/python3
import os
import sys
import glob
import shutil
import time
from datetime import timedelta

# Данный модуль запускается для c-файлов, специально подготовленных для EO
if __name__ == '__main__':
    start_time = time.monotonic()
    # Фиксация текущего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)

    # Проверка числа аргументов командной строки
    # Должен быть один аргумент, определяющий компилируемый Си-файл
    argc = len(sys.argv)
    argv = sys.argv
    # При одном аргументе запускается один тест, если он находитс в каталоге tests
    if argc == 2:
        print(f'Start test in directory {argv[1]}')
    else:
        # Не то число аргументов
        print(f'1) Numer of files =  {argc} Incorrect')
        os.chdir(currentDir)
        exit(1)

    # Проверка, что данный файл существует
    cProgramName = argv[1]
    if os.path.isfile(cProgramName):
        print(f'C file is: {cProgramName}')
    else:
        # Пока выход
        print(f'C-file {cProgramName} is absent')
        os.chdir(currentDir)
        exit(3)

    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    # Далее запускается стартер
    launcherStart = tmpDir + '/launcher.py ' + cProgramName
    os.system(launcherStart)

    resultDir = '../../result/'
    # Получение абсолютного пути до каталога с проектом на EO
    resultDir = os.path.abspath(resultDir)
    os.chdir(resultDir)
    os.system('ls')
    os.system('pwd')

    # При наличии global.eo начинается процесс компиляции
    eoMavenCode = os.system('mvn clean compile')
    print('eoMavenCode = ', eoMavenCode)
    if eoMavenCode != 0:
        print('Incorrect Maven Project Assembly')
        os.chdir(currentDir)
        exit(-4)

    # Запуск программы на EO с переносом результата в файл для сравнения
    eoRunCode = os.system('./run.sh')
    print('eoRunCode = ', eoRunCode)
    if eoRunCode != 0:
        print('Incorrect EO runtime')
        os.chdir(currentDir)
        exit(-3)

    end_time: float = time.monotonic()
    delta = timedelta(seconds=end_time - start_time)
    print(f'testOne execution time is {delta}')

    os.chdir(currentDir)
