/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_RECORD_MANAGER_H_
#define SRC_TRANSPILER_RECORD_MANAGER_H_

#include <cstdint>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "clang/AST/Decl.h"
#include "src/transpiler/eo_object.h"
#include "src/transpiler/memory_manager.h"

// Representation of a specific type (union or struct)
struct RecordType {
  int64_t id;
  std::string name;
  size_t size;
  std::vector<std::tuple<std::string, TypeSimpl, size_t>> fields;
  std::vector<EOObject> GetEORecordDecl();
  bool is_local;
};

struct RecordManager {
  RecordType Add(int64_t id, std::string name, size_t size,
                 std::vector<std::tuple<std::string, TypeSimpl, size_t>> fields,
                 bool is_local);
  RecordType *GetById(int64_t id);
  [[nodiscard]] std::vector<RecordType>::const_iterator begin() const;
  [[nodiscard]] std::vector<RecordType>::const_iterator end() const;

  bool Empty();
  EOObject GetShiftAlias(int64_t id, const std::string &member);

 private:
  std::vector<RecordType> record_types;
};

#endif  // SRC_TRANSPILER_RECORD_MANAGER_H_
