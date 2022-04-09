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
  const clang::Type* id;
  std::string name;
  std::map<std::string, size_t> fields; // field.name -> field.shift
  std::vector<EOObject> GetEORecordDecl() ;
};


struct RecordManager {
  RecordType Add(const clang::Type* id, std::string name,
                 std::map<std::string, size_t> fields);
  RecordType* getById(const clang::Type* id);
  size_t getShift(const clang::Type* id, const std::string& member);
  std::vector<RecordType>::const_iterator begin() const;
  std::vector<RecordType>::const_iterator end() const;

  bool Empty();

private:
  std::vector<RecordType> record_types;
};

#endif // __RECORD_MANAGER__
