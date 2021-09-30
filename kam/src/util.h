#ifndef __UTIL__
#define __UTIL__

// Заголовочный файл с описанием вспомогательных утилит
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
/*
    Declares clang::SyntaxOnlyAction.
    #include "clang/Frontend/FrontendActions.h"
    #include "clang/Tooling/CommonOptionsParser.h"
    #include "clang/Tooling/Tooling.h"

    Declares llvm::cl::extrahelp.
    #include "llvm/Support/CommandLine.h"

    #include "clang/ASTMatchers/ASTMatchers.h"
    #include "clang/ASTMatchers/ASTMatchFinder.h"

    #include "clang/AST/ASTContext.h"

    using namespace clang::tooling;
    using namespace llvm;
    using namespace clang;
    using namespace clang::ast_matchers;


    using namespace clang;
*/
// Запись строки символов в указанный файл
void str2file(std::string &str, std::string fileName);

// Чтение из файла в вектор строк
void file2vector(std::string fileName, std::vector<std::string> &text);

// Формирование текста с глобальными объектами
// Пока формируется только для одной единицы компиляции
// В дальнейшем нужно будет собирать множество разных файлов с одинаковыми расширениями.
void createGlobal(std::vector<std::string> &text);

// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName);

#endif // __UTIL__
