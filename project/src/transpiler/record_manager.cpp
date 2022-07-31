/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "src/transpiler/record_manager.h"

#include <map>
#include <vector>

RecordType RecordManager::Add(
    int64_t id, std::string name, size_t size,
    std::map<std::string, std::pair<clang::QualType, size_t>> fields,
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

__attribute__((unused)) size_t RecordManager::GetShift(
    int64_t id, const std::string &member) {
  RecordType *rt = GetById(id);
  if (rt != nullptr) {
    return rt->fields[member].second;
  }
  return -1;
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
    shift = std::to_string(field.second.second);
    EOObject eo_shift{shift, name + "-" + field.first};
    eo_shift.type = EOObjectType::EO_LITERAL;
    record_decl.push_back(eo_shift);
  }
  return record_decl;
}
