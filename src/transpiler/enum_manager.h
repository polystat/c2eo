/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_ENUM_MANAGER_H_
#define SRC_TRANSPILER_ENUM_MANAGER_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/Decl.h"
#include "src/transpiler/eo_object.h"
#include "src/transpiler/memory_manager.h"

class EnumConstantType {
 public:
  const clang::EnumConstantDecl *id;
  std::string name;
  int64_t value;
};

class EnumType {
 public:
  const clang::EnumDecl *id;
  std::string name;
  size_t size;
  std::vector<EnumConstantType> values;
};

class EnumManager {
 public:
  EnumType Add(const clang::EnumDecl *id, std::string name, size_t size,
               const std::vector<EnumConstantType> &values);

  EnumConstantType *GetConstantById(const clang::EnumConstantDecl *id);

  EnumType *GetById(const clang::EnumDecl *id);

  std::vector<EnumType> enum_types;
};

#endif  // SRC_TRANSPILER_ENUM_MANAGER_H_
