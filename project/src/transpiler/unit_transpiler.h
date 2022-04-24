#ifndef __UNIT_TRANSPILER__
#define __UNIT_TRANSPILER__

#include "string"
#include "set"
#include "iostream"
#include "memory_manager.h"
#include "function_manager.h"
#include "record_manager.h"

class UnitTranspiler {
public:
  UnitTranspiler() = default;

//   MemoryManager glob{"global-ram", 8}, ret{"return-ram"};
  MemoryManager glob{"global-ram"}, ret{"return-ram"};
  FunctionManager func_manager;
  RecordManager record_manager;
  friend std::ostream& operator <<(std::ostream& os, UnitTranspiler unit);

  std::string name;
  // Коллекция для составления списка алиасов
  std::set<std::string> used_external_objects;

  void SetPackageName(std::string packagename);
  void SetPathName(std::string pathName);
  // std::vector<EOObject>

private:
  std::string package_name;
  std::string path_name;
  std::string tmp;
  std::vector<EOObject> objects;

  void GenerateResult();


};


#endif // __UNIT_TRANSPILER__
