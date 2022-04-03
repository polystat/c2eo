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
  const clang::RecordDecl* id;
  size_t size;
  std::string type;
  std::string name;
  std::vector<RecordType> fields;
};


struct RecordManager {
  RecordType Add(const clang::RecordDecl* id, size_t size, std::string type, std::string name,
                 std::vector<RecordType> fields);
//  const RecordType& GetRecordByID(const clang::RecordDecl* id) const;
  RecordType* getById(const clang::RecordDecl *id);

private:
  std::vector<RecordType> record_types;
};

#endif // __RECORD_MANAGER__
