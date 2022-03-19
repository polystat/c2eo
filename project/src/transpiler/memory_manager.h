#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "eo_object.h"

// Representation of a simple variable stored in RAM
struct Variable {
  uint64_t id;
  size_t position;
  size_t size;
  std::string type;
  std::string alias;
  std::string value;
  std::string local_pointer;
  size_t shift;
  std::string type_postfix;
  bool is_initialized;
  bool operator==(const Variable& var) const;
  EOObject GetInitializer() const;
  EOObject GetAddress(std::string mem_name) const;
};

class MemoryManager{
 public:
  explicit MemoryManager(std::string name):pointer(0),name(std::move(name)) {}

  Variable Add(uint64_t id,
               size_t size,
               const std::string &type,
               std::string alias,
               std::string value,
               std::string local_name = "",
               size_t shift = 0,
               bool is_initialized = true);

  bool Empty();

  size_t RealMemorySize();

  const Variable& GetVarByID(uint64_t id) const;

  std::vector<Variable>::const_iterator begin() const;

  std::vector<Variable>::const_iterator end() const;

  std::string name;

  EOObject GetEOObject() const;
  void RemoveAllUsed(const std::vector<Variable>& all_local);
 private:
  // index of first free byte in memory
  size_t pointer;
  int mem_size = 2048;
  std::vector<Variable> variables;

};


#endif //__MEMORY_MANAGER__
