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
  int64_t subTypeId = -1;
  int64_t recordId = -1;
  TypeSimpl(int64_t _id = -1, std::string _name = "", size_t _size = 0,
            int64_t _subType = -1)
      : id(_id), name(std::move(_name)), size(_size), subTypeId(_subType) {}
  TypeSimpl(int64_t _id, const TypeSimpl& ts)
      : id(_id), name(ts.name), size(ts.size), subTypeId(ts.subTypeId) {}
  std::string GetTypeName(const clang::Type* type_ptr);
  [[nodiscard]] uint64_t GetSizeOfType() const;
  [[nodiscard]] uint64_t GetSizeOfBaseType() const;
};

struct TypeManger {
  TypeSimpl Add(const clang::Type* type_ptr);
  TypeSimpl GetById(int64_t id);

 private:
  std::vector<TypeSimpl> types;
};

#endif  // PROJECT_SRC_TRANSPILER_TYPE_Manager_H