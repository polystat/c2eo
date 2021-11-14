#define  VAR_DECL_INFO
// Функции, используемые при анализе переменных

#include "vardecl.h"
#include "generator.h"


std::string getIntTypeByVar(const VarDecl* VD);

// Определение и тестовый вывод основных параметров описания переменных
void getVarDeclParameters(const VarDecl* VD) {
    // Имя переменной
    auto varName = VD->getNameAsString();
    auto varID = reinterpret_cast<uint64_t>(VD);
#ifdef VAR_DECL_INFO
    llvm::outs() << "Name of Variable: " << varName << "\n";
    llvm::outs() << "  Var Kind Name: " << VD->getDeclKindName() << "\n";
    llvm::outs() << "  Var ID: " << varID << "\n";
#endif
    TypeInfo typeInfo = VD->getASTContext().getTypeInfo(VD->getType());
    auto typeSize = typeInfo.Width;
    auto fieldAlign = typeInfo.Align;
#ifdef VAR_DECL_INFO
    llvm::outs() << "  Size = " << typeInfo.Width << ", Align = " << typeInfo.Align << "\n";
#endif
    // Получение типа переменной
    ///std::string typeName{"any value"};
    ///StringRef varTypeName(typeName);
    ////auto qualType = VD->getType();
    QualType qualType = VD->getType();
    const IdentifierInfo* typeId = qualType.getBaseTypeIdentifier();
    ////varTypeName = typeId->getName();
    ////llvm::outs() << "  Type is " << varTypeName << "\n";
#ifdef VAR_DECL_INFO
    if (typeId != nullptr) {
        llvm::outs() << "  Type is " << typeId->getName() << "\n";
    } else {
        llvm::outs() << "  The problem to identify type name\n";
    }
#endif
    auto typePtr = qualType.getTypePtr();
#ifdef VAR_DECL_INFO
    if (typePtr->isBuiltinType()) {
        llvm::outs() << "  It is Builtin type\n";
//         BuiltinType* btPtr = dynamic_cast<BuiltinType*>(typePtr);
//         if(btPtr != nullptr) {
//             llvm::outs() << "  It is Possible to convert into Builtin type\n";
//         } else {
//             llvm::outs() << "  It is Impossible to convert into Builtin type\n";
//         }
    } else {
        llvm::outs() << "  It is not Builtin type\n";
    }
#endif
    //auto kind = typePtr->getKind();

    std::string strType = "";
    getTypeName(VD, strType);
/*
    if (typePtr->isCharType()) {
#ifdef VAR_DECL_INFO
        if (typePtr->isSignedIntegerType()) {
            llvm::outs() << "    -->signedCharType\n";
        } else {
            llvm::outs() << "    -->unsignedCharType\n";
        }
#endif
        strType = "c_char";
    } else if (typePtr->isBooleanType()) {
#ifdef VAR_DECL_INFO
        llvm::outs() << "    -->isBooleanType\n";
#endif
        strType = "c_bool";
    } else if (typePtr->isRealFloatingType()) {
#ifdef VAR_DECL_INFO
        llvm::outs() << "    -->isRealFloatingType\n";
#endif
        strType = "c_float64";
    } else if (typePtr->isIntegerType()) {
#ifdef VAR_DECL_INFO
        if (typePtr->isSignedIntegerType())
            llvm::outs() << "    -->signedIntegerType\n";
        else
            llvm::outs() << "    -->unsignedIntegerType\n";
#endif
        //TODO доработать этот код для разных размеров
        strType = getIntTypeByVar(VD);
    } else if (typePtr->isUnionType()) {
        RecordDecl* RD = typePtr->getAsRecordDecl();
        strType = "un_";
        if (RD->hasNameForLinkage())
            strType += RD->getNameAsString();
        else
            strType += std::to_string(reinterpret_cast<uint64_t>(RD));
#ifdef VAR_DECL_INFO
            llvm::outs() << "  " << strType << "\n";
#endif
    } else if (typePtr->isStructureType()) {
        RecordDecl* RD = typePtr->getAsRecordDecl();
        strType = "st_";
        if (RD->hasNameForLinkage())
            strType += RD->getNameAsString();
        else
            strType += std::to_string(reinterpret_cast<uint64_t>(RD));
#ifdef VAR_DECL_INFO
            llvm::outs() << "  " << strType << "\n";
#endif
    }*/
#ifdef VAR_DECL_INFO
    llvm::outs() << "  !!! class name = " << typePtr->getTypeClassName() << "\n";
#endif
    // StorageClass getStorageClass() const
    // Показывает на явное описани того или иного класса памяти в тексте программы
    // Наверное не во всех случаях полезно
    auto storageClass = VD->getStorageClass();
#ifdef VAR_DECL_INFO

    llvm::outs() << "  storage class is ";
    switch (storageClass) {
        case SC_None:
            llvm::outs() << "None";
            break;
        case SC_Extern:
            llvm::outs() << "Extern";
            break;
        case SC_Static:
            llvm::outs() << "Static";
            break;
        case SC_PrivateExtern:
            llvm::outs() << "PrivateExtern";
            break;
        case SC_Auto:
            llvm::outs() << "Auto";
            break;
        case SC_Register:
            llvm::outs() << "Register";
            break;
        default:
            llvm::outs() << "NOT DEFINED";
    }
    llvm::outs() << "\n";
#endif
    // Проверка на размещение переменной в локальной памяти
    auto inLocalStorage = VD->hasLocalStorage();
#ifdef VAR_DECL_INFO

    if (inLocalStorage) {
        llvm::outs() << "  Local Storage\n";
    } else {
        llvm::outs() << "  not Local Storage\n";
    }
#endif
    // Проверка на статическую локальную переменную
    auto staticLocal = VD->isStaticLocal();
#ifdef VAR_DECL_INFO
    if (staticLocal) {
        llvm::outs() << "  Static Local\n";
    } else {
        llvm::outs() << "  not Static Local\n";
    }
#endif
    // Внешняя переменная (описатель external)
    auto extStorage = VD->hasExternalStorage();
#ifdef VAR_DECL_INFO
    if (extStorage) {
        llvm::outs() << "  External Storage Defenition\n";
    } else {
        llvm::outs() << "  not External Storage Definition\n";
    }
#endif
    // Размещение переменной в глобальной памяти
    // Касается глобальных и статических переменных
    auto globalStorage = VD->hasGlobalStorage();
#ifdef VAR_DECL_INFO
    if (globalStorage) {
        llvm::outs() << "  has GlobalStorage\n";
    } else {
        llvm::outs() << "  has not GlobalStorage\n";
    }
#endif
    // Переменная с локальной видимостью
    auto localVarDecl = VD->isLocalVarDecl();
#ifdef VAR_DECL_INFO
    if (localVarDecl) {
        llvm::outs() << "  is Local VarDecl\n";
    } else {
        llvm::outs() << "  is not Local VarDecl\n";
    }
#endif
    // Переменная или параметр с локальной видимостью
    auto localVarDeclOrParm = VD->isLocalVarDeclOrParm();
#ifdef VAR_DECL_INFO
    if (localVarDeclOrParm) {
        llvm::outs() << "  is Local VarDecl or Parm\n";
    } else {
        llvm::outs() << "  is not Local VarDecl or Parm\n";
    }
#endif
    // Наличие начальной инициализации
    auto isInit = VD->hasInit();
    std::string strValue = "";

    if (isInit) {
#ifdef VAR_DECL_INFO
        llvm::outs() << "  has Initializer\n";
#endif
        initValueAnalysis(VD, strValue);
    } else {
#ifdef VAR_DECL_INFO
        llvm::outs() << "  has not Initializer\n";
#endif
        initZeroValueAnalysis(VD, strValue);
    }




    // Проверка, что переменная является глобальной
    if (globalStorage && !extStorage && !staticLocal && (storageClass != SC_Static)) {
        // Формируется глобальная переменная со всеми атрибутами
        VarGen* var = new VarGen;

        var->name = "g_" + varName;
        var->type = strType;
        var->value = strValue;
        var->identifiers[varID] = "^." + var->name;
        var->globalSpaceGenPtr->Add(var);
    } else if (globalStorage && !extStorage) {
        VarGen* var = new VarGen;
        var->name = "s_" + varName;
        var->type = strType;
        var->value = strValue;
        var->identifiers[varID] = "^." + AbstractGen::filename + "." + var->name;
        var->staticSpaceGenPtr->Add(var);
    }

    /*
       // Проверка, что переменная является статической.
       if (globalStorage && !extStorage && !staticLocal)
       {
           // Формируется глобальная переменная со всеми атрибутами
           VarGen* var = new VarGen;

           var->name = "g_" + varName;
           var->type = strType;
           var->value  = strValue;
           var->globalSpaceGenPtr->Add(var);
       }

       // Проверка, что переменная является статической.
        else if (staticLocal)
       {
           // Формируется глобальная переменная со всеми атрибутами
           StaticVarGen* var = new StaticVarGen;

           var->name = "s_" + varName;
           var->type = strType;
           var->value  = strValue;
           var->globalSpaceGenPtr->Add(var);
       } */

    //VD->dump();
}

std::string getIntTypeByVar(const VarDecl* VD) {
    auto qualType = VD->getType();      // квалифицированный тип (QualType)
    auto typeInfo = VD->getASTContext().getTypeInfo(qualType);
    bool isSigned = qualType->isSignedIntegerType();
    auto size = typeInfo.Width;
    /*
    std::string result = "";
    //TODO обработка беззнаковых, когда они появятся.
    if (isSigned)
    {
        switch (size)
        {
            case 16:
                result = "c_int16";
                break;
            case 32:
                result = "c_int32";
                break;
            case 64:
                result = "c_int64";
                break;
        }
    } else
    {
        switch (size)
        {
            case 16:
                result = "c_int16";
                break;
            case 32:
                result = "c_int32";
                break;
            case 64:
                result = "c_int64";
                break;
        }
    }*/
    std::string result = "c_";
    //TODO обработка беззнаковых, когда они появятся. (нет только c_uint64)
    if (!isSigned)
        result += 'u';
    result += "int" + std::to_string(size);
    return result;
}

// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initValueAnalysis(const VarDecl* VD, std::string &str) {
    // Анализ типа переменной для корректного преобразования в тип Eolang
    auto qualType = VD->getType();      // квалифицированный тип (QualType)
    auto typePtr = qualType.getTypePtr();   // указатель на тип (Type)

    // Анализ размера переменной для определения разновидности данных
    auto typeInfo = VD->getASTContext().getTypeInfo(qualType);
    auto size = typeInfo.Width;
    //auto align = typeInfo.Align;  // не нужен
    APValue* initVal = VD->evaluateValue();
    if (initVal != nullptr) {
        llvm::outs() << "    Initial Value = ";
        if (initVal->isInt()) {
            auto intValue = initVal->getInt().getExtValue();
            //llvm::outs() << intValue;
            if (typePtr->isCharType()) {
                str = "'";
                str += char(intValue);
                str += "'";
            } else {
                str = std::to_string(intValue); // просто целое число
            }
        } else if (initVal->isFloat() && (size == 64)) {
            auto floatValue = initVal->getFloat().convertToDouble();
            //llvm::outs() << floatValue;
            str = std::to_string(floatValue);
        } else if (initVal->isFloat() && (size == 32)) {
            auto floatValue = initVal->getFloat().convertToFloat();
            //llvm::outs() << floatValue;
            str = std::to_string(floatValue);
        }
        llvm::outs() << str << "\n";
    } else {
        str = "";
        for (InitListExpr::iterator it = ((clang::InitListExpr * )(VD->getInit()))->begin();
             it != ((clang::InitListExpr * )(VD->getInit()))->end(); it++) {
            if (!str.empty()) str += " ";
            getValue(*it, str);
        }
        llvm::outs() << "    no Initial Value\n";
    }
}

// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initZeroValueAnalysis(const VarDecl* VD, std::string &str) {
    // Анализ типа переменной для корректного преобразования в тип Eolang
    auto qualType = VD->getType();      // квалифицированный тип (QualType)
    auto typePtr = qualType.getTypePtr();   // указатель на тип (Type)

    // Анализ размера переменной для определения разновидности данных
    auto typeInfo = VD->getASTContext().getTypeInfo(qualType);
    auto size = typeInfo.Width;
    if (typePtr->isCharType()) {
        str = "'\\0'";
    } else if (typePtr->isIntegerType() || typePtr->isBooleanType()) {
        str = "0";
    } else if (typePtr->isRealFloatingType()) {
        str = "0.0";
    } else if (typePtr->isUnionType() || typePtr->isStructureType()) {
        const RecordDecl* RD = typePtr->getAsRecordDecl();
        str = "";
        for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
            if (!str.empty()) str += " ";
            std::string fieldVal = "";
            initZeroValueAnalysis((const VarDecl*) (*it), fieldVal);
            str += fieldVal;
        }
    } else {
        str = "";
    }

}


void getTypeName(const ValueDecl* VD, std::string &str) {
    TypeInfo typeInfo = VD->getASTContext().getTypeInfo(VD->getType());
    uint64_t typeSize = typeInfo.Width;
    unsigned fieldAlign = typeInfo.Align;
    const QualType qualType = VD->getType();
    const clang::Type* typePtr = qualType.getTypePtr();
    str = "c_";

    if (typePtr->isBooleanType()) {
        str += "bool";
        return;
    }
    if (typePtr->isFloatingType()) {
        str += "float" + std::to_string(typeSize);
        return;
    }

    if (!typePtr->isSignedIntegerType())
        str += "u";
    if (typePtr->isCharType()) {
        str += "char";
        return;
    }
    if (typePtr->isIntegerType()) {
        str += "int" + std::to_string(typeSize);
        return;
    }


    if (typePtr->isUnionType())
        str = "un_";
    if (typePtr->isStructureType())
        str = "st_";
    if (typePtr->isUnionType() || typePtr->isStructureType()) {
        RecordDecl* RD = typePtr->getAsRecordDecl();
        if (RD->hasNameForLinkage())
            str += RD->getNameAsString();
        else
            str += std::to_string(reinterpret_cast<uint64_t>(RD));
        return;
    }
}

void getValue(const Stmt* stmt, std::string &str) {

    str += "0";
}