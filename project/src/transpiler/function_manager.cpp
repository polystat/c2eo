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

#include "src/transpiler/function_manager.h"

#include <iostream>
#include <map>
#include <vector>

#include "src/transpiler/transpile_helper.h"

//==============================================================================
EOObject FunctionDefinition::GetEoObject() const {
  EOObject func_object(EOObjectType::EO_ABSTRACT);
  func_object.postfix = name;
  func_object.arguments = {"param-start", "param-size"};
  func_object.nested.push_back(GetBody());
  return func_object;
}

EOObject FunctionDefinition::GetBody() const { return GetFunctionBody(FD); }

void FunctionDefinition::TestOut() const { std::cout << name << "\n"; }

void FunctionDeclaration::TestOut() const { std::cout << name << "\n"; }

//------------------------------------------------------------------------------
void FunctionManager::AddDefinition(const FunctionDefinition &func_def) {
  definitions.push_back(func_def);
}

//------------------------------------------------------------------------------
void FunctionManager::AddDeclaration(const FunctionDeclaration &func_decl) {
  declarations.push_back(func_decl);
}
//------------------------------------------------------------------------------
void FunctionManager::AddEoObject(const EOObject &func) {
  functions.push_back(func);
}
//------------------------------------------------------------------------------
void FunctionManager::AddToMap(std::string *func_name) {
  if (func_name_map.find(*func_name) == func_name_map.end()) {
    if (*func_name == "f-printf") {
      func_name_map["printf"] = name_count++;
    } else {
      func_name_map[*func_name] = name_count++;
    }
  }
}
//------------------------------------------------------------------------------
void FunctionManager::ReverseMapToArrayMap() {
  for (const auto &func_pair : func_name_map) {
    func_name_map_as_array[func_pair.second] = func_pair.first;
  }
}
//------------------------------------------------------------------------------
const std::vector<EOObject> &FunctionManager::GetAllEoDefinitions() {
  return functions;
}
//------------------------------------------------------------------------------
const std::vector<FunctionDeclaration>
    &FunctionManager::GetAllFuncDeclarations() {
  return declarations;
}
//------------------------------------------------------------------------------
const std::vector<FunctionDefinition>
    &FunctionManager::GetAllFuncDefinitions() {
  return definitions;
}
//------------------------------------------------------------------------------
std::map<int, std::string> &FunctionManager::GetFuncArray() {
  return func_name_map_as_array;
}
//------------------------------------------------------------------------------
EOObject FunctionManager::GetFunctionCall(const clang::FunctionDecl *FD,
                                          size_t param_size) const {
  auto res_def = std::find_if(
      definitions.begin(), definitions.end(),
      [&FD](const FunctionDefinition &decl) { return decl.FD == FD; });
  if (res_def != definitions.end()) {
    EOObject call(res_def->name);
    call.prefix = "root";
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size),
                             EOObjectType::EO_LITERAL);
    return call;
  }
  auto res_decl = std::find_if(
      declarations.begin(), declarations.end(),
      [&FD](const FunctionDeclaration &decl) { return decl.FD == FD; });
  if (res_decl != declarations.end()) {
    EOObject call(res_decl->name);
    if (res_decl->FD->getDefinition() != nullptr) {
      call.prefix = "root";
    }
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size),
                             EOObjectType::EO_LITERAL);
    return call;
  }
  // Kernigan default declaration
  auto func_name{FD->getNameAsString()};
  EOObject call("f-" + func_name);
  call.prefix = "root";
  call.nested.emplace_back("empty-local-position");
  call.nested.emplace_back(std::to_string(param_size),
                           EOObjectType::EO_LITERAL);
  return call;
  // return EOObject(EOObjectType::EO_PLUG);
}

//------------------------------------------------------------------------------
std::string FunctionManager::GetEOFunctionName(
    const clang::FunctionDecl *FD) const {
  auto res_def = std::find_if(
      definitions.begin(), definitions.end(),
      [&FD](const FunctionDefinition &decl) { return decl.FD == FD; });
  if (res_def != definitions.end()) {
    return res_def->name;
  }
  auto res_decl = std::find_if(
      declarations.begin(), declarations.end(),
      [&FD](const FunctionDeclaration &decl) { return decl.FD == FD; });
  if (res_decl != declarations.end()) {
    return res_decl->name;
  }
  return "no-function-name";
}

__attribute__((unused)) void FunctionManager::TestOut() {
  if (!declarations.empty()) {
    std::cout << "Declarations:\n";
    for (const auto &declaration : declarations) {
      declaration.TestOut();
    }
  } else {
    std::cout << "No function declarations\n";
  }
  if (!definitions.empty()) {
    std::cout << "Definitions:\n";
    for (const auto &definition : definitions) {
      definition.TestOut();
    }
  } else {
    std::cout << "No function definitions\n";
  }
  if (!func_name_map.empty()) {
    std::cout << "Function names in map:\n";
    for (const auto &func_pair : func_name_map) {
      std::cout << func_pair.first << "[" << func_pair.second << "]\n";
    }
    std::cout << "Function names in array:\n";
    for (const auto &func_pair : func_name_map_as_array) {
      std::cout << func_pair.first << ": " << func_pair.second << "\n";
    }
  } else {
    std::cout << "No names in function map\n";
  }
}
