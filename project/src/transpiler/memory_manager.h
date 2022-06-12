#ifndef C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_
#define C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_

static const int some_non_zero_position = 999999;
static const int two_kilobytes = 2048;
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "eo_object.h"
#include "llvm/Support/CommandLine.h"

// Representation of a simple variable stored in RAM
struct Variable {
  const clang::VarDecl *id;
  size_t position;
  size_t size;
  // like c-int64
  std::string type;
  // bool isCustomType = false;
  // like g-x
  std::string alias;
  EOObject value;
  // address of local memory start in global
  std::string local_pointer;
  size_t shift;
  // only -int64
  std::string type_postfix;
  bool is_initialized;

  bool operator==(const Variable &var) const;

  [[nodiscard]] std::vector<EOObject> GetInitializer() const;

  [[nodiscard]] EOObject GetAddress(const std::string &mem_name) const;

  std::vector<EOObject> GetListInitializer(EOObject rootAlias, EOObject listValue) const;
};

class MemoryManager {
 public:
  [[maybe_unused]] explicit MemoryManager(std::string name,
                                          size_t start_pointer = 0)
      : pointer_(start_pointer), name_(std::move(name)) {}

  Variable Add(const clang::VarDecl *id, size_t size, const std::string &type,
               std::string alias, EOObject value, std::string local_name = "",
               size_t shift = 0, bool is_initialized = true);

  Variable AddExternal(const clang::VarDecl *id, size_t size,
                       const std::string &type, std::string alias,
                       EOObject value, std::string local_name = "",
                       size_t shift = 0,
                       __attribute__((unused)) bool is_initialized = false);

  bool Empty();

  size_t RealMemorySize();

  const Variable &GetVarById(const clang::VarDecl *id) const;

  [[nodiscard]] std::vector<Variable>::const_iterator begin() const;

  [[nodiscard]] std::vector<Variable>::const_iterator end() const;

  std::string name_;

  [[nodiscard]] EOObject GetEOObject() const;

  void RemoveAllUsed(const std::vector<Variable> &all_local);

  void SetExtEqGlob();

 private:
  // index of first free byte in memory
  size_t pointer_;
  int mem_size_ = two_kilobytes;
  std::vector<Variable> variables_;
  std::map<std::string, int> duplicates;
};

#endif  // C2EO_SRC_TRANSPILER_MEMORY_MANAGER_H_
