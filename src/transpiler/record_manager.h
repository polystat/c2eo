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

#ifndef PROJECT_SRC_TRANSPILER_RECORD_MANAGER_H_
#define PROJECT_SRC_TRANSPILER_RECORD_MANAGER_H_

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

#endif  // PROJECT_SRC_TRANSPILER_RECORD_MANAGER_H_
