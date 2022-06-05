#ifndef C2EO_SRC_TRANSPILER_UNIT_TRANSPILER_H_
#define C2EO_SRC_TRANSPILER_UNIT_TRANSPILER_H_

#include "string"
#include "set"
#include "iostream"
#include "memory_manager.h"
#include "function_manager.h"
#include "record_manager.h"

class UnitTranspiler {
 public:
  UnitTranspiler() = default;

//   MemoryManager glob_{"global-ram", 8}, ret_{"return-ram"};
  MemoryManager glob_{"global-ram"}, ret_{"return-ram"};
  FunctionManager func_manager_;
  RecordManager record_manager_;
  friend std::ostream &operator<<(std::ostream &os, UnitTranspiler unit);

  std::string name_;
  std::set<std::string> used_external_objects_;

  void SetPackageName(std::string package_name);
  static void SetPathName(std::string path_name);
  // std::vector<EOObject>

 private:
  std::string package_name_;
  std::string path_name_;
  std::string tmp_;
//  std::vector<EOObject> objects_;

  void GenerateResult();

};

#endif // C2EO_SRC_TRANSPILER_UNIT_TRANSPILER_H_
