#ifndef C2EO_SRC_TRANSPILER_RECORD_MANAGER_H_
#define C2EO_SRC_TRANSPILER_RECORD_MANAGER_H_

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
  std::map<std::string, std::pair<clang::QualType, size_t>> fields;  // field.name_ -> field.shift
  std::vector<EOObject> GetEORecordDecl();
  bool is_local;
};

struct RecordManager {
  RecordType Add(int64_t id, std::string name, size_t size,
                 std::map<std::string, std::pair<clang::QualType, size_t>> fields, bool is_local);
  RecordType *GetById(int64_t id);
  __attribute__((unused)) size_t GetShift(int64_t id,
                                          const std::string &member);
  [[nodiscard]] std::vector<RecordType>::const_iterator begin() const;
  [[nodiscard]] std::vector<RecordType>::const_iterator end() const;

  bool Empty();
  EOObject GetShiftAlias(int64_t id, const std::string &member);

 private:
  std::vector<RecordType> record_types;
};

#endif  // C2EO_SRC_TRANSPILER_RECORD_MANAGER_H_
