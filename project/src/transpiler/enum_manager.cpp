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
