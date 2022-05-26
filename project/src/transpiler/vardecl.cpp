#include "vardecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"
#include <sstream>

using namespace clang;

void InitValueAnalysis(const VarDecl *VD, std::string &str);

void InitZeroValueAnalysis(const VarDecl *VD, std::string &str);

void ArrayToBytes(__attribute__((unused)) Stmt *stmt, size_t size, const VarDecl *p_decl, std::string &string);

Variable ProcessVariable(const VarDecl *VD, const std::string &local_name, size_t shift) {
  auto var_name = VD->getNameAsString();
  QualType qual_type = VD->getType();
  TypeInfo type_info = VD->getASTContext().getTypeInfo(qual_type);
  auto type_size = type_info.Width / 8;

  std::string str_type{std::string("c_" + GetTypeName(VD->getType()))};
  auto storage_class = VD->getStorageClass();
  auto static_local = VD->isStaticLocal();
  auto ext_storage = VD->hasExternalStorage();
  auto global_storage = VD->hasGlobalStorage();
  auto is_init = VD->hasInit();
  std::string str_value;
  if (is_init) {
    InitValueAnalysis(VD, str_value);
  } else {
    InitZeroValueAnalysis(VD, str_value);
  }
  EOObject eo_object{str_value};

  extern UnitTranspiler transpiler;

  if (global_storage && !ext_storage && !static_local && (storage_class != SC_Static)) {
    return transpiler.glob_.Add(VD, type_size, str_type, "g-" + var_name, eo_object);
  } else if (global_storage && !ext_storage) {
    return transpiler.glob_.Add(VD, type_size, str_type, "s-" + var_name, eo_object);
  } else if (global_storage) {
    return transpiler.glob_.AddExternal(VD, type_size, str_type, "e-" + var_name, eo_object);
  } else // its local variable!
  {
    if (local_name.empty()) {
      return {};
    }
    const auto *PD = llvm::dyn_cast<ParmVarDecl>(VD);
    if (PD) {
      return transpiler.glob_.Add(VD, type_size, str_type, "p-" + var_name,
                                  eo_object, local_name, shift, VD->hasInit());
    }
    return transpiler.glob_.Add(VD, type_size, str_type, "l-" + var_name,
                                eo_object, local_name, shift, VD->hasInit());
  }
}

void InitValueAnalysis(const VarDecl *VD, std::string &str) {
  auto qual_type = VD->getType();
  auto type_ptr = qual_type.getTypePtr();

  auto type_info = VD->getASTContext().getTypeInfo(qual_type);
  auto size = type_info.Width;
  APValue *init_val = VD->evaluateValue();

  if (init_val != nullptr) {
    if (init_val->isInt()) {
      auto int_value = init_val->getInt().getExtValue();
      if (type_ptr->isCharType()) {
        str = "'";
        str += char(int_value);
        str += "'";
      } else {
        str = std::to_string(int_value);
      }
    } else if (init_val->isFloat() && (size == 64)) {
      auto float_value = init_val->getFloat().convertToDouble();
      str = std::to_string(float_value);
    } else if (init_val->isFloat() && (size == 32)) {
      auto float_value = init_val->getFloat().convertToFloat();
      str = std::to_string(float_value);
    }
  } else {
    Stmt *body = (Stmt *) ((clang::InitListExpr *) (VD->getInit()));
    ArrayToBytes(body, size, VD, str);
  }
}

void ArrayToBytes(__attribute__((unused)) Stmt *stmt,
                  __attribute__((unused)) size_t size,
                  __attribute__((unused)) const VarDecl *p_decl,
                  std::string &string) {
  string += "plug";
}

void InitZeroValueAnalysis(const VarDecl *VD, std::string &str) {
  auto qual_type = VD->getType();
  auto type_ptr = qual_type.getTypePtr();

  if (type_ptr->isCharType()) {
    str = "'\\0'";
  } else if (type_ptr->isIntegerType() || type_ptr->isBooleanType()
      || type_ptr->isPointerType() || type_ptr->isRecordType() || type_ptr->isArrayType()) {
    str = "0";
  } else {
    str = "";
  }
}
