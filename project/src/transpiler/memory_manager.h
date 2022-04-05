#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "eo_object.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"


// Representation of a simple variable stored in RAM
struct Variable {
  const clang::VarDecl *id;
  size_t position;
  size_t size;
  // like c-int64
  std::string type;
  // like g-x
  std::string alias;
  // TODO value should be EOObject
  std::string value;
  // address of local memory start in global
  std::string local_pointer;
  size_t shift;
  // only -int64
  std::string type_postfix;
  bool is_initialized;

  bool operator==(const Variable &var) const;

  EOObject GetInitializer() const;

  EOObject GetAddress(std::string mem_name) const;
};

class MemoryManager {
public:
  explicit MemoryManager(std::string name) : pointer(0), name(std::move(name)) {}

  Variable Add(const clang::VarDecl *id,
               size_t size,
               const std::string &type,
               std::string alias,
               std::string value,
               std::string local_name = "",
               size_t shift = 0,
               bool is_initialized = true);

  bool Empty();

  size_t RealMemorySize();

  const Variable &GetVarByID(const clang::VarDecl *id) const;

  std::vector<Variable>::const_iterator begin() const;

  std::vector<Variable>::const_iterator end() const;

  std::string name;

  EOObject GetEOObject() const;

  void RemoveAllUsed(const std::vector<Variable> &all_local);

private:
  // index of first free byte in memory
  size_t pointer;
  int mem_size = 2048;
  std::vector<Variable> variables;

};


#endif // __MEMORY_MANAGER__
