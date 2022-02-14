#include "memory_manager.h"


void MemoryManager::Add(uint64_t id,size_t size, std::string type, std::string alias, std::string value) {
  Variable var = {id, pointer,size,std::move(type),std::move(alias), std::move(value)};
  variables.push_back(var);
  pointer += size;
}
bool MemoryManager::Empty() {
  return variables.empty();
}
size_t MemoryManager::MemorySize() {
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

