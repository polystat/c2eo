#ifndef C2EO_SRC_TRANSPILER_FUNCTION_MANAGER_H_
#define C2EO_SRC_TRANSPILER_FUNCTION_MANAGER_H_

#include <cstdint>
#include <string>
#include <utility>
#include "clang/AST/Decl.h"
#include "eo_object.h"
#include "memory_manager.h"

// Representation of a simple function
struct FunctionDefinition {
  const clang::FunctionDecl *FD;
  std::string name;

  EOObject GetEoObject() const;

  void TestOut() const;

private:
  EOObject GetBody() const;
};

struct FunctionDeclaration {
  const clang::FunctionDecl *FD;
  std::string name;

  void TestOut() const;
};

struct FunctionManager {
  void AddDefinition(const FunctionDefinition &func_def);
  void AddDeclaration(const FunctionDeclaration &func_decl);
  void AddEoObject(const EOObject &func);

  const std::vector<EOObject> &GetAllEoDefinitions();

  EOObject GetFunctionCall(const clang::FunctionDecl *FD, size_t param_size) const;

  __attribute__((unused)) void TestOut();

 private:
  std::vector<FunctionDefinition> definitions;
  std::vector<FunctionDeclaration> declarations;
  std::vector<EOObject> functions;
};

#endif // C2EO_SRC_TRANSPILER_FUNCTION_MANAGER_H_
