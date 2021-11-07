#include <string>
#include <numeric>
#include "llvm/Support/Casting.h"

#include "generator.h"


// void VarGen::GenValue(std::string &str) {
//     str = name;
//     str << ".write ";
//     str << value;
// }


SpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::staticSpaceGenPtr = nullptr;
std::string AbstractGen::filename;
std::map<uint64_t ,std::string> AbstractGen::identifiers = std::map<uint64_t,std::string>();
int AbstractGen::shift = 0;

SourceGen::~SourceGen() {
    delete glob;
    delete stat;
}


std::ostream& operator<<(AbstractGen& generator, std::ostream & out)
{
    generator.Generate(out);
    return out;
}

//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::ostream &out) {
    out << type << " " << value << " > " << name;
}

//--------------------------------------------------------------------------------------------------
/// shift is indentation level. shift 2 is equal to 4 spaces
std::string StmtGen::getIndentSpaces(int shift) {

    return std::string(' ',shift*2);
}

//--------------------------------------------------------------------------------------------------
void MultiLineStmtGen::Add(StmtGen *stmt) {
    statements.push_back(stmt);
}

void MultiLineStmtGen::Generate(std::ostream &out) {
    int lines = 0;

    for (int i = 0; i < statements.size(); ++i) {
        out << getIndentSpaces(AbstractGen::shift);
        out << statements[i];
        if (i+1 != statements.size() || llvm::isa<EmptyStmtGen>(statements[i]))
        {
            out << "\n";
            lines++;
        }
    }

    if(!lines) {
        //TODO учесть кол-во пробелов
        out << "    TRUE";
    }
}

MultiLineStmtGen::~MultiLineStmtGen() {
    statements.clear();
}


void CompoundStmtGen::Generate(std::ostream &out) {
    out << value;
    out << "\n";
    AbstractGen::shift++;
    out << nested;
    AbstractGen::shift--;
}

//--------------------------------------------------------------------------------------------------
void FuncGen::Generate(std::ostream &out) {
    // Первоначально осуществляется генерация списка атрибутов
    out << "[";
    if(name == "main") {
        out << "arg] > main\n";
    } else {
        for(const auto& paramName: paramNames) {
            out << paramName;
            out << ", ";
        }
        out << "ret_param_xxxx] > ";
        out << name;
        out << "\n";
    }
    // Далее идет формирование тела функции
    out << getIndentSpaces(AbstractGen::shift);
    AbstractGen::shift++;
    out << body;
    AbstractGen::shift--;
    out << "\n";
    if(name == "main") {
        out << "main arg > @\n";
    }
}

void BinaryStmtGen::Generate(std::ostream &out) {
    //str << value +"(";
    bool leftLinear = isLeftLinear(left);
    if (!leftLinear)
        out <<"(";
    out << left;
    if (!leftLinear)
        out <<")";
    out << value + "(";
    out << right;
    out << ")";

}

BinaryStmtGen::~BinaryStmtGen() {
    delete BinaryStmtGen::left;
    delete BinaryStmtGen::right;
}

bool BinaryStmtGen::isLeftLinear(StmtGen *pGen) {
    //Если когда-нибудь у UnaryStmtGen появятся наследники, то реализовать что-то подобное https://xakep.ru/2019/03/14/cpp-magic/#toc04
    //На данный момент проверка заточена на этот класd
    if  (pGen == nullptr || llvm::isa<UnaryStmtGen>(pGen))
        return false;
    auto* unaryStmtGen = static_cast<UnaryStmtGen*> (pGen);
    if (unaryStmtGen->nestedStmt != nullptr)
        return isLeftLinear(unaryStmtGen->nestedStmt);
    return true;
}


UnaryStmtGen::~UnaryStmtGen() {
    delete UnaryStmtGen::nestedStmt;
}

void UnaryStmtGen::Generate(std::ostream &out) {
    bool empty = value.empty() || nestedStmt == nullptr;
    out << value;
    if (!empty)
        out << "(";
    if (nestedStmt != nullptr)
        nestedStmt->Generate(out);
    out << postfix;
    if (!empty)
        out << ")";
}

//--------------------------------------------------------------------------------------------------
void SpaceGen::Generate(std::ostream &out) {
    // Формирование списка глобальных объектов
    //
    for(auto globalObject: objects) {
        out << globalObject;
        out << "\n";
    }

}

//........................................................................
// Добавление очередного объекта к глобальному пространству
void SpaceGen::Add(AbstractGen* obj) {
    objects.push_back(obj);
}

SpaceGen::~SpaceGen() {
    objects.clear();
}
