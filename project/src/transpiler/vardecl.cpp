#define  VAR_DECL_INFO
// Функции, используемые при анализе переменных

#include "vardecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"
#include <sstream>

using namespace clang;

// Анализ полученного начального значения с последующим использованием
void initValueAnalysis(const VarDecl *VD, std::string &str);

// Анализ типа для неициализированных переменны с установкой нулевого значения
void initZeroValueAnalysis(const VarDecl *VD, std::string &str);
// std::string getIntTypeByVar(const VarDecl* VD);

void arraayToBytes(Stmt *stmt, size_t size, const VarDecl *pDecl, std::string& string);

void intToBytes(IntegerLiteral *pLiteral, size_t size, std::string &str);

void floatToBytes(FloatingLiteral *pLiteral, size_t size, std::string &str);

Variable ProcessVariable(const VarDecl *VD, std::string local_name, size_t shift) {
  // Имя переменной
  auto varName = VD->getNameAsString();
  QualType qualType = VD->getType();
  TypeInfo typeInfo = VD->getASTContext().getTypeInfo(qualType);
  // размер в байтах
  auto typeSize = typeInfo.Width / 8;
  const IdentifierInfo* typeId = qualType.getBaseTypeIdentifier();

  auto typePtr = qualType.getTypePtr();
  // auto kind = typePtr->getKind();

  std::string strType{std::string("c_" + GetTypeName(VD->getType()))};
  // StorageClass getStorageClass() const
  // Показывает на явное описание того или иного класса памяти в тексте программы
  // Наверное не во всех случаях полезно
  auto storageClass = VD->getStorageClass();
  // Проверка на размещение переменной в локальной памяти
  auto inLocalStorage = VD->hasLocalStorage();
  // Проверка на статическую локальную переменную
  auto staticLocal = VD->isStaticLocal();
  // Внешняя переменная (описатель external)
  auto extStorage = VD->hasExternalStorage();
  // Размещение переменной в глобальной памяти
  // Глобальных и статических переменных
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
  EOObject eoObject{strValue};

  extern UnitTranspiler transpiler;

  // Проверка, что переменная является глобальной
  if (globalStorage && !extStorage && !staticLocal && (storageClass != SC_Static)) {
    return transpiler.glob.Add(VD, typeSize, strType, "g-" + varName, eoObject);
  } else if (globalStorage && !extStorage) {
    return transpiler.glob.Add(VD, typeSize, strType, "s-" + varName, eoObject);
  } else // its local variable!
  {
    if (local_name.empty()) {
        // Тут генерируется EoObject с name = "".
        return {};
    }
    const auto *PD = llvm::dyn_cast<ParmVarDecl>(VD);
    if (PD) {
      return transpiler.glob.Add(VD, typeSize, strType, "p-" + varName,
                                 eoObject, local_name, shift, VD->hasInit());
    }
    return transpiler.glob.Add(VD, typeSize, strType, "l-" + varName,
                               eoObject, local_name, shift, VD->hasInit());
  }
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
  // auto align = typeInfo.Align;  // не нужен
  APValue *initVal = VD->evaluateValue();

//  auto x = VD->getInit();
//  auto y = llvm::dyn_cast<InitListExpr>(x);
//  auto z = y->children();
//  for (const auto ch : z) {
//      llvm::outs() << ch->getStmtClassName() << "\n";
//  }

  if (initVal != nullptr) {
    if (initVal->isInt()) {
      auto intValue = initVal->getInt().getExtValue();
      // llvm::outs() << intValue;
      if (typePtr->isCharType()) {
        str = "'";
        str += char(intValue);
        str += "'";
      } else {
        str = std::to_string(intValue); // просто целое число
      }
    } else if (initVal->isFloat() && (size == 64)) {
      auto floatValue = initVal->getFloat().convertToDouble();
      // llvm::outs() << floatValue;
      str = std::to_string(floatValue);
    } else if (initVal->isFloat() && (size == 32)) {
      auto floatValue = initVal->getFloat().convertToFloat();
      // llvm::outs() << floatValue;
      str = std::to_string(floatValue);
    }
  } else {
    Stmt *body = (Stmt *) ((clang::InitListExpr *) (VD->getInit()));
    arraayToBytes(body, size, VD, str);
//    str = "(" + str + ")";
  }
}

void arraayToBytes(Stmt *stmt, size_t size, const VarDecl *pDecl, std::string& str) {
  stmt->dump();
  if (stmt->getStmtClass() == clang::Stmt::InitListExprClass) {
    auto *body = llvm::dyn_cast<InitListExpr>(stmt);
    QualType qualType = body->getType();
    size_t elementSize = 0;
    if (qualType->isArrayType()) {
      auto *arrayType = llvm::dyn_cast<ConstantArrayType>(qualType);
      auto elementType = arrayType->getElementType();
      elementSize = pDecl->getASTContext().getTypeInfo(elementType).Width / 8;
    }
    for (auto element = body->child_begin(); element != body->child_end(); element++) {
      if (qualType->isRecordType()) {
        elementSize = 4; // todo
      }
      arraayToBytes(*element, elementSize, pDecl, str);
    }
  } else if (stmt->getStmtClass() == Stmt::IntegerLiteralClass) {
    auto *body = llvm::dyn_cast<IntegerLiteral>(stmt);
    intToBytes(body, size, str);
  } else if (stmt->getStmtClass() == Stmt::FloatingLiteralClass) {
    auto *body = llvm::dyn_cast<FloatingLiteral>(stmt);
    floatToBytes(body, size, str);
  } else if (stmt->getStmtClass() == Stmt::ImplicitCastExprClass) {
    for (auto child = stmt->child_begin(); child != stmt->child_end(); child++)
      arraayToBytes(*child, size, pDecl, str);
  }
  llvm::errs() << "\n";
}

void floatToBytes(FloatingLiteral *pLiteral, size_t size, std::string &str) {
  std::string alph = "0123456789abcdef";
  llvm::APFloat an_float = pLiteral->getValue();
  auto fVal = (an_float.convertToDouble());
  long long val = *(reinterpret_cast<long*>(&fVal));
  while (size--) {
    str += alph[(val / 16 % 16)];
    str += alph[(val % 16)];
    val /= 256;
    str += " ";
  }
}

void intToBytes(IntegerLiteral *pLiteral, size_t size, std::string &str) {
  std::string alph = "0123456789abcdef";
  bool is_signed = pLiteral->getType()->isSignedIntegerType();
  llvm::APInt an_int = pLiteral->getValue();
  int64_t val = 0;
  if(is_signed)
    val = an_int.getSExtValue();
  else
    val = an_int.getZExtValue();
  while (size--) {
    str += alph[(val / 16 % 16)];
    str += alph[(val % 16)];
    val /= 256;
    str += " ";
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
  if (typePtr->isCharType()) {
    str = "'\\0'";
  } else if (typePtr->isIntegerType() || typePtr->isBooleanType()) {
    str = "0";
  } else if (typePtr->isIntegerType() || typePtr->isBooleanType()) {
    str = "0";
  } else if (typePtr->isPointerType()) {
    str = "0";
  } else if (typePtr->isRecordType()) {
    const RecordDecl *RD = typePtr->getAsRecordDecl();
    str = "0";
  } else if (typePtr->isArrayType()) {
      str = "0";
  } else {
      str = "";
  }
}
