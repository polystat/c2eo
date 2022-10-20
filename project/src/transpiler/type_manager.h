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
  bool isArray = false;
  bool isRecord = false;
  TypeSimpl(int64_t _id = -1, std::string _name = "", size_t _size = 0,
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
  TypeSimpl Add(const clang::Type* type_ptr);
  TypeSimpl GetById(int64_t id);

 private:
  std::vector<TypeSimpl> types;
  const clang::Type* GetSubType(const clang::Type* pType);
};

#endif  // PROJECT_SRC_TRANSPILER_TYPE_Manager_H