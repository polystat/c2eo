/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/recorddecl.h"

#include <algorithm>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/vardecl.h"

std::vector<RecordType> ProcessRecordType(const clang::RecordDecl *RD,
                                          bool is_local) {
  if (RD == nullptr) {
    return {};
  }
  extern UnitTranspiler transpiler;
  std::vector<RecordType> types;
  RecordType *existed = transpiler.record_manager_.GetById(RD->getID());
  if (existed != nullptr) {
    return {};
  }

  std::string name;
  if (RD->isUnion()) {
    name = "un-";
  }
  if (RD->isStruct()) {
    name = "st-";
  }
  if (RD->hasNameForLinkage() && !RD->getNameAsString().empty()) {
    name += RD->getNameAsString();
  } else {
    name += std::to_string(reinterpret_cast<uint64_t>(RD));
  }

  uint64_t size = 0;

  std::vector<std::tuple<std::string, TypeSimpl, size_t>> fields;
  size_t shift = 0;

  for (auto it = RD->decls_begin(); it != RD->decls_end(); it++) {
    if (it->getKind() == clang::Decl::Record) {
      auto new_types =
          ProcessRecordType(llvm::dyn_cast<clang::RecordDecl>(*it), is_local);
      types.insert(types.end(), new_types.begin(), new_types.end());
    } else if (it->getKind() == clang::Decl::Field) {
      auto *field = llvm::dyn_cast<clang::FieldDecl>(*it);

      if (field == nullptr) {
        continue;
      }
      std::string field_name;
      if (!field->isUnnamedBitfield() && !field->getNameAsString().empty()) {
        field_name = /* "f-" + */ field->getNameAsString();
      } else {
        field_name = "field" + std::to_string(field->getID());
      }
      const TypeSimpl typeInfo =
          transpiler.type_manger_.Add(field->getType().getTypePtrOrNull());
      fields.emplace_back(field_name, typeInfo, shift);

      if (RD->isStruct()) {
        shift += typeInfo.GetSizeOfType();
        size = shift;
      } else {
        size = std::max(size, typeInfo.GetSizeOfType());
      }
    }
  }
  types.push_back(transpiler.record_manager_.Add(RD->getID(), name, size,
                                                 fields, is_local));
  return types;
}
