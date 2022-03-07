#ifndef __FUNCTION_MANAGER__
#define __FUNCTION_MANAGER__

#include <cstdint>
#include <string>
#include <utility>
#include "clang/AST/Decl.h"
#include "eo_object.h"

// Representation of a simple variable stored in RAM
struct FunctionDefinition {
  const clang::FunctionDecl *FD;
  std::string name;
  EOObject GetEOObject() const;
 private:
  EOObject GetBody() const;
};

struct FunctionDeclaration{
  const clang::FunctionDecl *FD;
  std::string name;
};


struct FunctionManager {

  void Add(const clang::FunctionDecl* FD);
  const std::vector<FunctionDefinition>& GetAllDefinitions();
 private:
  std::vector<FunctionDefinition> definitions;
  std::vector<FunctionDeclaration> declarations;
};

#endif // __FUNCTION_MANAGER__
