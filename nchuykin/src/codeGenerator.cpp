#include <sstream>
#include <llvm/Support/raw_ostream.h>
#include <fstream>
#include "codeGenerator.h"


std::vector<Variable*> CodeGenerator::vars = std::vector<Variable*>();

void CodeGenerator::getCodeToConsole() {
    llvm::outs() << getCode();
}

std::string CodeGenerator::getCode() {
    std::stringstream ss;
    ss << "+package c2eo\n\n";
    ss <<  "+alias varint c2eo.varInt\n";
    ss <<  "+alias varint c2eo.varFloat\n\n";

    ss <<  "[arg] > filenameC\n";
    for (Variable* v : vars)
    {
        ss <<  "  " << v->type <<" < " << v->name << "!\n";
    }
    ss<<"\n";

    ss <<  "  [arg] > main\n";
    ss <<  "    \"now main is empty\" > @\n\n";

    ss <<  "  seq > @\n";
    for (Variable* v : vars)
    {
        ss <<  "    " << v->name <<".set " << v->getValue() << "\n";
    }
    ss <<  "\n    main arg\n";
    ss <<  "[args...] > app\n";
    ss <<  "  seq > @\n";
    ss <<  "    filenameC args\n";
    return ss.str();
}

void CodeGenerator::getCodeToFile(const char* filename) {
    std::ofstream out(filename);
    out << getCode();
    out.close();

}

CodeGenerator::~CodeGenerator() {
    for (Variable* v : vars)
    {
        delete v;
    }
}

std::string IntVariable::getValue() {
    std::stringstream  ss;
    ss << value;
    return ss.str();
}
