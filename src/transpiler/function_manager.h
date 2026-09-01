/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_FUNCTION_MANAGER_H_
#define SRC_TRANSPILER_FUNCTION_MANAGER_H_

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/Decl.h"
#include "src/transpiler/eo_object.h"
#include "src/transpiler/memory_manager.h"

// Representation of a simple function
struct FunctionDefinition {
  const clang::FunctionDecl *FD;
  std::string name;

  [[nodiscard]] EOObject GetEoObject() const;
  void TestOut() const;

 private:
  [[nodiscard]] EOObject GetBody() const;
};

struct FunctionDeclaration {
  const clang::FunctionDecl *FD;
  std::string name;

  void TestOut() const;
};

struct FunctionManager {
  void AddDefinition(const FunctionDefinition &func_def);
  void AddDeclaration(const FunctionDeclaration &func_decl);
  void AddEoObject(const EOObject &func);

  void AddToMap(std::string *func_name);
  int GetMapIndex(std::string *func_name) { return func_name_map[*func_name]; }
  void ReverseMapToArrayMap();

  const std::vector<EOObject> &GetAllEoDefinitions();

  EOObject GetFunctionCall(const clang::FunctionDecl *FD,
                           size_t param_size) const;
  std::string GetEOFunctionName(const clang::FunctionDecl *FD) const;

  std::map<int, std::string> &GetFuncArray();

  void SetNameCount(int value) { name_count = value; }

  const std::vector<FunctionDeclaration> &GetAllFuncDeclarations();
  const std::vector<FunctionDefinition> &GetAllFuncDefinitions();

  __attribute__((unused)) void TestOut();

 private:
  std::vector<FunctionDefinition> definitions;
  std::vector<FunctionDeclaration> declarations;
  std::vector<EOObject> functions;
  std::map<std::string, int> func_name_map;
  std::map<int, std::string> func_name_map_as_array;
  int name_count;
};

#endif  // SRC_TRANSPILER_FUNCTION_MANAGER_H_
