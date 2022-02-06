#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__
#include <string>
#include <utility>
#include <vector>

// Representation of a simple variable stored in RAM
struct Variable{
  int position;
  int size;
  std::string type;
  std::string alias;
  std::string value;
};

class MemoryManager{
 public:
  explicit MemoryManager(std::string name):pointer(0),name(std::move(name)) {}

  void Add(int size, std::string type, std::string alias);

 private:
  // index of first free byte in memory
  int pointer;
  std::string name;
  std::vector<Variable> variables;

};


#endif //__MEMORY_MANAGER__
