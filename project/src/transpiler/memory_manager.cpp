#include "memory_manager.h"

MemoryManager global_memory("g-ram");//, static_memory("s_ram");

void MemoryManager::Add(int size, std::string type, std::string alias) {
  Variable var = {pointer,size,std::move(type),std::move(alias)};
  pointer += size;
}
