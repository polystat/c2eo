#include <sstream>
#include "unit_transpiler.h"


std::ostream &operator<<(std::ostream &os, UnitTranspiler unit) {
  if (unit.tmp == "")
    unit.GenerateResult();
  os << unit.tmp;
  return os;
}


void UnitTranspiler::GenerateResult() {
  std::stringstream body;
  body << "[args...] > global\n" ;
  if(!glob.Empty())
  {
    body << "  ram "<<glob.MemorySize()<<" > " << glob.name << "\n";
    used_external_objects.insert("c2eo.system.ram");
  }

  if(!stat.Empty())
  {
    body << "  ram "<<stat.MemorySize()<<" > s-ram\n";
    used_external_objects.insert("c2eo.system.ram");
  }

  if(!glob.Empty()){
    for(const auto& var : glob){
      body << "  address g-ram " << var.position << " > " << var.alias<<"\n";
    }
    used_external_objects.insert("c2eo.system.address");
  }

  if(!stat.Empty()){
    for(const auto& var : stat){
      body << "  address s-ram " << var.position << " > " << var.alias<<"\n";
    }
    used_external_objects.insert("c2eo.system.address");
  }

  body << "\n";

  if(!glob.Empty()||!stat.Empty()){
    body << "  seq > @\n";
    for(const auto& var : glob){
      body << "    " << var.alias << ".set " << var.value <<"\n";
    }
    for(const auto& var : stat){
      body << "    " << var.alias << ".set " << var.value <<"\n";
    }
    body << "    TRUE\n";
  }

  std::stringstream header;
  header << "+package c2eo.src.temporary_name\n\n";
  for(const auto& alias : used_external_objects)
  {
    header << "+alias "<<alias<<"\n";
  }

  header << "\n";

  tmp = header.str() + body.str();
}
