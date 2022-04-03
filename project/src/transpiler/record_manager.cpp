#include "record_manager.h"

RecordType RecordManager::Add(const clang::RecordDecl* id, size_t size, std::string type, std::string name,
                              std::vector<RecordType> fields) {
  RecordType recordType = {id, size, std::move(type), std::move(name), std::move(fields)};
  record_types.push_back(recordType);
  return recordType;
}

RecordType* RecordManager::getById(const clang::RecordDecl *id) {
  for(auto it = record_types.begin(); it != record_types.end(); it++)
    if(it->id == id)
      return it.base();
  return nullptr;
}
