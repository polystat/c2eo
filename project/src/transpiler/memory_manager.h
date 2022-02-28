#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__
#include <string>
#include <utility>
#include <vector>
#include "eo_object.h"

// Representation of a simple variable stored in RAM
struct Variable{
  uint64_t id;
  int position;
  size_t size;
  std::string type;
  std::string alias;
  std::string value;
  EOObject GetInitializer() const;
  EOObject GetAdress(std::string mem_name) const;
};

class MemoryManager{
 public:
  explicit MemoryManager(std::string name):pointer(0),name(std::move(name)) {}

  void Add(uint64_t id, size_t size, std::string type, std::string alias, std::string value);

  bool Empty();

  size_t RealMemorySize();

  const Variable& GetVarByID(uint64_t id) const;

  std::vector<Variable>::const_iterator begin() const;

  std::vector<Variable>::const_iterator end() const;

  std::string name;
  EOObject GetEOObject() const;
 private:
  // index of first free byte in memory
  int pointer;
  int mem_size = 2048;
  std::vector<Variable> variables;

};


#endif //__MEMORY_MANAGER__
