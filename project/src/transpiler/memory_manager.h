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

#ifndef PROJECT_SRC_TRANSPILER_MEMORY_MANAGER_H_
#define PROJECT_SRC_TRANSPILER_MEMORY_MANAGER_H_

static const int some_non_zero_position = 999999;
static const int eight_kilobytes = 8192;
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "src/transpiler/eo_object.h"

// Representation of a simple variable stored in RAM
struct Variable {
  const clang::VarDecl *id;
  size_t position;
  size_t size;
  // like c-int64
  std::string type;
  // bool isCustomType = false;
  // like g-x
  std::string alias;
  EOObject value;
  // address of local memory start in global
  std::string local_pointer;
  size_t shift;
  // only -int64
  std::string type_postfix;
  bool is_initialized;

  bool operator==(const Variable &var) const;

  [[nodiscard]] EOObject GetInitializer() const;

  [[nodiscard]] EOObject GetAddress(const std::string &mem_name) const;
};

class MemoryManager {
 public:
  [[maybe_unused]] explicit MemoryManager(std::string name,
                                          size_t start_pointer = 8)
      : name_(std::move(name)), pointer_(start_pointer) {}

  Variable Add(const clang::VarDecl *id, size_t size, const std::string &type,
               std::string alias, EOObject value, std::string local_name = "",
               size_t shift = 0, bool is_initialized = true);

  Variable AddExternal(const clang::VarDecl *id, size_t size,
                       const std::string &type, std::string alias,
                       EOObject value, std::string local_name = "",
                       size_t shift = 0,
                       __attribute__((unused)) bool is_initialized = false);

  bool Empty();

  [[nodiscard]] size_t GetFreeSpacePointer() const;

  const Variable &GetVarById(const clang::VarDecl *id) const;

  [[nodiscard]] std::vector<Variable>::const_iterator begin() const;

  [[nodiscard]] std::vector<Variable>::const_iterator end() const;

  std::string name_;

  [[nodiscard]] EOObject GetEOObject() const;

  void RemoveAllUsed(const std::vector<Variable> &all_local);

  void SetExtEqGlob();

 private:
  // index of first free byte in memory
  size_t pointer_;
  int mem_size_ = eight_kilobytes;
  std::vector<Variable> variables_;
  std::map<std::string, int> duplicates;
};

#endif  // PROJECT_SRC_TRANSPILER_MEMORY_MANAGER_H_
