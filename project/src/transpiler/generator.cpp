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
std::map <uint64_t, std::string> AbstractGen::identifiers = std::map<uint64_t, std::string>();
int AbstractGen::shift = 0;

SourceGen::~SourceGen() {
    delete glob;
    delete stat;
}


//--------------------------------------------------------------------------------------------------
void VarGen::Generate(std::ostream &out) {
    out << type << " " << value << nestedStmt << " > " << name;
}

//--------------------------------------------------------------------------------------------------
/// shift is indentation level. shift 2 is equal to 4 spaces
std::string StmtGen::getIndentSpaces() {

    return std::string(shift * 2, ' ');
}

//--------------------------------------------------------------------------------------------------
void MultiLineStmtGen::Add(StmtGen* stmt) {
    UnaryStmtGen* st = llvm::dyn_cast<UnaryStmtGen>(stmt);
    //TODO Вынести Empty как отдельный тип или метод
    if (st && st->op == "" && st->value == "" && st->nestedStmt == nullptr)
        return;
    statements.push_back(stmt);
}

void MultiLineStmtGen::Generate(std::ostream &out) {
    int lines = 0;

    for (int i = 0; i < statements.size(); ++i) {
        //TODO тревожный звоночек, что ObjectStmt это Multiline
        if (!llvm::isa<MultiLineStmtGen>(statements[i]) && !llvm::isa<ObjectStmtGen>(statements[i]))
            out << getIndentSpaces();
        out << statements[i];
        if (i + 1 != statements.size() || llvm::isa<EmptyStmtGen>(statements[i])) {
            out << "\n";
            lines++;
        }
    }


}

MultiLineStmtGen::~MultiLineStmtGen() {
    statements.clear();
}


void CompoundStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    out << "seq";
    if (is_decorator)
        out << " > @";
    out << "\n";
    AbstractGen::shift++;
    if (!statements.empty()) {
        MultiLineStmtGen::Generate(out);
        out << "\n";
    }
    out << getIndentSpaces();
    out << "TRUE";
    AbstractGen::shift--;
}

//--------------------------------------------------------------------------------------------------
void FuncGen::Generate(std::ostream &out) {
    // Первоначально осуществляется генерация списка атрибутов
    out << "[";
    if (name == "main") {
        out << "arg] > main\n";
    } else {
        for (const auto &paramName: paramNames) {
            out << paramName;
            out << " ";
        }
        out << "ret_param_xxxx] > ";
        out << name;
        out << "\n";
    }
    // Далее идет формирование тела функции

    AbstractGen::shift++;
    out << body;
    AbstractGen::shift--;
    out << "\n";
    if (name == "main") {
        out << "main arg > @\n";
    }
}

void BinaryStmtGen::Generate(std::ostream &out) {
    //str << value +"(";
    out << value << " ";
    //bool leftLinear = isLeftLinear(left);
    // lets add left brackets everytime
    bool leftLinear = false;
    if (!leftLinear)
        out << "(";
    out << left;
    if (!leftLinear)
        out << ")";
    out << " ";
    bool rightLiteral = llvm::isa<LiteralStmtGen>(right);
    if (!rightLiteral)
        out << "(";
    out << right;
    if (!rightLiteral)
        out << ")";

}

BinaryStmtGen::~BinaryStmtGen() {
    delete BinaryStmtGen::left;
    delete BinaryStmtGen::right;
}

bool BinaryStmtGen::isLeftLinear(StmtGen* pGen) {
    if (pGen == nullptr || !llvm::isa<UnaryStmtGen>(pGen))
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
    out << "(" << value;
    if (!empty)
        out << "(";
    if (nestedStmt != nullptr)
        nestedStmt->Generate(out);
    // Now we dont use dot operator and postfix is deprecated
    //out << postfix;
    if (!empty)
        out << ")";
    out << ")";
}

//--------------------------------------------------------------------------------------------------
void SpaceGen::Generate(std::ostream &out) {
    // Формирование списка глобальных объектов
    //
    for (auto globalObject: objects) {
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


void IfStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    out << "if\n";
    AbstractGen::shift++;
    MultiLineStmtGen::Generate(out);
    AbstractGen::shift--;
}

IfStmtGen::~IfStmtGen() {
}

//--------------------------------------------------------------------------------------------------
void RecordGen::Generate(std::ostream &out) {
    out << "[";
    out << "arr";
    out << "] > " << name;
    out << "\n";
    shift++;
    out << StmtGen::getIndentSpaces() << "\"" << type << "\" > type\n";
    for (size_t i = 0; i < fields.size(); i++) {
        out << "\n" << StmtGen::getIndentSpaces() << fields[i]->type << " ";
        out << "(arr.get " << i << ") ";
        out << "> " << fields[i]->name;
    }
    if (!fields.empty())
        out << "\n";
    out << "\n" << StmtGen::getIndentSpaces() << "[value] > write\n";
    shift++;
    out << StmtGen::getIndentSpaces() << "seq > @";
    shift++;
    for (RecordGen* vg: fields)
        out << "\n" << StmtGen::getIndentSpaces() << "^." << vg->name << ".write (value." << vg->name << ")";
    out << "\n" << StmtGen::getIndentSpaces() << "TRUE\n";
    shift -= 3;
}

void WhileStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    out << "while\n";
    AbstractGen::shift++;
    MultiLineStmtGen::Generate(out);
    AbstractGen::shift--;
}

WhileStmtGen::~WhileStmtGen() {

}

void LiteralStmtGen::Generate(std::ostream &out) {
    out << "(";
    out << value;
    out << ")";
}

//TODO универсвализировать, добавить параметры возможно наследовать от compgen
void ObjectStmtGen::Generate(std::ostream &out) {
    out << getIndentSpaces();
    //TODO убрать эти два пробела
    out << "[] > @\n";
    AbstractGen::shift++;
    body->Generate(out);
    AbstractGen::shift--;
}

ObjectStmtGen::~ObjectStmtGen() {

}

void DoWhileStmtGen::Generate(std::ostream &out) {
    StmtGen* s = nullptr;
    if (!llvm::isa<CompoundStmtGen>(statements[1]))
        out << getIndentSpaces();
    out << statements[1] << "\n";
    out << getIndentSpaces();
    out << "while\n";
    AbstractGen::shift++;
    MultiLineStmtGen::Generate(out);
    AbstractGen::shift--;
}

DoWhileStmtGen::~DoWhileStmtGen() {

}

void MemberStmtGen::Generate(std::ostream &out) {
    if (nestedStmt) {
        nestedStmt->Generate(out);
        out << ".";
    }
    out << value;
}

void ListStmtGen::Generate(std::ostream &out) {
    out << "(* ";
    for (size_t i = 0; i < elements.size(); i++) {
        if (i)
            out << " ";
        elements[i]->Generate(out);
    }
    out << ")";
}