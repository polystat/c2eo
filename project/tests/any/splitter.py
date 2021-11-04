#! /usr/bin/python3
import os
import sys

# Установка комментариев на строки препроцессора и printf
if __name__ == '__main__':
    argv = sys.argv
    cFile =  open(argv[1], "r")
    cEoFile =  open(argv[2], "w")

    # Получение текущего  рабочего каталога
    currentDir = os.getcwd()

    # Получение пути до работающего скрипта
    myPath = os.path.realpath(__file__)
    #print(f'Splitter Directory is: {myPath}')
    tmpDir = myPath if os.path.isdir(myPath) else os.path.dirname(myPath)
    # Изменение рабочего каталога на каталог скрипта
    os.chdir(tmpDir)

    cLine = cFile.readline()
    while cLine:
        words = cLine.split()
        print(words)
        if words != []:
            if (words[0][0] == '#') or words[0].find('printf') != -1:
                cEoFile.write('// ' + cLine)
            else:
                cEoFile.write(cLine)
        cLine = cFile.readline()

    cFile.close()
    cEoFile.close()

    # Изменение рабочего каталога на текущий
    os.chdir(currentDir)
