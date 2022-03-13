#ifndef __UTIL__
#define __UTIL__

// Заголовочный файл с описанием вспомогательных утилит
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string str_join(std::vector<std::string> vec);

// Запись строки символов в указанный файл
void str2file(std::string str, std::string fileName);

// Чтение из файла в вектор строк
void file2vector(std::string fileName, std::vector<std::string> &text);

// Формирование текста с глобальными объектами
// Пока формируется только для одной единицы компиляции
// В дальнейшем нужно будет собирать множество разных файлов с одинаковыми расширениями.
void createGlobal(std::vector<std::string> &text, std::string filename);

// Формирование текста со статическими переменными
// Генерируется файл package_name.eo
void createStatic(std::vector<std::string> &text, std::string filename);



// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName);

#endif // __UTIL__
