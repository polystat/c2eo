#define  VAR_DECL_INFO
// Функции, используемые при анализе переменных

#include "vardecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"
#include <sstream>

using namespace clang;

// Анализ полученного начального значения с последующим использованием
void initValueAnalysis(const VarDecl *VD, std::string &str);

// Анализ типа для неициализированных переменны с установко нулевого значения
void initZeroValueAnalysis(const VarDecl *VD, std::string &str);
//std::string getIntTypeByVar(const VarDecl* VD);

Variable ProcessVariable(const VarDecl *VD, std::string local_name, size_t shift){
  // Имя переменной
  auto varName = VD->getNameAsString();
  QualType qualType = VD->getType();
  TypeInfo typeInfo = VD->getASTContext().getTypeInfo(qualType);
  // размер в байтах
  auto typeSize = typeInfo.Width / 8;
  const IdentifierInfo* typeId = qualType.getBaseTypeIdentifier();

  auto typePtr = qualType.getTypePtr();
  //auto kind = typePtr->getKind();

  std::string strType = "c_" + GetTypeName(VD->getType());
  // StorageClass getStorageClass() const
  // Показывает на явное описани того или иного класса памяти в тексте программы
  // Наверное не во всех случаях полезно
  auto storageClass = VD->getStorageClass();
  // Проверка на размещение переменной в локальной памяти
  auto inLocalStorage = VD->hasLocalStorage();
  // Проверка на статическую локальную переменную
  auto staticLocal = VD->isStaticLocal();
  // Внешняя переменная (описатель external)
  auto extStorage = VD->hasExternalStorage();
  // Размещение переменной в глобальной памяти
  // Касается глобальных и статических переменных
  auto globalStorage = VD->hasGlobalStorage();
  // Переменная с локальной видимостью
  auto localVarDecl = VD->isLocalVarDecl();
  // Переменная или параметр с локальной видимостью
  auto localVarDeclOrParm = VD->isLocalVarDeclOrParm();
  // Наличие начальной инициализации
  auto isInit = VD->hasInit();
  std::string strValue;
  if (isInit) {
    initValueAnalysis(VD, strValue);
  } else {
    initZeroValueAnalysis(VD, strValue);
  }

  extern UnitTranspiler transpiler;

  // Проверка, что переменная является глобальной
  if (globalStorage && !extStorage && !staticLocal && (storageClass != SC_Static)) {
    return transpiler.glob.Add(VD, typeSize, strType, "g-" + varName, strValue);
  } else if (globalStorage && !extStorage) {
    return transpiler.glob.Add(VD, typeSize, strType, "s-" + varName, strValue);
  } else // its local variable!
  {
    if (local_name.empty())
      return {};
    const auto *PD = llvm::dyn_cast<ParmVarDecl>(VD);
    if (PD) {
      return transpiler.glob.Add(VD, typeSize, strType, "p-" + varName, strValue, local_name, shift, VD->hasInit());
    }
    return transpiler.glob.Add(VD, typeSize, strType, "l-" + varName, strValue, local_name, shift, VD->hasInit());
  }
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
    } else {
        //TODO fix generation of non-const initialization. Maybe value in Variable should be EOObject!
        //TODO Fix Record decl generation
        Stmt* body = (Stmt * )((clang::InitListExpr * )(VD->getInit()));
        std::stringstream ss;
        //getASTStmtGen(body, &VD->getASTContext())->Generate(ss);
        str = "plug";
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
    } else {
        str = "0";
    }
}