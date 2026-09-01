/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/record_manager.h"

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

RecordType RecordManager::Add(
    int64_t id, std::string name, size_t size,
    std::vector<std::tuple<std::string, TypeSimpl, size_t>> fields,
    bool is_local = false) {
  RecordType record_type = {id, std::move(name), size, std::move(fields),
                            is_local};
  record_types.push_back(record_type);
  return record_type;
}

RecordType *RecordManager::GetById(int64_t id) {
  for (auto it = record_types.begin(); it != record_types.end(); it++) {
    if (it->id == id) {
      return it.base();
    }
  }
  return nullptr;
}

EOObject RecordManager::GetShiftAlias(int64_t id, const std::string &member) {
  RecordType *rt = GetById(id);
  if (rt != nullptr) {
    return EOObject{rt->name + "-" + member, EOObjectType::EO_LITERAL};
  }
  return EOObject{EOObjectType::EO_PLUG};
}

std::vector<RecordType>::const_iterator RecordManager::begin() const {
  return record_types.begin();
}

std::vector<RecordType>::const_iterator RecordManager::end() const {
  return record_types.end();
}

bool RecordManager::Empty() { return record_types.empty(); }

std::vector<EOObject> RecordType::GetEORecordDecl() {
  std::vector<EOObject> record_decl;
  std::string shift;
  for (const auto &field : fields) {
    shift = std::to_string(std::get<2>(field));
    EOObject eo_shift{shift, name + "-" + std::get<0>(field)};
    eo_shift.type = EOObjectType::EO_LITERAL;
    record_decl.push_back(eo_shift);
  }
  return record_decl;
}
