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
  int64_t id = RD->getID();
  RecordType *existed = transpiler.record_manager_.GetById(id);
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
  //  name += std::to_string(id);
  uint64_t size = 0;

  std::vector<std::tuple<std::string, clang::QualType, size_t>> fields;
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
      fields.emplace_back(field_name, field->getType(), shift);

      clang::QualType qual_type = field->getType();
      clang::TypeInfo type_info = field->getASTContext().getTypeInfo(qual_type);
      if (RD->isStruct()) {
        shift += type_info.Width / byte_size;
        size = shift;
      } else {
        size = std::max(size, type_info.Width / byte_size);
      }
    }
  }
  types.push_back(
      transpiler.record_manager_.Add(id, name, size, fields, is_local));
  return types;
}
