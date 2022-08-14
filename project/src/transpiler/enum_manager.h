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

#ifndef PROJECT_SRC_TRANSPILER_ENUM_MANAGER_H_
#define PROJECT_SRC_TRANSPILER_ENUM_MANAGER_H_

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

#endif  // PROJECT_SRC_TRANSPILER_ENUM_MANAGER_H_
