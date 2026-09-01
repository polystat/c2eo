/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/enumdecl.h"

#include <algorithm>
#include <string>
#include <vector>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/vardecl.h"

EnumType ProcessEnumDecl(const clang::EnumDecl *ED) {
  if (ED == nullptr) {
    return {};
  }

  extern UnitTranspiler transpiler;
  if (transpiler.enum_manager_.GetById(ED) != nullptr) {
    return {};
  }

  std::vector<EnumConstantType> constants;
  std::string enum_name = "en-";
  try {
    enum_name += ED->getNameAsString();
  } catch (std::exception &) {
    enum_name += "noname";
  }

  uint64_t size = 0;

  for (auto decl = ED->decls_begin(); decl != ED->decls_end(); decl++) {
    if (decl->getKind() == clang::Decl::EnumConstant) {
      auto *const enum_const_decl =
          llvm::dyn_cast<clang::EnumConstantDecl>(*decl);
      if (enum_const_decl == nullptr) {
        return {};
      }
      const TypeSimpl typeInfo = transpiler.type_manger_.Add(
          enum_const_decl->getType().getTypePtrOrNull());
      size = typeInfo.GetSizeOfType();
      const std::string name = "c-" + enum_const_decl->getNameAsString();
      const int64_t value = GetInt64Value(enum_const_decl->getInitVal());
      constants.push_back(EnumConstantType{enum_const_decl, name, value});
    }
  }
  return transpiler.enum_manager_.Add(ED, enum_name, size, constants);
}
