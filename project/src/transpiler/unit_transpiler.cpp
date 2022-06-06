#include "unit_transpiler.h"

#include <fstream>
#include <sstream>

#include "aliases.h"
#include "transpile_helper.h"

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

  if (!record_manager_.Empty()) {  // todo: it isn't global, is it? it should be
                                   // out of labels
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
        std::to_string(transpiler.glob_.RealMemorySize()),
        EOObjectType::EO_LITERAL);
    main_call.nested.emplace_back("0", EOObjectType::EO_LITERAL);
    init_seq.nested.push_back(main_call);
  }

  init_seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  body.nested.push_back(init_seq);

  std::stringstream result;
  result << "+package c2eo.src." << package_name_ << "\n";

  used_external_objects_ = FindAllExternalObjects(body);
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
        alias_out << "+package c2eo.external\n\n"
                  << "+alias c2eo.stdio.printf\n\n"
                  << "[args...] > " << ext_obj << "\n"
                  << "  printf \"" << ext_obj
                  << " is declaration only\\n\" > @";
      }
    }
  }

  result << "\n";

  result << body;
  tmp_ = result.str();
}

void UnitTranspiler::SetPackageName(std::string package_name) {
  package_name_ = std::move(package_name);
}

void UnitTranspiler::SetPathName(std::string path_name) {
  path_name = std::move(path_name);
}
