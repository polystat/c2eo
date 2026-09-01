/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_TYPE_MANAGER_H_
#define SRC_TRANSPILER_TYPE_MANAGER_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/Type.h"

enum class ComplexType {
  NONE,
  ARRAY,
  RECORD,
  POINTER,
  PHANTOM,
};
struct TypeSimpl {
  int64_t id;
  std::string name;
  size_t size;
  int64_t subTypeId = -1;
  int64_t recordId = -1;
  ComplexType typeStyle = ComplexType::NONE;
  explicit TypeSimpl(int64_t _id = -1, std::string _name = "", size_t _size = 0,
                     int64_t _subType = -1, int64_t _recordId = -1,
                     ComplexType _typeStyle = ComplexType::NONE)
      : id(_id),
        name(std::move(_name)),
        size(_size),
        subTypeId(_subType),
        recordId(_recordId),
        typeStyle(_typeStyle) {}
  TypeSimpl(int64_t _id, const TypeSimpl& ts)
      : id(_id),
        name(ts.name),
        size(ts.size),
        subTypeId(ts.subTypeId),
        recordId(ts.recordId),
        typeStyle(ts.typeStyle) {}
  std::string GetTypeName(const clang::Type* type_ptr);
  [[nodiscard]] uint64_t GetSizeOfType() const;
  [[nodiscard]] uint64_t GetSizeOfBaseType() const;
};

struct TypeManger {
  TypeSimpl Add(const clang::Type* type_ptr);
  TypeSimpl GetById(int64_t id, bool isNew = false);

 private:
  std::vector<TypeSimpl> types;
  static const clang::Type* GetSubType(const clang::Type* type_ptr);
};

#endif  // SRC_TRANSPILER_TYPE_MANAGER_H_
