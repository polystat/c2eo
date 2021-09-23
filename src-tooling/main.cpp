#include "visitors.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Incorrect command line format. Necessary: recvisitor <C-file-name>\n";
        return -1;
    }
    // Компилируемый модуль читается из файла в строку
    std::string fileName{argv[1]};
    std::ifstream moduleStream(fileName);
    if(moduleStream.fail()) {
        std::cout << "Module" << fileName << " isn't accesible" << std::endl;
        return -2;
    }
    std::stringstream ss;
    ss << moduleStream.rdbuf();
    std::string cModule(ss.str()); // Модуль языка Си (в перспективе)
    /// Тестовый вывод прочитанного артефакта
    std::cout << cModule << std::endl;
    clang::tooling::runToolOnCode(std::make_unique<Action>(), cModule);
    return 0;
}
