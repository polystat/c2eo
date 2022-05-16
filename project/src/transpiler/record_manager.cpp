#include "record_manager.h"

RecordType RecordManager::Add(int64_t id, std::string name, size_t size,
                              std::map<std::string, size_t> fields, bool is_local=false) {
  RecordType recordType = {id, std::move(name), size,std::move(fields), is_local};
  record_types.push_back(recordType);
  return recordType;
}

RecordType* RecordManager::getById(int64_t id) {
  for(auto it = record_types.begin(); it != record_types.end(); it++)
    if(it->id == id)
      return it.base();
  return nullptr;
}

size_t RecordManager::getShift(int64_t id, const std::string& member) {
  RecordType* rt = getById(id);
  if(rt)
    return rt->fields[member];
  return -1;
}

EOObject RecordManager::getShiftAlias(int64_t id, const std::string& member) {
  RecordType* rt = getById(id);
  if(rt)
    return EOObject{rt->name + "-" + member, EOObjectType::EO_LITERAL}; // todo: is it EO_LITERAL?
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
  std::string shift;
  for(const auto& field:fields) {
    shift = std::to_string(field.second);
    EOObject eoShift{shift, name + "-" + field.first};
    eoShift.type = EOObjectType::EO_LITERAL;
//    recordDecl.emplace_back(shift, EOObjectType::EO_LITERAL);
    recordDecl.push_back(eoShift);
  }
  return recordDecl;
}
