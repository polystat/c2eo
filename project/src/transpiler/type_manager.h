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

#ifndef PROJECT_SRC_TRANSPILER_TYPE_MANAGER_H_
#define PROJECT_SRC_TRANSPILER_TYPE_MANAGER_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/Type.h"

struct TypeSimpl {
  int64_t id;
  std::string name;
  size_t size;
  int64_t subTypeId = -1;
  int64_t recordId = -1;
  bool isArray = false;
  bool isRecord = false;
  explicit TypeSimpl(int64_t _id = -1, std::string _name = "", size_t _size = 0,
                     int64_t _subType = -1, int64_t _recordId = -1,
                     bool _isArray = false, bool _isRecord = false)
      : id(_id),
        name(std::move(_name)),
        size(_size),
        subTypeId(_subType),
        recordId(_recordId),
        isArray(_isArray),
        isRecord(_isRecord) {}
  TypeSimpl(int64_t _id, const TypeSimpl& ts)
      : id(_id),
        name(ts.name),
        size(ts.size),
        subTypeId(ts.subTypeId),
        recordId(ts.recordId),
        isArray(ts.isArray),
        isRecord(ts.isRecord) {}
  std::string GetTypeName(const clang::Type* type_ptr);
  [[nodiscard]] uint64_t GetSizeOfType() const;
  [[nodiscard]] uint64_t GetSizeOfBaseType() const;
};

struct TypeManger {
  TypeSimpl Add(const clang::Type* type_ptr, bool addSubs = false);
  TypeSimpl GetById(int64_t id);

 private:
  std::vector<TypeSimpl> types;
  static const clang::Type* GetSubType(const clang::Type* type_ptr);
};

#endif  // PROJECT_SRC_TRANSPILER_TYPE_MANAGER_H_
