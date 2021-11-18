#! /usr/bin/python3
import os
import sys
import glob
import shutil
import re
import time
from datetime import timedelta
from typing import TextIO

app = '''[args...] > app
  global args > @
'''


def isFloat(strNum):
    # Проверка на соответствие числу через регулярное выражение
    result = re.fullmatch(r'[-+]?[0-9]*[.,][0-9]+(?:[eE][-+]?[0-9]+)?', strNum)
    if result:
        # re.sub(r',', r'.', strNum)
        # print(f'strNum = {strNum}. It is float number')
        return True
    else:
        # print(f'strNum = {strNum}. It is not float number')
        return False

# Функция получает предыдущее имя и порождает следующее.
# Диапазон имен: aaa-zzz


def genNextName(lastName):
    # Проверка на стартовое имя
    if (lastName >= 'aaa') and (lastName < 'zzz'):
        tmp_list = list(lastName)
        # print('tmp_list = ', tmp_list)
        if tmp_list[2] == 'z':
            if tmp_list[1] == 'z':
                tmp_list[0] = chr(ord(tmp_list[0])+1)
                tmp_list[1] = 'a'
                tmp_list[2] = 'a'
            else:
                tmp_list[1] = chr(ord(tmp_list[1])+1)
                tmp_list[2] = 'a'
        else:
            tmp_list[2] = chr(ord(tmp_list[2]) + 1)
    else:
        return 'aaa'
    new_name: str = ''.join(tmp_list)
    # print('lastName = ', lastName)
    return new_name

# Сравнение результатов полученных при выполнении программ на C и EO


def testDataCompare(testedDir):
    cFile = open(testedDir + '/cResult.txt', 'r')
    eoFile = open(testedDir + '/eoResult.txt', 'r')
    # Создается файл для формирования результатов сравнения
    logFile = open(testedDir + '/test.log', 'w')

    erCount = 0
    cLine = cFile.readline()
    eoLine = eoFile.readline()

    iLine = 1
    while cLine and eoLine:
        if cLine != eoLine:
            # Проверка на числа с плавающей точкой и их эквивалентность
            if not (isFloat(cLine[:-1]) and isFloat(eoLine[:-1])):
                # Констатация различия
                #: c({cLine}) != eo({eoLine})')
                print(f'  Noequal in line = {iLine}')
                logFile.write(f'  Noequal in line = {iLine}\n')
                erCount += 1
            else:  # Числа с плавающей точкой
                # Получение и сравнение этих чисел с заданной (небольшой) точностью
                if abs(float(cLine) - float(eoLine)) < 0.0001:
                    # Числа идентичны
                    #: c({cLine}) == eo({eoLine})')
                    print(f'  line = {iLine} is OK')
                    logFile.write(f'  line = {iLine} is OK\n')
                else:
                    # Числа не совпадают
                    print(f'  Noequal float numbers in line = {iLine}')
                    logFile.write(
                        f'  Noequal float numbers in line = {iLine}\n')
                    erCount += 1
        else:
            print(f'  line = {iLine} is OK')  #: c({cLine}) == eo({eoLine})')
            logFile.write(f'  line = {iLine} is OK\n')
        cLine = cFile.readline()
        eoLine = eoFile.readline()
        iLine += 1

    # Проверка, что после выхода обе строки имеют одинаковое значение
    # Противное может показывать, что число строк в результатах запуска не совпадает.
    # То есть, это будет сигнализировать о непрохождении теста.
    if cLine != eoLine:
        erCount += 1
        print(f'Test FAIL: different number of strings in C-code and EO-code')

    if erCount > 0:
        print(f'Test FAIL: {erCount} errors')
        logFile.write(f'FAIL: {erCount} errors :-(\n')
    else:
        print(f'PASS')
        logFile.write(f'PASS :-)\n')

    cFile.close()
    eoFile.close()
    logFile.close()

    return bool(erCount)


if __name__ == '__main__':
    # Фиксация времени начала работы скрипта
    start_time = time.monotonic()
    # Фиксация текущего каталога (из которого запускается скрипт)
    currentDir = os.getcwd()

    # Получение пути до работающего (этого) скрипта
    myPath = os.path.realpath(__file__)
    # print(f'testOne Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    # Получение абсолютного пути до каталога с проектом на EO
    # Пока это внутренняя песочница
    resultDir = '../../../result/'
    resultDir = os.path.abspath(resultDir)

    # Получение списка каталогов в каждом из которых содержится один тест
    testedDirNameList = os.listdir('./tests')
    testedDirNameList.sort()
    print(testedDirNameList)

    # Начальная установка генератора алиасов для файла со всеми резульатами программы на C
    cTestAliasName = '000'

    os.chdir('../../bin')
    os.system('python3 eo_version_update.py') # Update EO version in pom.xml
    os.system('cmake ..') # Rebuild project
    os.system('make')
    os.chdir('../tests/any')

    # Первоначально происходит компиляция и выполнение всех тестов на C
    # с сохранением результатов в локальных файлах.
    testCount = 0  # счетчик тестов
    сResultList = []  # список строк с результатами тестирования
    for testedDirName in testedDirNameList:
        testedDir = tmpDir + '/tests/' + testedDirName + '/'

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

        # Удаление содержимого временного каталога
        os.system('rm -r quickResults/*')
        # Удаление содержимого песочницы
        os.system('rm -r result/eo/c2eo/src/*')

        cFileName = testedDir + testedDirName + '.c'
        outName = ' -o ' + testedDir + 'a.out'
        print(cFileName)
        print(outName)

        # Компиляция всех C-тестов
        value = os.system('clang ' + cFileName + outName)

        # Запуск всех C-тестов с перенаправлением результатов в cResult.txt
        value = os.system(testedDir + 'a.out > ' + testedDir + 'cResult.txt')

        # Запуск лаунчера с формированием множества файлов global.eo в тестовых каталогах (как и раньше)
        # Имя программы на C для EO должно совпадать с именем каталога плюс суффикс -eo
        cEoRelationName = 'tests/' + testedDirName + '/' + testedDirName + '-eo.c'
        cEoFileName = testedDir + testedDirName + '-eo.c'
        # Файл создается в любом случае, так как тест м.б. переписан
        print('Start splitter')
        os.system('./splitter.py ' + cFileName + ' ' + cEoFileName)

        # Далее запускается стартер
        launcherStart = '../../bin/launcher.py ' + cEoRelationName
        print('Start launcher')
        os.system(launcherStart)

        os.chdir(resultDir)
        os.system('ls')
        os.system('pwd')

        # Копирование в тестовый каталог файла global.eo для возможного анализа
        globalFileDir = resultDir + '/eo/c2eo/src/'
        if os.path.exists(os.path.join(globalFileDir, 'global.eo')):
            shutil.copy(os.path.join(globalFileDir, 'global.eo'), testedDir)
        else:
            print(
                f'FAIL. File global.eo is absence in {globalFileDir} for test {testedDirName}')
            # Нет смысла продолжать дальше, так как тест не пройдет из-за отсутствия global.eo
            # хотя бы для одного теста. Нужно смотреть ошибку. Она фатальная и убивает быстрый тест
            os.chdir(currentDir)
            exit(-1)

        os.chdir(tmpDir)

    # Формирование в отдельных подкаталогах пакетов с тестовым кодом на EO
    for nameEo in testedDirNameList:
        # Открытие и редактирование очередного файла на EO
        testedEoFile = open(tmpDir + '/tests/' + nameEo + '/global.eo', 'r')
        # Чтение и модификация первой строки.
        tmpStr = testedEoFile.readline()
        eoAllCode = tmpStr[:-1] + '.' + nameEo + '\n'
        eoCodeStr = testedEoFile.readline()
        while eoCodeStr:
            eoAllCode += eoCodeStr
            eoCodeStr = testedEoFile.readline()
        # Добавление в конец кода запуска теста
        eoAllCode += app
        testedEoFile.close()

        # Занесение сформированного файла в свой тестовый подкаталог
        # dirName = tmpDir + '/quickResults/' + nameEo
        # os.mkdir(dirName)
        # globTestFileName = dirName + '/global.eo'
        # testEo = open(globTestFileName, 'w')
        # testEo.write(eoAllCode)
        # testEo.close()

        # Передача сформированного каталога пока в песочницу
        newDir = 'result/eo/c2eo/src/' + nameEo
        os.mkdir(newDir)
        resultEo = open(newDir + '/global.eo', 'w')
        resultEo.write(eoAllCode)
        resultEo.close()

    # Запуск сформированного кода на компиляцию и выполнение
    resultDirSandbox = 'result/'
    os.chdir(resultDirSandbox)
    eoMavenCode = os.system('mvn clean compile')
    # print('eoMavenCode = ', eoMavenCode)
    if eoMavenCode != 0:
        # Если maven не проходит глобальную компиляцию, то это фатально.
        print('Incorrect Maven Project Assembly')
        os.chdir(currentDir)
        exit(-4)

    # Запуск программы на EO с переносом результата в файл для сравнения
    for nameEo in testedDirNameList:
        eoRunCode = os.system(
            './run-one.sh  c2eo.src.' + nameEo + '.app > ' + tmpDir + '/tests/' + nameEo + '/eoResult.txt')
        # print('eoRunCode = ', eoRunCode)
        if eoRunCode != 0:
            print(f'Incorrect EO runtime for test {nameEo}')
            os.chdir(currentDir)
            exit(-3)

    # Сравнение результатов в тестовых файлах только если все они сформировались.
    stepCount = 0
    passCount = 0
    for nameEo in testedDirNameList:
        print(f'{nameEo}:')
        if not testDataCompare(tmpDir + '/tests/' + nameEo):
            passCount += 1
        stepCount += 1

    # print(сResultList)

    # Формирование окончетельного отчета
    print(f'{passCount} tests out of {stepCount} passed.')

    # Формирование времени выполнения сценария
    end_time: float = time.monotonic()
    delta = timedelta(seconds=end_time - start_time)
    print(f'testQuick execution time is {delta}')
