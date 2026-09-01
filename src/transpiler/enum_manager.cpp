/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/enum_manager.h"

EnumType EnumManager::Add(const clang::EnumDecl *id, std::string name,
                          size_t size,
                          const std::vector<EnumConstantType> &values) {
  //    llvm::outs() << "enum " << name << "\n";
  //    for (auto val : values) {
  //        llvm::outs() << "enum const " << val.name << " " << val.value <<
  //        "\n";
  //    }
  EnumType enumType = {id, std::move(name), size, values};
  enum_types.push_back(enumType);
  return enumType;
}

EnumConstantType *EnumManager::GetConstantById(
    const clang::EnumConstantDecl *id) {
  for (auto type = enum_types.begin(); type != enum_types.end(); type++) {
    for (auto constant = type.base()->values.begin();
         constant != type.base()->values.end(); constant++) {
      if (constant->id == id) {
        return constant.base();
      }
    }
  }
  return nullptr;
}

EnumType *EnumManager::GetById(const clang::EnumDecl *id) {
  for (auto type = enum_types.begin(); type != enum_types.end(); type++) {
    if (type->id == id) {
      return type.base();
    }
  }
  return nullptr;
}
