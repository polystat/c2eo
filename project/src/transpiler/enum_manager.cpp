#include "enum_manager.h"

EnumType EnumManager::Add(const clang::EnumDecl *id, std::string name, size_t size, const std::vector<EnumConstantType> &values) {
    EnumType enumType = {id, std::move(name), size, values};
    enum_types.push_back(enumType);
    return enumType;
}

EnumConstantType* EnumManager::GetConstantById(const clang::EnumConstantDecl *id) {
    for (auto type = enum_types.begin(); type != enum_types.end(); type++) {
        for (auto constant = type.base()->values.begin(); constant != type.base()->values.end(); constant++) {
            if (constant->id == id) {
                return constant.base();
            }
        }
    }
    return nullptr;
}
