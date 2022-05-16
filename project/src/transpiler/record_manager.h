#ifndef __RECORD_MANAGER__
#define __RECORD_MANAGER__

#include <cstdint>
#include <string>
#include <utility>
#include "clang/AST/Decl.h"
#include "eo_object.h"
#include "memory_manager.h"

// Representation of a specific type (union or struct)
struct RecordType {
  int64_t id;
  std::string name;
  size_t size;
  std::map<std::string, size_t> fields; // field.name -> field.shift
  std::vector<EOObject> GetEORecordDecl() ;
  bool is_local;
};


struct RecordManager {
  RecordType Add(int64_t id, std::string name, size_t size,
                 std::map<std::string, size_t> fields, bool is_local);
  RecordType* getById(int64_t id);
  size_t getShift(int64_t id, const std::string& member);
  std::vector<RecordType>::const_iterator begin() const;
  std::vector<RecordType>::const_iterator end() const;

  bool Empty();
  EOObject getShiftAlias(int64_t id, const std::string &member);
private:
  std::vector<RecordType> record_types;
};

#endif // __RECORD_MANAGER__
