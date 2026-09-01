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

#include "src/transpiler/eo_object.h"

#include <utility>

#include "src/transpiler/util.h"

EOObject::EOObject(EOObjectType type) : type(type) {
  // nested.reserve(10000);
#ifdef TRACEOUT_NEW_EO
  std::cout << *this;  // << "\n";
#endif
}

// Create simple complete Object
EOObject::EOObject(std::string name)
    : name(std::move(name)), type(EOObjectType::EO_COMPLETE) {
  // nested.reserve(10000);
#ifdef TRACEOUT_NEW_EO
  std::cout << *this;  // << "\n";
#endif
}

// Create simple object, may be used for literal
EOObject::EOObject(std::string name, EOObjectType type)
    : name(std::move(name)), type(type) {
#ifdef TRACEOUT_NEW_EO
  std::cout << *this;  // << "\n";
#endif
}

// create complete name_ with body
EOObject::EOObject(std::string name, std::string postfix)
    : name(std::move(name)),
      postfix(std::move(postfix)),
      type(EOObjectType::EO_COMPLETE) {
  // nested.reserve(10000);
#ifdef TRACEOUT_NEW_EO
  std::cout << *this;  // << "\n";
#endif
}

__attribute__((unused)) void EOObject::AddNested(const EOObject &obj) {
  nested.push_back(obj);
}

auto EOObject::GetSpaceIndent() {
  return std::string(
      static_cast<std::basic_string<char>::size_type>(2 * EOObject::indent),
      ' ');
}

std::ostream &operator<<(std::ostream &os, const EOObject &obj) {
  if (obj.type == EOObjectType::EO_EMPTY) {
    for (const auto &child : obj.nested) {
      os << child;
    }
    return os;
  }
  os << EOObject::GetSpaceIndent();
  if (obj.type == EOObjectType::EO_PLUG) {
    os << "plug"
       << "\n";
    return os;
  }
  if (obj.type == EOObjectType::EO_ABSTRACT) {
    os << "[" << StrJoin(obj.arguments) << "]";
  } else {
    if (!obj.prefix.empty()) {
      os << obj.prefix << ".";
    }
    os << obj.name;
  }
  if (!obj.postfix.empty()) {
    os << " > " << obj.postfix;
  }
  os << "\n";
  if (!obj.nested.empty()) {
    EOObject::indent++;
    for (const auto &child : obj.nested) {
      os << child;
    }
    EOObject::indent--;
  }
  return os;
}
