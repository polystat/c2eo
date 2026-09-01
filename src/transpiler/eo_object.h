/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_EO_OBJECT_H_
#define SRC_TRANSPILER_EO_OBJECT_H_

#include <ostream>
#include <string>
#include <vector>

enum class EOObjectType {
  EO_EMPTY,
  EO_COMPLETE,
  EO_LITERAL,
  EO_ABSTRACT,
  EO_PLUG,
  EO_TEMPLATE,
};

struct EOObject {
 public:
  EOObject() = default;

  explicit EOObject(EOObjectType type);

  // Create simple complete Object
  explicit EOObject(std::string name);

  // Create simple object, may be used for literal
  EOObject(std::string name, EOObjectType type);

  // create complete name_ with body
  EOObject(std::string name, std::string postfix);

  // Add nested object to vector of nested
  __attribute__((unused)) void AddNested(const EOObject &obj);

  std::vector<std::string> arguments;
  std::string name;
  std::string prefix;
  std::string postfix;
  EOObjectType type = EOObjectType::EO_EMPTY;
  std::vector<EOObject> nested;

  friend std::ostream &operator<<(std::ostream &os, const EOObject &obj);

 private:
  static auto GetSpaceIndent();
  inline static int indent = 0;
};

#endif  // SRC_TRANSPILER_EO_OBJECT_H_
