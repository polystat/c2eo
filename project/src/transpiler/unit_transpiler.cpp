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

#include "src/transpiler/unit_transpiler.h"

#include <fstream>
#include <sstream>
#include <utility>

#include "src/transpiler/aliases.h"
#include "src/transpiler/transpile_helper.h"

std::ostream &operator<<(std::ostream &os, UnitTranspiler unit) {
  if (unit.tmp_.empty()) {
    unit.GenerateResult();
  }
  os << unit.tmp_;
  return os;
}

void UnitTranspiler::GenerateResult() {
  EOObject body(EOObjectType::EO_ABSTRACT);

  body.arguments.emplace_back("args...");
  body.postfix = "global";
  body.nested.emplace_back("$", "root");
  body.nested.push_back(glob_.GetEOObject());
  body.nested.emplace_back("memory", "empty-global-position");
  body.nested.push_back(ret_.GetEOObject());
  body.nested.emplace_back("memory", "return-mem_size");
  EOObject ret_address("address", "return");
  ret_address.nested.emplace_back(ret_.name_);
  ret_address.nested.emplace_back("0", EOObjectType::EO_LITERAL);
  body.nested.push_back(ret_address);

  if (!glob_.Empty()) {
    glob_.SetExtEqGlob();
    for (const auto &var : glob_) {
      body.nested.emplace_back(var.GetAddress(glob_.name_));
    }
  }

  if (!record_manager_.Empty()) {  // TODO(nkchuykin): it isn't global, is it?
                                   // it should be out of labels
    for (auto type : record_manager_) {
      if (!type.is_local) {
        auto record_fields = type.GetEORecordDecl();
        body.nested.insert(body.nested.end(), record_fields.begin(),
                           record_fields.end());
      }
    }
  }
  for (const auto &func : func_manager_.GetAllEoDefinitions()) {
    body.nested.push_back(func);
  }

  // call function generation
  func_manager_.ReverseMapToArrayMap();
  // TEST tmp out to see functions
  // func_manager_.TestOut();
  EOObject call{"[index param-start param-size] > call",
                EOObjectType::EO_LITERAL};
  EOObject index_of{"at. > @", EOObjectType::EO_LITERAL};
  EOObject star{"*", EOObjectType::EO_LITERAL};
  for (const auto &func_element : func_manager_.GetFuncArray()) {
    std::string func_name{func_element.second};
    bool find_flag = false;
    auto ext_obj_to_find = FindAllExternalObjects(body);
    for (const auto &ext_func_name : ext_obj_to_find) {
      if (func_name != ext_func_name) {
        continue;
      }
      find_flag = true;
      break;
    }
    if (!find_flag) {
      for (const auto &func : func_manager_.GetAllEoDefinitions()) {
        std::string func_def_name = func.postfix;
        if (func_name != func_def_name) {
          continue;
        }
        find_flag = true;
        break;
      }
    }
    if (!find_flag) {
      func_name = "null-function";
    }
    star.nested.emplace_back(func_name + " param-start param-size",
                             EOObjectType::EO_LITERAL);
  }
  index_of.nested.push_back(star);
  index_of.nested.emplace_back("index", EOObjectType::EO_LITERAL);
  call.nested.push_back(index_of);
  body.nested.push_back(call);

  EOObject init_seq("seq", "@");
  for (const auto &var : glob_) {
    init_seq.nested.push_back(var.GetInitializer());
  }
  if (std::find_if(body.nested.begin(), body.nested.end(),
                   [](const EOObject &x) { return x.postfix == "main"; }) !=
      body.nested.end()) {
    EOObject main_call("main");
    extern UnitTranspiler transpiler;
    main_call.nested.emplace_back(
        std::to_string(transpiler.glob_.GetFreeSpacePointer()),
        EOObjectType::EO_LITERAL);
    main_call.nested.emplace_back("0", EOObjectType::EO_LITERAL);
    init_seq.nested.push_back(main_call);
  }

  init_seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  body.nested.push_back(init_seq);

  std::stringstream result;

  used_external_objects_ = FindAllExternalObjects(body);
  // Null Function Alias Creating
  result << "+alias c2eo.coperators.null-function\n";
  for (const auto &ext_obj : used_external_objects_) {
    if (known_types.find(ext_obj) == known_types.end()) {
      std::string alias;
      try {
        alias = known_aliases.at(ext_obj);
        result << alias << "\n";
      } catch (std::out_of_range &) {
        result << "+alias c2eo.external." << ext_obj << "\n";
        std::string alias_file_name{path_name_ + ext_obj + ".eo.alias"};
        std::ofstream alias_out(alias_file_name);
        alias_out << "+alias c2eo.coperators.printf\n\n"
                  << "+package c2eo.external\n\n"
                  << "[args...] > " << ext_obj << "\n"
                  << "  printf \"" << ext_obj
                  << " is declaration only\\n\" > @";
      }
    }
  }

  result << "\n+package c2eo.src." << package_name_ << "\n";
  result << body;
  tmp_ = result.str();
}

void UnitTranspiler::SetPackageName(std::string package_name) {
  package_name_ = std::move(package_name);
}

void UnitTranspiler::SetPathName(std::string &path_name) {
  path_name_ = path_name;
}

void UnitTranspiler::GenerateMeta() { generate_meta_ = true; }

[[maybe_unused]] bool UnitTranspiler::IsGenerateMeta() const {
  return generate_meta_;
}
