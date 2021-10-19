// Функции, используемые при анализе переменных

#include "funcdecl.h"
#include "stmt.h"
#include "generator.h"

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод основных параметров описания функции
void getFuncDeclParameters(const FunctionDecl *FD) {
    // Имя переменной
    auto funcName = FD->getNameAsString();
    llvm::outs() << "Name of Function: " << funcName << "\n";
    llvm::outs() << "  Func Kind Name: " << FD->clang::Decl::getDeclKindName() << "\n";

    // Нужны только функции, имеющие тело
    auto hasBody = FD->hasBody();
    if(hasBody) {
        llvm::outs() << "  Has Body\n";
    } else {
        llvm::outs() << "  Dousn't have Body\n";
    }

    // Нужны только функции, имеющие тело
    auto hasTrivialBody = FD->hasTrivialBody();
    if(hasTrivialBody) {
        llvm::outs() << "  Has Trivial Body\n";
    } else {
        llvm::outs() << "  Does not have Trivial Body\n";
    }

    //auto body = FD->getBody();
    Stmt* body = FD->getBody();
    if(body != nullptr) {
        llvm::outs() << "  Body pointer != 0\n";
    } else {
        llvm::outs() << "  Body pointer == 0\n";
    }

    auto defined = FD->isDefined();
    if(defined) {
        llvm::outs() << "  Function is defined\n";
    } else {
        llvm::outs() << "  Function isn't defined\n";
    }

    /// Determines whether this is a global function.
    bool isGlobal = FD->isGlobal();
    if(isGlobal) {
        llvm::outs() << "  Function is global\n";
    } else {
        llvm::outs() << "  Function isn't global\n";
    }

    /// Determines whether this function is known to be 'noreturn', through
    /// an attribute on its declaration or its type.
    bool isNoReturn = FD->isNoReturn();
    if(isNoReturn) {
        llvm::outs() << "  Function does not return value\n";
    } else {
        llvm::outs() << "  Function returns value\n";
    }
/*
    bool isParam_empty = FD->param_empty();
    if(isParam_empty) {
        llvm::outs() << "  Function does not have parameters\n";
    } else {
        llvm::outs() << "  Function has parameters\n";
    }

    size_t paramSize = FD->param_size();
    llvm::outs() << "  Function has " << paramSize << " parameters\n";
*/
    unsigned numParams = FD->getNumParams();
    std::vector<std::string> paramNames;
    llvm::outs() << "  Function has " << numParams << " parameters\n";
    for(auto i = 0; i < numParams; i++) {
        llvm::outs() << "     Parameter " << i << ":\n";
        auto PVD = FD->getParamDecl(i);
        getParmVarDeclParameters(PVD);
        auto paramName = PVD->getNameAsString();
        paramNames.push_back(paramName);
    }

    // Определение типа возвращаемого значения
    QualType returnType = FD->getReturnType();
    auto typePtr = returnType.getTypePtr();   // указатель на тип (Type)
    if(typePtr->isBooleanType()) {
        llvm::outs() << "  Returns bool type\n";
    } else if(typePtr->isCharType()) {
        llvm::outs() << "  Returns char type\n";
    } else if (typePtr->isIntegerType()){
        llvm::outs() << "  Returns integer type\n";
    } else if(typePtr->isRealFloatingType()) {
        llvm::outs() << "  Returns real type\n";
    } else {
        llvm::outs() << "  Returns other type\n";
    }

    /// Determine whether the "inline" keyword was specified for this function.
   bool isInline = FD->isInlineSpecified();
    if(isInline) {
        llvm::outs() << "  Function is Inline\n";
    } else {
        llvm::outs() << "  Function isn't Inline\n";
    }

    // Обработка и анализ составного оператора
    if(body != nullptr) {
        char* bodyName = (char*)(body->getStmtClassName());
        //int bodyTag = (int)(body->getStmtClass());
        Stmt::StmtClass stmtClass = body->getStmtClass();
        if(bodyName != nullptr) {
            //llvm::outs() << "  It is " << bodyName << " statement. Tag = " << bodyTag << "\n";
            llvm::outs() << "  It is " << bodyName << " statement. stmtClass = " << stmtClass << "\n";
        } else {
            llvm::outs() << "  Incorrect body name identification\n";
        }
        // Далее должна начаться обработка составного оператора;
        // вложенного в функцию
//        if(bodyTag == 8) { // Нужно разобраться с именами перчислимых типов
        if(stmtClass == Stmt::CompoundStmtClass) { // Нужно разобраться с именами перчислимых типов
            getCompoundStmtParameters(static_cast<CompoundStmt*>(body));
        }
    }
    //TODO расскомментировать эти строки и добавить статику
    // Формируется глобальная функция со всеми атрибутами
    /*
    if(isGlobal) {
        GlobalFuncGen* func = new GlobalFuncGen;
        if(funcName == "main") {
            func->name = "main";
        } else {
            func->name = "g_" + funcName;
        }
        // Осуществляется перенос имен атрибутов в глобальную функцию
        for(auto paramName: paramNames) {
            func->paramNames.push_back(paramName);
        }
        func->globalSpaceGenPtr->Add(func);
    }*/

    //FD->dump();
}

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод основных формальных параметров
void getParmVarDeclParameters(const ParmVarDecl *PVD) {
    // Имя переменной
    auto varName = PVD->getNameAsString();
    llvm::outs() << "      Name of Variable: " << varName << "\n";
    llvm::outs() << "      Var Kind Name: " << PVD->getDeclKindName() << "\n";

    TypeInfo typeInfo = PVD->getASTContext().getTypeInfo(PVD->getType());
    auto typeSize = typeInfo.Width;
    auto fieldAlign = typeInfo.Align;
    llvm::outs() << "      Size = " << typeInfo.Width << ", Align = " << typeInfo.Align << "\n";

    // Получение типа переменной
    ///std::string typeName{"any value"};
    ///StringRef varTypeName(typeName);
    ////auto qualType = PVD->getType();
    QualType qualType = PVD->getType();
    const IdentifierInfo* typeId = qualType.getBaseTypeIdentifier();
    ////varTypeName = typeId->getName();
    ////llvm::outs() << "  Type is " << varTypeName << "\n";
    if(typeId != nullptr) {
        llvm::outs() << "      Type is " << typeId->getName() << "\n";
    } else {
        llvm::outs() << "      The problem to identify type name\n";
    }

    auto typePtr = qualType.getTypePtr();
    if(typePtr->isBuiltinType()) {
        llvm::outs() << "      It is Builtin type\n";
//         BuiltinType* btPtr = dynamic_cast<BuiltinType*>(typePtr);
//         if(btPtr != nullptr) {
//             llvm::outs() << "  It is Possible to convert into Builtin type\n";
//         } else {
//             llvm::outs() << "  It is Impossible to convert into Builtin type\n";
//         }
    } else {
        llvm::outs() << "      It is not Builtin type\n";
    }
    //auto kind = typePtr->getKind();

    std::string strType = "";

    if(typePtr->isCharType()) {
        if(typePtr->isSignedIntegerType()) {
            llvm::outs() << "        -->signedCharType\n";
        } else {
            llvm::outs() << "        -->unsignedCharType\n";
        }
        strType = "varchar";
    } else if(typePtr->isBooleanType()) {
        llvm::outs() << "        -->isBooleanType\n";
        strType = "varbool";
    } else if(typePtr->isRealFloatingType()) {
        llvm::outs() << "        -->isRealFloatingType\n";
        strType = "varfloat";
    } else if(typePtr->isIntegerType()) {
        if(typePtr->isSignedIntegerType())
            llvm::outs() << "        -->signedIntegerType\n";
        else
            llvm::outs() << "        -->unsignedIntegerType\n";
        strType = "varint";
    }

    llvm::outs() << "  !!! class name = " << typePtr->getTypeClassName() << "\n";

/*
    // StorageClass getStorageClass() const
    // Показывает на явное описани того или иного класса памяти в тексте программы
    // Наверное не во всех случаях полезно
    auto storageClass = PVD->getStorageClass();
    llvm::outs() << "  storage class is ";
    switch(storageClass) {
        case SC_None:           llvm::outs() << "None";          break;
        case SC_Extern:         llvm::outs() << "Extern";        break;
        case SC_Static:         llvm::outs() << "Static";        break;
        case SC_PrivateExtern:  llvm::outs() << "PrivateExtern"; break;
        case SC_Auto:           llvm::outs() << "Auto";          break;
        case SC_Register:       llvm::outs() << "Register";      break;
        default:                llvm::outs() << "NOT DEFINED";
    }
    llvm::outs() << "\n";

    // Проверка на размещение переменной в локальной памяти
    auto inLocalStorage = PVD->hasLocalStorage();
    if(inLocalStorage) {
        llvm::outs() << "  Local Storage\n";
    } else {
        llvm::outs() << "  not Local Storage\n";
    }
    // Проверка на статическую локальную переменную
    auto staticLocal = PVD->isStaticLocal();
    if(staticLocal) {
        llvm::outs() << "  Static Local\n";
    } else {
        llvm::outs() << "  not Static Local\n";
    }
    // Внешняя переменная (описатель external)
    auto extStorage = PVD->hasExternalStorage();
    if(extStorage) {
        llvm::outs() << "  External Storage Defenition\n";
    } else {
        llvm::outs() << "  not External Storage Definition\n";
    }
    // Размещение переменной в глобальной памяти
    // Касается глобальных и статических переменных
    auto globalStorage = PVD->hasGlobalStorage();
    if(globalStorage) {
        llvm::outs() << "  has GlobalStorage\n";
    } else {
        llvm::outs() << "  has not GlobalStorage\n";
    }
    // Переменная с локальной видимостью
    auto localVarDecl = PVD->isLocalVarDecl();
    if(localVarDecl) {
        llvm::outs() << "  is Local VarDecl\n";
    } else {
        llvm::outs() << "  is not Local VarDecl\n";
    }
    // Переменная или параметр с локальной видимостью
    auto localVarDeclOrParm = PVD->isLocalVarDeclOrParm();
    if(localVarDeclOrParm) {
        llvm::outs() << "  is Local VarDecl or Parm\n";
    } else {
        llvm::outs() << "  is not Local VarDecl or Parm\n";
    }
    // Наличие начальной инициализации
    auto isInit = PVD->hasInit();
    std::string strValue = "";
    if(isInit) {
        llvm::outs() << "  has Initializer\n";
        initValueAnalysis(PVD, strValue);
    } else {
        llvm::outs() << "  has not Initializer\n";
        initZeroValueAnalysis(PVD, strValue);
    }

    // Проверка, что переменная является глобальной
    if (globalStorage && !extStorage && !staticLocal)
    {
        // Формируется глобальная переменная со всеми атрибутами
        VarGen* var = new VarGen;

        var->name = "g_" + varName;
        var->type = strType;
        var->value  = strValue;
        var->globalSpaceGenPtr->Add(var);
    }
*/
    //PVD->dump();
}
