// Файл util.h с реализацией вспомогатеьлных утилит

#include "util.h"

// Запись строки символов в указанный файл
void str2file(std::string &str, std::string fileName) {
    std::ofstream out;          // поток для записи
    out.open(fileName); // окрываем файл для записи
    if (out.is_open()) {
        out << str;
    }
}

// Чтение из файла в вектор строк
void file2vector(std::string fileName, std::vector<std::string> &text) {
    std::ifstream in;          // поток для xntybz
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
void createGlobal(std::vector<std::string> &text) {
    // Создается заголовок, определяющий глобальный объект
    text.push_back(
        "+package c2eo\n\n"
        "+alias varint c2eo.varInt\n"
        "+alias varfloat c2eo.varFloat\n\n"

        "+alias sprintf org.eolang.txt.sprintf\n"
        "+alias stdout org.eolang.io.stdout\n\n"

        "[arg] > global\n"
    );
    // Читаются сформированные глобальные объекты
    file2vector("glob.global", text);
    file2vector("glob.debug", text);
    // Формируется начало последовательности инициализаций
    text.push_back("  seq > @");
    // Читаются инициализации объектов
    file2vector("glob.seq", text);
}

// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName) {
    std::ofstream out;          // поток для записи
    out.open(fileName); // окрываем файл для записи
    if (out.is_open()) {
        for(auto line: text) {
            if (!line.empty())
                out << line << "\n";
        }
    }
}

