// Функции, используемые при анализе переменных

#include "vardecl.h"
#include "generator.h"

// Определение и тестовый вывод основных параметров описания переменных
void getVarDeclParameters(const VarDecl *VD) {
    // Имя переменной
    auto varName = VD->getNameAsString();
    llvm::outs() << "Name of Variable: " << varName << "\n";
    llvm::outs() << "  Var Kind Name: " << VD->getDeclKindName() << "\n";

    TypeInfo typeInfo = VD->getASTContext().getTypeInfo(VD->getType());
    auto typeSize = typeInfo.Width;
    auto fieldAlign = typeInfo.Align;
    llvm::outs() << "  Size = " << typeInfo.Width << ", Align = " << typeInfo.Align << "\n";

    // Получение типа переменной
    ///std::string typeName{"any value"};
    ///StringRef varTypeName(typeName);
    ////auto qualType = VD->getType();
    QualType qualType = VD->getType();
    const IdentifierInfo *typeId = qualType.getBaseTypeIdentifier();
    ////varTypeName = typeId->getName();
    ////llvm::outs() << "  Type is " << varTypeName << "\n";
    if (typeId != nullptr) {
        llvm::outs() << "  Type is " << typeId->getName() << "\n";
    } else {
        llvm::outs() << "  The problem to identify type name\n";
    }

    auto typePtr = qualType.getTypePtr();
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
    //auto kind = typePtr->getKind();

    std::string strType = "";

    if (typePtr->isCharType()) {
        if (typePtr->isSignedIntegerType()) {
            llvm::outs() << "    -->signedCharType\n";
        } else {
            llvm::outs() << "    -->unsignedCharType\n";
        }
        strType = "c_char";
    } else if (typePtr->isBooleanType()) {
        llvm::outs() << "    -->isBooleanType\n";
        strType = "c_bool";
    } else if (typePtr->isRealFloatingType()) {
        llvm::outs() << "    -->isRealFloatingType\n";
        strType = "c_float";
    } else if (typePtr->isIntegerType()) {
        if (typePtr->isSignedIntegerType())
            llvm::outs() << "    -->signedIntegerType\n";
        else
            llvm::outs() << "    -->unsignedIntegerType\n";
        //TODO доработать этот код для разных размеров
        strType = "c_int32";
    }

    llvm::outs() << "  !!! class name = " << typePtr->getTypeClassName() << "\n";

    // StorageClass getStorageClass() const
    // Показывает на явное описани того или иного класса памяти в тексте программы
    // Наверное не во всех случаях полезно
    auto storageClass = VD->getStorageClass();
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

    // Проверка на размещение переменной в локальной памяти
    auto inLocalStorage = VD->hasLocalStorage();
    if (inLocalStorage) {
        llvm::outs() << "  Local Storage\n";
    } else {
        llvm::outs() << "  not Local Storage\n";
    }
    // Проверка на статическую локальную переменную
    auto staticLocal = VD->isStaticLocal();
    if (staticLocal) {
        llvm::outs() << "  Static Local\n";
    } else {
        llvm::outs() << "  not Static Local\n";
    }
    // Внешняя переменная (описатель external)
    auto extStorage = VD->hasExternalStorage();
    if (extStorage) {
        llvm::outs() << "  External Storage Defenition\n";
    } else {
        llvm::outs() << "  not External Storage Definition\n";
    }
    // Размещение переменной в глобальной памяти
    // Касается глобальных и статических переменных
    auto globalStorage = VD->hasGlobalStorage();
    if (globalStorage) {
        llvm::outs() << "  has GlobalStorage\n";
    } else {
        llvm::outs() << "  has not GlobalStorage\n";
    }
    // Переменная с локальной видимостью
    auto localVarDecl = VD->isLocalVarDecl();
    if (localVarDecl) {
        llvm::outs() << "  is Local VarDecl\n";
    } else {
        llvm::outs() << "  is not Local VarDecl\n";
    }
    // Переменная или параметр с локальной видимостью
    auto localVarDeclOrParm = VD->isLocalVarDeclOrParm();
    if (localVarDeclOrParm) {
        llvm::outs() << "  is Local VarDecl or Parm\n";
    } else {
        llvm::outs() << "  is not Local VarDecl or Parm\n";
    }
    // Наличие начальной инициализации
    auto isInit = VD->hasInit();
    std::string strValue = "";
    if (isInit) {
        llvm::outs() << "  has Initializer\n";
        initValueAnalysis(VD, strValue);
    } else {
        llvm::outs() << "  has not Initializer\n";
        initZeroValueAnalysis(VD, strValue);
    }




    // Проверка, что переменная является глобальной
    if (globalStorage && !extStorage && !staticLocal && (storageClass != SC_Static)) {
        // Формируется глобальная переменная со всеми атрибутами
        VarGen *var = new VarGen;

        var->name = "g_" + varName;
        var->type = strType;
        var->value = strValue;
        var->globalSpaceGenPtr->Add(var);
    } else if (globalStorage && !extStorage)
    {
        VarGen* var = new VarGen;
        var->name = "ss_" + varName;
        var->type = strType;
        var->value  = strValue;
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

// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initValueAnalysis(const VarDecl *VD, std::string &str) {
    // Анализ типа переменной для корректного преобразования в тип Eolang
    auto qualType = VD->getType();      // квалифицированный тип (QualType)
    auto typePtr = qualType.getTypePtr();   // указатель на тип (Type)

    // Анализ размера переменной для определения разновидности данных
    auto typeInfo = VD->getASTContext().getTypeInfo(qualType);
    auto size = typeInfo.Width;
    //auto align = typeInfo.Align;  // не нужен
    APValue *initVal = VD->evaluateValue();
    if(initVal != nullptr) {
        llvm::outs() << "    Initial Value = ";
        if(initVal->isInt()) {
            auto intValue = initVal->getInt().getExtValue();
            //llvm::outs() << intValue;
            if(typePtr->isBooleanType()) {
                if(intValue == 0) {str = "false";}
                else {str = "true";}
            } else if(typePtr->isCharType()) {
                str = "'";
                str += char(intValue);
                str += "'";
            } else {
                str = std::to_string(intValue); // просто целое число
            }
        } else if(initVal->isFloat() && (size == 64)) {
            auto floatValue = initVal->getFloat().convertToDouble();
            //llvm::outs() << floatValue;
            str = std::to_string(floatValue);
        } else if(initVal->isFloat() && (size == 32)) {
            auto floatValue = initVal->getFloat().convertToFloat();
            //llvm::outs() << floatValue;
            str = std::to_string(floatValue);
        }
        llvm::outs() << str << "\n";
    } else {
        llvm::outs() << "    no Initial Value\n";
    }
}

// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initZeroValueAnalysis(const VarDecl *VD, std::string &str) {
    // Анализ типа переменной для корректного преобразования в тип Eolang
    auto qualType = VD->getType();      // квалифицированный тип (QualType)
    auto typePtr = qualType.getTypePtr();   // указатель на тип (Type)

    // Анализ размера переменной для определения разновидности данных
    auto typeInfo = VD->getASTContext().getTypeInfo(qualType);
    auto size = typeInfo.Width;
    if(typePtr->isBooleanType()) {
        str = "false";
    } else if(typePtr->isCharType()) {
        str = "'\\0'";
    } else if (typePtr->isIntegerType()){
        str = "0";
    } else if(typePtr->isRealFloatingType()) {
        str = "0.0";
    } else {
        str = "";
    }
}

