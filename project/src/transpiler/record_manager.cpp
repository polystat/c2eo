#include "record_manager.h"

RecordType RecordManager::Add(const clang::RecordDecl* id, std::string name,
                              std::map<std::string, size_t> fields) {
  RecordType recordType = {id, std::move(name), std::move(fields)};
  record_types.push_back(recordType);
  return recordType;
}

RecordType* RecordManager::getById(const clang::RecordDecl* id) {
  for(auto it = record_types.begin(); it != record_types.end(); it++)
    if(it->id == id)
      return it.base();
  return nullptr;
}

size_t RecordManager::getShift(const clang::RecordDecl* id, const std::string& member) {
  RecordType* rt = getById(id);
  if(rt)
    return rt->fields[member];
  return -1;
}

EOObject RecordManager::getShiftAlias(const clang::RecordDecl* id, const std::string& member) {
  RecordType* rt = getById(id);
  if(rt)
    return EOObject{rt->name + "-" + member};
  return EOObject{EOObjectType::EO_PLUG};
}

std::vector<RecordType>::const_iterator RecordManager::begin() const {
  return record_types.begin();
}

std::vector<RecordType>::const_iterator RecordManager::end() const {
  return record_types.end();
}

bool RecordManager::Empty() {
  return record_types.empty();
}

std::vector<EOObject> RecordType::GetEORecordDecl() {
  std::vector<EOObject> recordDecl;
  for(const auto& field:fields) {
    recordDecl.emplace_back(std::to_string(field.second), name + "-" + field.first);
  }
  return recordDecl;
}
