#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <numeric>


#include "generator.h"


//--------------------------------------------------------------------------------------------------

SpaceGen* AbstractGen::globalSpaceGenPtr = nullptr;
SpaceGen* AbstractGen::staticSpaceGenPtr = nullptr;
std::string AbstractGen::filename = "";
std::map<uint64_t ,std::string> AbstractGen::identifiers = std::map<uint64_t,std::string>();



//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::string &str) {
    str = type + " ";
    str += value;
    str += " > ";
    str += name;
}

// void VarGen::GenValue(std::string &str) {
//     str = name;
//     str += ".write ";
//     str += value;
// }

//--------------------------------------------------------------------------------------------------
void GlobalFuncGen::Generate(std::string &str) {
    // Первоначально осуществляется генерация списка атрибутов
    str = "[";
    if(name == "main") {
        str += "arg] > main\n";
    } else {
        for(auto paramName: paramNames) {
            str += paramName;
            str += ", ";
        }
        str += "ret_param_xxxx] > ";
        str += name;
        str += "\n";
    }
    // Далее идет формирование тела функции
    str += getIndentSpaces(body->shift);
    body->Generate(str);
    str += "\n";
    if(name == "main") {
        str += "main arg > @\n";
    }
}

void GlobalFuncGen::GenValue(std::string &str) {
    if(name == "main") {
        str += "\n    main arg\n";
    }
}

//--------------------------------------------------------------------------------------------------
void SpaceGen::Generate(std::string &str) {
    str = "";
    // Формирование списка глобальных объектов
    //
    for(auto globalObject: objects) {
        std::string strObj = "";
        globalObject->Generate(strObj);
        str += strObj;
        str += "\n";
    }

}

void SpaceGen::GenValue(std::string &str) {
    // Формирование списка инициализаций
    for(auto globalObject: objects) {
        std::string strInit = "";
        globalObject->GenValue(strInit);
        str += "    ";
        str += strInit;
        str += "\n";
    }
    //str += "\n";
}

//........................................................................
// Добавление очередного объекта к глобальному пространству
void SpaceGen::Add(AbstractGen* obj) {
    objects.push_back(obj);
}

SpaceGen::~SpaceGen() {
    objects.clear();
}

//--------------------------------------------------------------------------------------------------
void ApplicationGen::Generate(std::string &str) {
    str = R""""()"""";
}

//--------------------------------------------------------------------------------------------------
void FullGen::Generate(std::string &str) {
    
}


void CompoundStmtGen::Generate(std::string &str) {
    str += value;
    str += "\n";
    std::vector<std::string> lines;
    for (auto stmt : statements)
    {
        std::string strobj = "";
        strobj += getIndentSpaces(stmt->shift);
        stmt->Generate(strobj);
        lines.push_back(strobj);
    }
///!!! Косяк где-то в этом коде.
    if(lines.size() != 0) {
        std::string res = std::accumulate(
                std::next(lines.begin()), lines.end(),
                lines[0],
                [](std::string a, std::string b) {
                    return a + "\n" + b;
                }
        );
        str += res;
    } else {
        str += "    TRUE";
    }
   //  lines[0];

}

CompoundStmtGen::~CompoundStmtGen() {
    CompoundStmtGen::statements.clear();
}

void CompoundStmtGen::Add(StmtGen* stmtGen) {
    CompoundStmtGen::statements.push_back(stmtGen);
}

std::string StmtGen::getIndentSpaces(int shift) {
    std::string res = "";
    for (int i = 0; i < shift; ++i) {
        res += "  ";
    }
    return res;
}

void BinaryStmtGen::Generate(std::string &str) {
    //str += value +"(";
    bool leftLinear = isLeftLinear(left);
    if (!leftLinear)
        str +="(";
    left->Generate(str);
    if (!leftLinear)
        str +=")";
    str += value +"(";
    right->Generate(str);
    str += ")";

}

BinaryStmtGen::~BinaryStmtGen() {
    delete BinaryStmtGen::left;
    delete BinaryStmtGen::right;
}



bool BinaryStmtGen::isLeftLinear(StmtGen *pGen) {
    //Если когда-нибудь у UnaryStmtGen появятся наследники, то реализовать что-то подобное https://xakep.ru/2019/03/14/cpp-magic/#toc04
    //На данный момент проверка заточена на этот класd
    if  (pGen == nullptr || !pGen->is_unary())
        return false;
    UnaryStmtGen* unaryStmtGen = static_cast<UnaryStmtGen*> (pGen);
    if (unaryStmtGen->nestedStmt != nullptr)
        return isLeftLinear(unaryStmtGen->nestedStmt);
    return true;
}

UnaryStmtGen::~UnaryStmtGen() {
    delete UnaryStmtGen::nestedStmt;
}

void UnaryStmtGen::Generate(std::string &str) {
    bool empty = value.empty() || nestedStmt == nullptr;
    str += value;
    if (!empty)
        str += "(";
    if (nestedStmt != nullptr)
        nestedStmt->Generate(str);
    str += postfix;
    if (!empty)
        str += ")";
}
