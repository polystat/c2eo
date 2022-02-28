#include <sstream>
#include "unit_transpiler.h"


std::ostream &operator<<(std::ostream &os, UnitTranspiler unit) {
  if (unit.tmp.empty())
    unit.GenerateResult();
  os << unit.tmp;
  return os;
}


void UnitTranspiler::GenerateResult() {
  EOObject body(EOObjectType::EO_ABSTRACT);
  body.arguments.emplace_back("args...");
  body.postfix = "global";
  body.nested.push_back(glob.GetEOObject());
  body.nested.emplace_back("memory","empty-global-position");
  body.nested.push_back(ret.GetEOObject());
  body.nested.emplace_back("memory","return-mem_size");
  EOObject ret_addr("address","return");
  ret_addr.nested.emplace_back(ret.name);
  ret_addr.nested.emplace_back("0");
  body.nested.push_back(ret_addr);

  if(!glob.Empty()){
    for(const auto& var : glob){
      body.nested.emplace_back(var.GetAdress(glob.name));
    }
  }

  for(const auto& func : func_manager){
    body.nested.push_back(func.GetEOObject());
  }

  EOObject init_seq("seq","@");
  for(const auto& var : glob){
    init_seq.nested.push_back(var.GetInitializer());
  }
  init_seq.nested.emplace_back("TRUE");

  body.nested.push_back(init_seq);




  std::stringstream result;
  result << "+package c2eo.src.temporary_name\n\n";
  //TODO Make correct alias generation
  /*for(const auto& alias : used_external_objects)
  {
    header << "+alias "<<alias<<"\n";
  }

  header << "\n"; */

  result <<  body;
  tmp = result.str();
}
