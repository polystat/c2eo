// Файл util.h с реализацией вспомогательных утилит

#include "util.h"

using namespace std;

// Запись строки символов в указанный файл
void str2file(std::string str, std::string fileName) {
  std::ofstream out;          // поток для записи
  out.open(fileName); // окрываем файл для записи
  if (out.is_open()) {
    out << str;
  }
}

// Чтение из файла в вектор строк
void file2vector(std::string fileName, std::vector<std::string> &text) {
  std::ifstream in;          // поток для чтения
  in.open(fileName); // окрываем файл для записи
  std::string line;
  if (in.is_open()) {
    while (getline(in, line)) {
      text.push_back(line);
    }
  }
}

// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName) {
  std::ofstream out;          // поток для записи
  out.open(fileName); // окрываем файл для записи
  if (out.is_open()) {
    for (auto line: text) {
      out << line << "\n";
    }
  }
}

std::string str_join(std::vector<std::string> vec) {
  if (vec.empty())
    return "";
  if (vec.size() == 1)
    return vec[0];
  string res{vec[0]};
  for (int i = 1; i < vec.size(); ++i) {
    res += " " + vec[i];
  }
  return res;
}

