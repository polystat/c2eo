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

#include "src/transpiler/type_manager.h"

#include <iostream>

#include "clang/AST/ASTContext.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/vardecl.h"
extern clang::ASTContext* context;
TypeSimpl TypeManger::GetById(int64_t id) {
  for (const auto& ts : types) {
    if (ts.id == id) {
      return ts;
    }
  }
  return TypeSimpl();
}
TypeSimpl TypeManger::Add(const clang::Type* type_ptr) {
  if (type_ptr == nullptr) {
    return TypeSimpl();
  }
  auto id = reinterpret_cast<intptr_t>(type_ptr);
  TypeSimpl existType = GetById(id);
  if (existType.id != -1) {
    return existType;
  }
  TypeSimpl ts;
  if (type_ptr->isTypedefNameType()) {
    ts = TypeSimpl(id, Add(type_ptr->getUnqualifiedDesugaredType()));
  } else {
    ts.id = id;
    const clang::TypeInfo type_info = context->getTypeInfo(type_ptr);
    ts.size = type_info.Width;
    if (type_ptr->isPointerType()) {
      ts.size = 8UL * byte_size;  // Size of any pointer == 8 byte
    }
    ts.name = ts.GetTypeName(type_ptr);
    const clang::Type* sub_type_ptr = GetSubType(type_ptr);
    if (sub_type_ptr != nullptr) {
      ts.subTypeId = Add(sub_type_ptr).id;
    }
  }
  types.push_back(ts);
  return ts;
}
const clang::Type* TypeManger::GetSubType(const clang::Type* type_ptr) {
  if (type_ptr->isPointerType()) {
    return type_ptr->getPointeeType().getTypePtr();
  }
  if (type_ptr->isConstantArrayType()) {
    return type_ptr->getArrayElementTypeNoTypeQual();
  }
  return nullptr;
}
std::string TypeSimpl::GetTypeName(const clang::Type* type_ptr) {
  if (type_ptr == nullptr) {
    return "undefinedtype";
  }
  std::string str;
  if (type_ptr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (type_ptr->isPointerType()) {
    str += "ptr";
    return str;
  }
  if (type_ptr->isConstantArrayType()) {
    isArray = true;
    const auto* array_type =
        clang::dyn_cast<clang::ConstantArrayType>(type_ptr);
    if (array_type != nullptr) {
      const clang::Type* element_type =
          array_type->getElementType().getTypePtrOrNull();
      if (element_type != nullptr && element_type->isCharType()) {
        str += "string";
        return str;
      }
      str += "array";
      return str;
    }
  }

  if (type_ptr->isFloatingType()) {
    str += "float" + std::to_string(size);
    return str;
  }

  if (!type_ptr->isSignedIntegerType()) {
    str += "u";
  }
  if (type_ptr->isIntegerType()) {
    str += "int" + std::to_string(size);
    return str;
  }

  if (type_ptr->isUnionType()) {
    str = "un-";
  }
  if (type_ptr->isStructureType()) {
    str = "st-";
  }
  if (type_ptr->isUnionType() || type_ptr->isStructureType()) {
    isRecord = true;
    clang::RecordDecl* RD = type_ptr->getAsRecordDecl();
    recordId = RD->getID();
    if (RD->hasNameForLinkage()) {
      str += RD->getNameAsString();
    } else {
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    }
    return str;
  }

  return "undefinedtype";
}
uint64_t TypeSimpl::GetSizeOfType() const {
  if (name == "float32" || name == "ptr") {
    return 8;  // 8 bytes for float32.
  }
  return size / byte_size;
}
uint64_t TypeSimpl::GetSizeOfBaseType() const {
  if (subTypeId != -1) {
    extern UnitTranspiler transpiler;
    return transpiler.type_manger_.GetById(subTypeId).GetSizeOfType();
  }
  return GetSizeOfType();
}
