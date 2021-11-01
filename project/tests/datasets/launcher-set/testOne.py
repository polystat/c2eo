#! /usr/bin/python3
import os
import sys
import glob
import shutil
import re

def isFloat(strNum):
    # Проверка на соответствие числу через регулярное выражение
    result = re.fullmatch(r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?', strNum)
    if result:
        ####re.sub(r',', r'.', strNum)
        #print(f'strNum = {strNum}. It is float number')
        return True
    else:
        #print(f'strNum = {strNum}. It is not float number')
        return False

if __name__ == '__main__':
    # Фиксация текущего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    #print(f'testOne Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    # Проверка числа аргументов командной строки
    argc = len(sys.argv)
    argv = sys.argv
    # При одном аргументе запускается один тест, если он находитс в каталоге tests
    if argc == 2:
        print(f'Start test in directory {argv[1]}')
    else:
        # Пока выход
        print(f'1) Numer of files =  {argc} Incorrect')
        exit(1)
    # Проверка, что данный каталог существует
    testedDir = tmpDir + '/tests/' + argv[1]
    if os.path.exists(testedDir):
        print(f'Tested Directory is: {testedDir}')
    else:
        # Пока выход
        print(f'2) Tested Directory {testedDir} is absent')
        exit(2)
    # Имя программы на C должно совпадать с именем каталога
    cProgramName = testedDir + '/' + argv[1] + '.c'
    if os.path.isfile(cProgramName):
        print(f'C file is: {cProgramName}')
    else:
        # Пока выход
        print(f'3) C file {cProgramName} is absent')
        exit(3)

    # Предварительная очистка тестового каталога от лишних файлов.
    # Необходима, так как тестовая программа может измениться.
    for file in list(glob.glob(os.path.join(testedDir, '*.txt'))):
        os.remove(file)
    for file in list(glob.glob(os.path.join(testedDir, '*-eo.c'))):
        os.remove(file)
    for file in list(glob.glob(os.path.join(testedDir, '*.eo'))):
        os.remove(file)
    for file in list(glob.glob(os.path.join(testedDir, '*.out'))):
        os.remove(file)
    for file in list(glob.glob(os.path.join(testedDir, '*.log'))):
        os.remove(file)

    # Имя программы на C для EO должно совпадать с именем каталога плюс суффикс -eo
    cEoOnlyName = argv[1] + '-eo.c'
    cEoPathName = testedDir + '/' + cEoOnlyName
    # Файл создается в любом случае, так как тест м.б. переписан
    os.system('./splitter.py ' + cProgramName + ' ' + cEoPathName)

    # После этого можно входить в каталог и запускать компиляцию программы на C
    os.chdir(testedDir)
    os.system('clang ' + cProgramName)
    os.system('./a.out > cResult.txt')
    # Далее запускается стартер
    os.system('../../../../../bin/launcher.py ' + cEoOnlyName)

    resultDir = '../../../../../../result/'
    # Получение абсолютного пути до каталога с проектом на EO
    resultDir = os.path.abspath(resultDir)
    os.chdir(resultDir)
    os.system('ls')
    os.system('pwd')

    # Копирование в тестовый каталог файла global.eo для возможного анализа
    globalFileDir = resultDir + '/eo/c2eo/src/'
    if os.path.exists(os.path.join(globalFileDir, 'global.eo')):
        shutil.copy(os.path.join(globalFileDir, 'global.eo'), testedDir)
    else:
        print(f'File global.eo is absence in: {globalFileDir}')
        # Нет смысла продолжать дальше, так как тест не пройдет из-за отсутствия global.eo
        exit(-1)

    # При наличии global.eo начинается процесс компиляции
    eoMavenCode = os.system('mvn clean compile')
    print('eoMavenCode = ', eoMavenCode)
    if eoMavenCode != 0:
        print('Incorrect Maven Project Assembly')
        exit(-4)

    # Запуск программы на EO с переносом результата в файл для сравнения
    eoRunCode = os.system('./run.sh > ' + testedDir + '/' + 'eoResult.txt')
    print('eoRunCode = ', eoRunCode)
    if eoRunCode != 0:
        print('Incorrect EO runtime')
        exit(-3)

    # Сравнение результатов полученных при выполнении программ на C и EO
    cFile =  open(testedDir + '/cResult.txt', "r")
    eoFile = open(testedDir + '/eoResult.txt', "r")
    # Создается файл для формирования результатов сравнения
    logFile = open(testedDir + '/test.log', "w")

    erCount = 0
    cLine = cFile.readline()
    eoLine = eoFile.readline()

    # Сравнение длин списков перед их сопоставлением
    #if len(cLine) != len(eoLine):
        #Списки несопоставимы. Тест провален
        #print(f'Test FAIL: different results between sorce end transpiled programs')
        #exit(-2)

    iLine = 1
    while cLine and eoLine:
        if cLine != eoLine:
            # Проверка на числа с плавающей точкой и их эквивалентность
            if not(isFloat(cLine[:-1]) and isFloat(eoLine[:-1])):
                # Констатация различия
                print(f'  Noequal in line = {iLine}')   #: c({cLine}) != eo({eoLine})')
                logFile.write(f'  Noequal in line = {iLine}\n')
                erCount += 1
            else:   # Числа с плавающей точкой
                # Получение и сравнение этих чисел с заданной (небольшой) точностью
                if abs(float(cLine) - float(eoLine)) < 0.0001:
                    # Числа идентичны
                    print(f'  line = {iLine} is OK')        #: c({cLine}) == eo({eoLine})')
                    logFile.write(f'  line = {iLine} is OK\n')
                else:
                    # Числа не совпадают
                    print(f'  Noequal float numbers in line = {iLine}')
                    logFile.write(f'  Noequal float numbers in line = {iLine}\n')
                    erCount += 1
        else:
            print(f'  line = {iLine} is OK')        #: c({cLine}) == eo({eoLine})')
            logFile.write(f'  line = {iLine} is OK\n')
        cLine = cFile.readline()
        eoLine = eoFile.readline()
        iLine += 1

    if erCount > 0:
        print(f'Test FAIL: {erCount} errors')
        logFile.write(f'FAIL: {erCount} errors :-(\n')
    else:
        print(f'PASS')
        logFile.write(f'PASS :-)\n')

    cFile.close()
    eoFile.close()
    logFile.close()

    os.chdir(currentDir)
    exit(erCount)
