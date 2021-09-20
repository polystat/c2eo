// Функции, используемые при анализе переменных

#include "vardecl.h"
#include "codeGenerator.h"

// Определение и тестовый вывод основных параметров описания переменных
void getVarDeclParameters(const VarDecl *VD) {
    // Имя переменной
    auto varName = VD->getNameAsString();
    llvm::outs() << "Name of Variable: " << varName << "\n";

    // Проверка на размещение переменной в локальной памяти
    auto inLocalStorage = VD->hasLocalStorage();
    if(inLocalStorage) {
        llvm::outs() << "  Local Storage\n";
    } else {
        llvm::outs() << "  not Local Storage\n";
    }

    // Проверка на статическую локальную переменную
    auto staticLocal = VD->isStaticLocal();
    if(staticLocal) {
        llvm::outs() << "  Static Local\n";
    } else {
        llvm::outs() << "  not Static Local\n";
    }

    // Внешняя переменная (описатель external)
    auto extStorage = VD->hasExternalStorage();
    if(extStorage) {
        llvm::outs() << "  External Storage\n";
    } else {
        llvm::outs() << "  not External Storage\n";
    }

    // Размещение переменной в глобальной памяти
    // Касается глобальных и статических переменных
    auto globalStorage = VD->hasGlobalStorage();
    if(globalStorage) {
        llvm::outs() << "  has GlobalStorage\n";
    } else {
        llvm::outs() << "  has not GlobalStorage\n";
    }

    // Переменная с локальной видимостью
    auto localVarDecl = VD->isLocalVarDecl();
    if(localVarDecl) {
        llvm::outs() << "  is Local VarDecl\n";
    } else {
        llvm::outs() << "  is not Local VarDecl\n";
    }

    // Переменная или параметр с локальной видимостью
    auto localVarDeclOrParm = VD->isLocalVarDeclOrParm();
    if(localVarDeclOrParm) {
        llvm::outs() << "  is Local VarDecl or Parm\n";
    } else {
        llvm::outs() << "  is not Local VarDecl or Parm\n";
    }

    // Наличие начальной инициализации
    auto isInit = VD->hasInit();
    int value = 0;
    if(isInit) {
        llvm::outs() << "  has Initializer\n";
        initValueAnalysis(VD, value);
    } else {
        llvm::outs() << "  has not Initializer\n";
    }

    if (globalStorage && !extStorage && !staticLocal)
    {
        IntVariable* var = new IntVariable;
        var->name = "c_" + varName;
        var->type = "varint";
        var->value  = value;
        CodeGenerator::vars.push_back(var);
    }
  //VD->dump();
}

// Анализ полученного начального значения с передачей значения по ссылке
void initValueAnalysis(const VarDecl *VD, int& value) {
    APValue *initVal = VD->evaluateValue();
    if(initVal != nullptr) {
        llvm::outs() << "    Initial Value = ";
        if(initVal->isInt()) {
            value = initVal->getInt().getExtValue();
            llvm::outs() << value;
        }
        llvm::outs() << "\n";
    } else {
        llvm::outs() << "    no Initial Value\n";
    }
}

