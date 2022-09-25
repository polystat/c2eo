/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "src/transpiler/vardecl.h"

#include <string>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"

using clang::VarDecl;

EOObject InitValueAnalysis(const VarDecl *VD);

EOObject InitZeroValueAnalysis(const VarDecl *VD);

__attribute__((unused)) void ArrayToBytes(__attribute__((unused))
                                          clang::Stmt *stmt,
                                          size_t size, const VarDecl *p_decl,
                                          std::string &string);

EOObject InitValueEOObj(const VarDecl *VD, bool is_init);

Variable ProcessVariable(const VarDecl *VD, const std::string &local_name,
                         size_t shift) {
  // TEST Out of ProcessVariable input
  // std::cout << "ProcessVariable\n";

  auto var_name = VD->getNameAsString();
  // TEST
  // std::cout << "var_name = " << var_name << "\n";

  clang::QualType qual_type = VD->getType();
  clang::TypeInfo type_info = VD->getASTContext().getTypeInfo(qual_type);
  auto type_size = type_info.Width / byte_size;
  if (qual_type->isFloatingType() && type_size == 4) {
    type_size = 8;  // 8 bytes for float32.
  }

  std::string str_type{"c_" + GetTypeName(VD->getType())};
  auto storage_class = VD->getStorageClass();
  auto static_local = VD->isStaticLocal();
  auto ext_storage = VD->hasExternalStorage();
  auto global_storage = VD->hasGlobalStorage();
  auto is_init = VD->hasInit();
  // std::string str_value;
  EOObject initial_value = InitValueEOObj(VD, is_init);
  extern UnitTranspiler transpiler;

  if (global_storage && !ext_storage && !static_local &&
      (storage_class != clang::SC_Static)) {
    return transpiler.glob_.Add(VD, type_size, str_type, "g-" + var_name,
                                initial_value);
  }
  if (global_storage && !ext_storage) {
    return transpiler.glob_.Add(VD, type_size, str_type, "s-" + var_name,
                                initial_value);
  }
  if (global_storage) {
    return transpiler.glob_.AddExternal(VD, type_size, str_type,
                                        "e-" + var_name, initial_value);
  }
  // its local variable!

  if (local_name.empty()) {
    return {};
  }
  const auto *PD = llvm::dyn_cast<clang::ParmVarDecl>(VD);
  if (PD != nullptr) {
    return transpiler.glob_.Add(VD, type_size, str_type, "p-" + var_name,
                                initial_value, local_name, shift,
                                VD->hasInit());
  }
  return transpiler.glob_.Add(VD, type_size, str_type, "l-" + var_name,
                              initial_value, local_name, shift, VD->hasInit());
}

EOObject InitValueEOObj(const VarDecl *VD, bool is_init) {
  if (is_init) {
    return InitValueAnalysis(VD);
  }
  return InitZeroValueAnalysis(VD);
}

EOObject InitValueAnalysis(const VarDecl *VD) {
  auto qual_type = VD->getType();
  auto type_info = VD->getASTContext().getTypeInfo(qual_type);
  auto size = type_info.Width;
  clang::APValue *init_val = VD->evaluateValue();
  if (init_val == nullptr) {
    return GetStmtEOObject(VD->getInit());
  }

  std::string str;
  if (init_val->isInt()) {
    auto int_value = init_val->getInt().getExtValue();
    str = std::to_string(int_value);
  } else if (init_val->isFloat() && (size == double_size)) {
    auto float_value = init_val->getFloat().convertToDouble();
    str = std::to_string(float_value);
  } else if (init_val->isFloat() && (size == float_size)) {
    auto float_value = init_val->getFloat().convertToFloat();
    str = std::to_string(float_value);
  }
  if (str.empty()) {
    return GetStmtEOObject(VD->getInit());
  }
  return {str, EOObjectType::EO_LITERAL};
}

EOObject InitZeroValueAnalysis(const VarDecl *VD) {
  auto qual_type = VD->getType();
  // TEST
  // std::cout << "InitZeroValueAnalysis - start\n";

  const auto *type_ptr = qual_type.getTypePtr();
  std::string str;
  if (type_ptr->isIntegerType() || type_ptr->isBooleanType() ||
      type_ptr->isPointerType() || type_ptr->isRecordType() ||
      type_ptr->isArrayType() || type_ptr->isCharType()) {
    str = "0";
  } else if (type_ptr->isRealType() || type_ptr->isFloatingType()) {
    str = "0.000000";
  } else {
    str = "";
  }
  return {str, EOObjectType::EO_LITERAL};
}
