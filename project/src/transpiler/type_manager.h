//
// Created by yarrya on 27.09.22.
//

#ifndef PROJECT_SRC_TRANSPILER_TYPE_Manager_H
#define PROJECT_SRC_TRANSPILER_TYPE_Manager_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/Type.h"

struct TypeSimpl {
  int64_t id;
  std::string name;
  size_t size;
  TypeSimpl* subType = nullptr;
  TypeSimpl(int64_t _id, std::string _name, size_t _size,
            TypeSimpl* _subType = nullptr)
      : id(_id), name(std::move(_name)), size(_size), subType(_subType) {}
  std::string GetTypeName(const clang::Type *type_ptr) const;
  [[nodiscard]] uint64_t GetSizeOfType() const;
  [[nodiscard]] uint64_t GetSizeOfBaseType() const;
};

struct TypeManger {
 TypeSimpl* Add(clang::QualType newType);
 TypeSimpl* GetById(int64_t id);
 private:
  std::vector<TypeSimpl*> types;
};

#endif  // PROJECT_SRC_TRANSPILER_TYPE_Manager_H
