#include "decl.h"
#include "recorddecl.h"


void getTypeName(VarDecl* VD, std::string &str);

void initValueAnalysis_1(const VarDecl* VD, std::string &str);

void initZeroValueAnalysis_1(const VarDecl* VD, std::string &str);

std::string GetNameStoragePrefix(VarDecl* VD);

std::string getNestedPrefix(VarDecl* VD);

std::vector<AbstractGen*> getDeclGen(Decl* decloration) {
    std::vector < AbstractGen * > result;
    auto declKind = decloration->getKind();
    if (declKind == Decl::Var) {
        VarDecl* VD = dyn_cast<VarDecl>(decloration);
        VarGen* VG = getVarDeclGen(VD);
        result.push_back(VG);
    } else if (declKind == Decl::Record) {
        RecordDecl* RD = dyn_cast<RecordDecl>(decloration);
        std::vector < RecordGen * > RGs = getAllRecordDeclSubObjects(RD);
        for (auto rg: RGs)
            result.push_back(rg);
    }
    return result;
}

VarGen* getVarDeclGen(VarDecl* VD) {
    VarGen* var = new VarGen;
    auto varName = VD->getNameAsString();
    auto varID = reinterpret_cast<uint64_t>(VD);
    std::string strType = "";
    getTypeName(VD, strType);
    auto isInit = VD->hasInit();
    std::string strValue = "";
    StmtGen* stmtValue = nullptr;
    if (isInit) {
        stmtValue = getStmtGen(VD->getInit());
    } else {
        initZeroValueAnalysis_1(VD, strValue);
    }
    std::string nameStoragePrefix = GetNameStoragePrefix(VD);
    var->name = nameStoragePrefix + varName;
    var->type = strType;
    var->value = strValue;
    var->nestedStmt = stmtValue;
    std::string nestedPrefix = getNestedPrefix(VD);
    var->identifiers[varID] = nestedPrefix + var->name;
    return var;
}

std::string getNestedPrefix(VarDecl* VD) {
    auto globalStorage = VD->hasGlobalStorage();
    auto extStorage = VD->hasExternalStorage();
    auto storageClass = VD->getStorageClass();
    auto staticLocal = VD->isStaticLocal();
    if (globalStorage && !extStorage && (storageClass == SC_Static || staticLocal))
        return AbstractGen::filename + ".";
    return "";
}

std::string GetNameStoragePrefix(VarDecl* VD) {
    auto globalStorage = VD->hasGlobalStorage();
    auto extStorage = VD->hasExternalStorage();
    auto localVarDecl = VD->isLocalVarDecl();
    auto storageClass = VD->getStorageClass();
    auto staticLocal = VD->isStaticLocal();
    if (globalStorage && !extStorage && !staticLocal && (storageClass != SC_Static))
        return "g_";
    if (globalStorage && !extStorage)
        return "s_";
    if (localVarDecl)
        return "l_";
    return "undefined_";
}


// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initValueAnalysis_1(const VarDecl* VD, std::string &str) {
    //Init Value is only stmt, then lets use getStmt


    /*
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
        Stmt* body = (Stmt * )((clang::InitListExpr * )(VD->getInit()));
        getListValue(body, str, &VD->getASTContext());
        llvm::outs() << "    no Initial Value\n";
    }*/
}

// Анализ полученного начального значения с тестовым выводом его
// и формированием строки со значением на выходе
void initZeroValueAnalysis_1(const VarDecl* VD, std::string &str) {
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
    } else if (typePtr->isRecordType()) {
        const RecordDecl* RD = typePtr->getAsRecordDecl();
        str = "";
        for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
            if (!str.empty()) str += " ";
            std::string fieldVal = "";
            initZeroValueAnalysis_1((const VarDecl*) (*it), fieldVal);
            str += fieldVal;
        }
    } else {
        str = "";
    }
}


void getTypeName(VarDecl* VD, std::string &str) {
    TypeInfo typeInfo = VD->getASTContext().getTypeInfo(VD->getType());
    uint64_t typeSize = typeInfo.Width;
//    unsigned fieldAlign = typeInfo.Align;
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
