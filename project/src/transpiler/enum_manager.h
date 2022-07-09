#ifndef C2EO_ENUM_MANAGER_H
#define C2EO_ENUM_MANAGER_H

#include <cstdint>
#include <string>
#include <utility>

#include "clang/AST/Decl.h"
#include "eo_object.h"
#include "memory_manager.h"

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
    EnumType Add(const clang::EnumDecl *id, std::string name, size_t size, const std::vector<EnumConstantType> &values);

    EnumConstantType* GetConstantById(const clang::EnumConstantDecl *id);

    std::vector<EnumType> enum_types;
};


#endif //C2EO_ENUM_MANAGER_H
