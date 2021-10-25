#! /usr/bin/python3
import os
import sys

# Установка комментариев на строки препроцессора и printf
if __name__ == '__main__':
    argv = sys.argv
    cFile =  open(argv[1], "r")
    cEoFile =  open(argv[2], "w")

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
