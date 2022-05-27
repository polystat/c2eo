#ifndef C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_
#define C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_

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
  const clang::VarDecl* id;
  size_t position;
  size_t size;
  // like c-int64
  std::string type;
//  bool isCustomType = false;
  // like g-x
  std::string alias;
  // TODO value should be EOObject
  EOObject value;
  // address of local memory start in global
  std::string local_pointer;
  size_t shift;
  // only -int64
  std::string type_postfix;
  bool is_initialized;

  bool operator==(const Variable& var) const;

  EOObject GetInitializer() const;

  EOObject GetAddress(const std::string& mem_name) const;
};

class MemoryManager {
public:
  explicit MemoryManager(std::string name, size_t start_pointer = 0) :
      pointer_(start_pointer), name_(std::move(name)) {}

  Variable Add(const clang::VarDecl* id,
               size_t size,
               const std::string &type,
               std::string alias,
               EOObject value,
               std::string local_name = "",
               size_t shift = 0,
               bool is_initialized = true);

  Variable AddExternal(const clang::VarDecl *id,
                       size_t size,
                       const std::string &type,
                       std::string alias,
                       EOObject value,
                       std::string local_name = "",
                       size_t shift = 0,
                       __attribute__((unused)) bool is_initialized = false);

  bool Empty();

  size_t RealMemorySize();

  const Variable &GetVarById(const clang::VarDecl *id) const;

  std::vector<Variable>::const_iterator begin() const;

  std::vector<Variable>::const_iterator end() const;

  std::string name_;

  EOObject GetEOObject() const;

  void RemoveAllUsed(const std::vector<Variable> &all_local);

  void SetExtEqGlob();

 private:
  // index of first free byte in memory
  size_t pointer_;
  int mem_size_ = 2048;
  std::vector<Variable> variables_;

};

#endif // C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_
