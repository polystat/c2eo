#include <algorithm>
#include <exception>
#include <stdexcept>
#include "memory_manager.h"

using namespace std;


void MemoryManager::Add(uint64_t id,size_t size, std::string type, std::string alias, std::string value) {
  Variable var = {id, pointer,size,std::move(type),std::move(alias), std::move(value)};
  variables.push_back(var);
  pointer += size;
}
bool MemoryManager::Empty() {
  return variables.empty();
}
size_t MemoryManager::RealMemorySize() {
  size_t result = 0;
  for (const auto& v : variables) {
    result += v.size;
  }
  return result;
}
std::vector<Variable>::const_iterator MemoryManager::begin() const{
  return variables.begin();
}
std::vector<Variable>::const_iterator MemoryManager::end() const{
  return variables.end();
}
const Variable &MemoryManager::GetVarByID(uint64_t id) const{
  auto res = find_if(variables.begin(),variables.end(),
                     [id](Variable x) { return x.id == id;});
  if (res == variables.end())
    throw invalid_argument("element with id "+ to_string(id) + " not found");
  return  *res;
}

EOObject MemoryManager::GetEOObject() const {
  EOObject res{"ram"};
  res.postfix = name;
  res.nested.emplace_back(to_string(mem_size), EOObjectType::EO_LITERAL);
  return res;
}

EOObject Variable::GetInitializer() const{
  EOObject res("write");
  res.nested.emplace_back(alias);
  res.nested.emplace_back(value,EOObjectType::EO_LITERAL);
  return res;
}
EOObject Variable::GetAdress(string mem_name) const{
  EOObject addr("address",alias);
  if(!mem_name.empty())
    addr.nested.emplace_back(std::move(mem_name));
  addr.nested.emplace_back(to_string(position));
  return addr;
}
