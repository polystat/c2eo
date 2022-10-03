//
// Created by yarrya on 27.09.22.
//

#include "type_manager.h"

#include "clang/AST/ASTContext.h"
#include "vardecl.h"
extern clang::ASTContext* context;
TypeSimpl* TypeManger::GetById(int64_t id) {
  for (const auto& ts : types) {
    if (ts->id == id) {
      return ts;
    }
  }
  return nullptr;
}
TypeSimpl* TypeManger::Add(clang::QualType qual_type) {
  const clang::Type* type_ptr = qual_type.getTypePtr();
  if (type_ptr == nullptr) {
    return nullptr;
  }
  auto id = (int64_t)type_ptr;
  TypeSimpl* existType = GetById(id);
  if (existType != nullptr) {
    return existType;
  }
  TypeSimpl* ts;
  ts->id = id;
  clang::TypeInfo type_info = context->getTypeInfo(type_ptr);
  ts->size = type_info.Width;
  if (type_ptr->isPointerType()) {
    ts->size = 8 * byte_size;  // Size of any pointer == 8 byte
  }
  ts->name = ts->GetTypeName(type_ptr);
  if (type_ptr->isConstantArrayType()) {
    const auto* const ptr_type =
        clang::dyn_cast<clang::ConstantArrayType>(type_ptr);
    ts->subType = Add(ptr_type->getElementType());
  } else if (type_ptr->isPointerType()) {
    const auto* const arr_type = clang::dyn_cast<clang::PointerType>(type_ptr);
    ts->subType = Add(arr_type->getPointeeType());
  }
  types.push_back(ts);
  if (types.empty()) {
    return nullptr;
  }
  return types.back();
}
std::string TypeSimpl::GetTypeName(const clang::Type* type_ptr) const {
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
    const auto* const arr_type =
        clang::dyn_cast<clang::ConstantArrayType>(type_ptr);
    if (arr_type->getElementType()->isCharType()) {
      str += "string";
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
    clang::RecordDecl* RD = type_ptr->getAsRecordDecl();
    if (RD->hasNameForLinkage()) {
      str += RD->getNameAsString();
    } else {
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    }
    return str;
  }

  return "undefinedtype";
}
uint64_t TypeSimpl::GetSizeOfType() const { return size / byte_size; }
uint64_t TypeSimpl::GetSizeOfBaseType() const {
  if (subType != nullptr)
    return subType->GetSizeOfType();
  return  GetSizeOfType();
}
