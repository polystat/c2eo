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

# Фрагмент данных с метаинформацией и началом глобального объекта
meta = '''+package c2eo.src
+alias stdout org.eolang.io.stdout
+alias sprintf org.eolang.txt.sprintf

+alias c2eo.ctypes.c_bool
+alias c2eo.ctypes.c_char
+alias c2eo.ctypes.c_float64
+alias c2eo.ctypes.c_int16
+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_int64

[arg] > global

'''

#------------------------------------------------------------------------------
if __name__ == '__main__':
    print("Hello from collector!!!!")

    # Получение текущего каталога
    print(f'Current Working Directory is: {os.getcwd()}')
    # Проверка наличия нужного каталога
    if os.path.exists(assemblyDir):
        print(f'Resul Directory is: {assemblyDir}')

    # Получение содержимого каталога
    #print(f'Directory {assemblyDir} contain: {os.listdir(assemblyDir)}')
    assemlyStaticFileList = list(glob.glob(os.path.join(assemblyDir, '*.stat')))
    print(f'Static objects. Directory {assemblyDir} contain files: {assemlyStaticFileList}')
    assemlyGlobalFileList = list(glob.glob(os.path.join(assemblyDir, '*.glob')))
    print(f'Global objects. Directory {assemblyDir} contain files: {assemlyGlobalFileList}')

    # Сборка файла global.eo
    # Формирование всех глобальных объектов
    collectInfo = meta
    for globalFile in assemlyGlobalFileList:
        with open(globalFile, "r") as f:
            for line in f:
                #print(line)
                if line != '\n':
                    collectInfo += '  '
                collectInfo +=line

    # Добавление в файл всех статических объектов
    for staticFile in assemlyStaticFileList:
        # Получение имени файла для использования в качестве имени статического объекта
        name = os.path.basename(staticFile)
        name = os.path.splitext(name)[0]
        print(f'Static object name = {name}')
        collectInfo += '\n  [] > ' + name + '\n'
        with open(staticFile, "r") as f:
            for line in f:
                print(line)
                if line != '\n':
                    collectInfo += '    '
                collectInfo +=line


    print(collectInfo)
    with open("../assembly/global.eo", "w") as f:
        f.write(collectInfo)

