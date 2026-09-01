/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_UNIT_TRANSPILER_H_
#define SRC_TRANSPILER_UNIT_TRANSPILER_H_

#include "iostream"
#include "set"
#include "src/transpiler/enum_manager.h"
#include "src/transpiler/function_manager.h"
#include "src/transpiler/memory_manager.h"
#include "src/transpiler/record_manager.h"
#include "src/transpiler/type_manager.h"
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
  TypeManger type_manger_;
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

#endif  // SRC_TRANSPILER_UNIT_TRANSPILER_H_
