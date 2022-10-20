#include "type_manager.h"

#include <iostream>

#include "clang/AST/ASTContext.h"
#include "unit_transpiler.h"
#include "vardecl.h"
extern clang::ASTContext* context;
TypeSimpl TypeManger::GetById(int64_t id) {
  for (const auto& ts : types) {
    if (ts.id == id) {
      return ts;
    }
  }
  return {};
}
TypeSimpl TypeManger::Add(const clang::Type* type_ptr) {
  if (type_ptr == nullptr) {
    return {};
  }
  auto id = (int64_t)type_ptr;
  TypeSimpl existType = GetById(id);
  if (existType.id != -1) {
    return existType;
  }
  TypeSimpl ts;
  if (type_ptr->isTypedefNameType()) {
    ts = TypeSimpl(id, Add(type_ptr->getUnqualifiedDesugaredType()));
  } else {
    ts.id = id;
    clang::TypeInfo type_info = context->getTypeInfo(type_ptr);
    ts.size = type_info.Width;
    if (type_ptr->isPointerType()) {
      ts.size = 8 * byte_size;  // Size of any pointer == 8 byte
    }
    ts.name = ts.GetTypeName(type_ptr);
    if (type_ptr->isConstantArrayType() || type_ptr->isPointerType()) {
      ts.subTypeId = Add(type_ptr->getPointeeOrArrayElementType()).id;
    }
  }
  types.push_back(ts);
  if (types.empty()) {
    return {};
  };
  return types.back();
}
std::string TypeSimpl::GetTypeName(const clang::Type* type_ptr) {
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
    } else {
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
    clang::RecordDecl* RD = type_ptr->getAsRecordDecl();
    recordId = (int64_t)RD->getID();
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
  return size / byte_size; }
uint64_t TypeSimpl::GetSizeOfBaseType() const {
  if (subTypeId != -1) {
    extern UnitTranspiler transpiler;
    return transpiler.type_manger_.GetById(subTypeId).GetSizeOfType();
  }
  return GetSizeOfType();
}