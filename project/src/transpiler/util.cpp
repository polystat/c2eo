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

// Формирование строк для файла с глобальными объектами
// Пока формируется только для одной единицы компиляции
// В дальнейшем нужно будет собирать множество разных файлов с одинаковыми расширениями.
void createGlobal(std::vector<std::string> &text, std::string filename) {
    // Создается заголовок, определяющий глобальный объект
    text.push_back( R""""(+package c2eo

+alias c2eo.ctypes.c_bool
+alias c2eo.ctypes.c_char
+alias c2eo.ctypes.c_float32
+alias c2eo.ctypes.c_float64
+alias c2eo.ctypes.c_int16
+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_int64
+alias c2eo.ctypes.c_uint16
+alias c2eo.ctypes.c_uint32
+alias c2eo.ctypes.c_uint64

[arg] > global
)""""
    );
    // Читаются сформированные глобальные объекты
    file2vector(filename+".glob", text);
    // Формируется начало последовательности инициализаций
//!    text.push_back("\n  seq > @");
    // Читаются инициализации объектов
//!    file2vector(filename+".glob.seq", text);
}

// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName) {
    std::ofstream out;          // поток для записи
    out.open(fileName); // окрываем файл для записи
    if (out.is_open()) {
        for(auto line: text) {
            out << line << "\n";
        }
    }
}

void createStatic(std::vector<std::string> &text, std::string filename) {
    // Создается заголовок, определяющий статический объект
    text.push_back( R""""(+package c2eo

+alias c2eo.ctypes.c_bool
+alias c2eo.ctypes.c_char
+alias c2eo.ctypes.c_float32
+alias c2eo.ctypes.c_float64
+alias c2eo.ctypes.c_int16
+alias c2eo.ctypes.c_int32
+alias c2eo.ctypes.c_int64
+alias c2eo.ctypes.c_uint16
+alias c2eo.ctypes.c_uint32
+alias c2eo.ctypes.c_uint64

[arg] > )""""+filename+ "\n");
    // Читаются сформированные статические объекты
    file2vector(filename+".stat", text);
    // Формируется начало последовательности инициализаций
//!    text.push_back("\n  seq > @");
    // Читаются инициализации объектов
//!    file2vector(filename+".stat.seq", text);

}
std::string str_join(std::vector<std::string> vec) {
  if(vec.empty())
    return "";
  if(vec.size()==1)
    return vec[0];
  string res{vec[0]};
  for (int i = 1; i < vec.size(); ++i) {
    res += " " + vec[i];
  }
  return res;
}

