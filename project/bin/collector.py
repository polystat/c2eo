#! /usr/bin/python3
# collector.py - запуск транспилятора и сборщика, очистка промежуточных хранилищ.

#------------------------------------------------------------------------------
import os
import glob
import shutil

#------------------------------------------------------------------------------
# Каталог для размещения файлов на EO, полученных в ходе транспиляции и сборки
# Указан относительно текущего каталога
resultDir = "../../result/eo/c2eo/src/"

#------------------------------------------------------------------------------
# Каталог в котором формируется файл global.eo для последующей пересылки в
# каталог проекта на EO
assemblyDir = "../assembly/"

#------------------------------------------------------------------------------
if __name__ == '__main__':
    print("Hello from collector!!!!")

    # Получение текущего каталога
    print(f'Current Working Directory is: {os.getcwd()}')
    # Проверка наличия нужного каталога
    if os.path.exists(assemblyDir):
        print(f'Resul Directory is: {assemblyDir}')

    # Получение содержимого каталога
    print(f'Directory {assemblyDir} contain: {os.listdir(assemblyDir)}')
    assemlyStaticFileList = list(glob.glob(os.path.join(assemblyDir, '*.stat')))
    print(f'Directory {assemblyDir} contain: {assemlyStaticFileList}')
    assemlyGlobalFileList = list(glob.glob(os.path.join(assemblyDir, '*.stat')))
    print(f'Directory {assemblyDir} contain: {assemlyGlobalFileList}')

    # Начало сборки файла global.eo

    # Получение имени файла для обозначения статического объекта
    # Делается обход по списку файлов, содержащих информацию о статических объектах
    ##name = os.path.basename(r'C:\python3\file.txt ')
    ##name = os.path.splitext("/path/to/some/file.txt")[0]
