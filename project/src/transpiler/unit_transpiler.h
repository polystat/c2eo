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

#ifndef PROJECT_SRC_TRANSPILER_UNIT_TRANSPILER_H_
#define PROJECT_SRC_TRANSPILER_UNIT_TRANSPILER_H_

#include "iostream"
#include "set"
#include "src/transpiler/enum_manager.h"
#include "src/transpiler/function_manager.h"
#include "src/transpiler/memory_manager.h"
#include "src/transpiler/record_manager.h"
#include "string"

class UnitTranspiler {
 public:
  //   UnitTranspiler() = default;
  UnitTranspiler() {
    func_manager_.SetNameCount(0);
    std::string tmp_str{"null-function"};
    func_manager_.AddToMap(&tmp_str);
  }

  // MemoryManager glob_{"global-ram", 8}, ret_{"return-ram"};
  MemoryManager glob_{"global-ram"}, ret_{"return-ram"};
  FunctionManager func_manager_;
  RecordManager record_manager_;
  EnumManager enum_manager_;
  friend std::ostream &operator<<(std::ostream &os, UnitTranspiler unit);

  std::string name_;
  std::set<std::string> used_external_objects_;

  void SetPackageName(std::string package_name);
  //   static void SetPathName(std::string path_name);
  void SetPathName(std::string &path_name);
  // std::vector<EOObject>

  void GenerateMeta();
  [[maybe_unused]] [[nodiscard]] bool IsGenerateMeta() const;

 private:
  std::string package_name_;
  std::string path_name_;
  std::string tmp_;
  // std::vector<EOObject> objects_;

  void GenerateResult();
  bool generate_meta_ = false;
};

#endif  // PROJECT_SRC_TRANSPILER_UNIT_TRANSPILER_H_
