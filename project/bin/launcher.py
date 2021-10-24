#! /usr/bin/python3
# launcher.py - запуск транспилятора и сборщика, очистка промежуточных хранилищ.

#------------------------------------------------------------------------------
import os
import sys
import glob
import shutil

#------------------------------------------------------------------------------
# Каталог для размещения файлов на EO, полученных в ходе транспиляции и сборки
# Указан относительно текущего каталога
resultRelDir = "../../result/eo/c2eo/src/"

#------------------------------------------------------------------------------
# Каталог в котором формируется файл global.eo для последующей пересылки в
# каталог проекта на EO
assemblyRelDir = "../assembly/"

#------------------------------------------------------------------------------
if __name__ == '__main__':
    argc = len(sys.argv)
    print(f'argc = {argc}')
    argv = sys.argv
    print(f'argv = {argv}')
 
    # Получение текущего  рабочего каталога
    currentDir = os.getcwd()
    ##print(f'Current Working Directory is: {currentDir}')

    # Получение абсолютного пути до аргумента командной строки стартера
    startDir = os.path.abspath(currentDir)
    ##print(f'Start Working Directory is: {startDir}')
    # Получение пути до аргумента передаваемого для анализа
    argPath = startDir + '/' + argv[1]
    ##print(f'Argument Path: {argPath}')
 
    # Получение пути работающего скрипта (надеюсь, что данного)
    launcherPath = os.path.realpath(__file__)
    print(f'Launcher Directory is: {launcherPath}')
    tmpDir = launcherPath if os.path.isdir(launcherPath) else os.path.dirname(launcherPath)
    ##print(f'Tmp Directory is: {tmpDir}')
    # Изменение рабочего каталога
    os.chdir(tmpDir)
 
    # Получение абсолютного пути до каталога src
    resultDir = os.path.abspath(resultRelDir)
    ##print(f'Result Directory is: {resultDir}')
 
 
    # Получение абсолютного пути до каталога assembly
    assemblyDir = os.path.abspath(assemblyRelDir)
    ##print(f'Assembly Directory is: {assemblyDir}')
 
    # Проверка наличия нужного каталога
    ##if os.path.exists(resultDir):
    ##    print(f'Result Directory is: {resultDir}')

    # Получение содержимого каталога
    ##print(f'Directory {resultDir} contain: {os.listdir(resultDir)}')
    ##print(f'Directory {assemblyDir} contain: {os.listdir(assemblyDir)}')
    resultFileList = list(glob.glob(os.path.join(resultDir, '*.eo')))
    print(f'Directory {resultDir} contain: {resultFileList}')
    assemlyFileList = list(glob.glob(os.path.join(assemblyDir, '*.c')))
    print(f'Directory {assemblyDir} contain: {assemlyFileList}')

    # Удаление всех файлов, находящихся в каталоге assemblyDir
    for file in list(glob.glob(os.path.join(assemblyDir, '*'))):
        os.remove(file)
    # Запуск транспилятора

    if argc < 2:
        print(f'Incorrect number of argument = {argc}')
        exit(1)

    c2eoProgram =  os.getcwd() + '/c2eo ' + argPath + ' static01'
    print(f'c2eoProgram = {c2eoProgram}')


    os.system(c2eoProgram)
    #os.startfile(c2eoProgram)

    #for file in list(glob.glob(os.path.join(assemblyDir, '*.c'))):
    #    shutil.move(file, resultDir)

    # Запуск сборщика
    collectorScript = os.getcwd() + '/collector.py'
    os.system(collectorScript)
    #os.startfile(r'./collector')

   # Удаление всех файлов, находящихся в каталоге resultDir
    for file in list(glob.glob(os.path.join(resultDir, '*'))):
        os.remove(file)

    # Проверка наличия файла global.eo в каталоге assembly
    if os.path.exists(os.path.join(assemblyDir, 'global.eo')):
        shutil.move(os.path.join(assemblyDir, 'global.eo'), resultDir)
    else:
        print(f'File global.eo is absence in: {assemblyDir}')
        exit(2)

    print('Launcher: OK!')

