#include <iostream>
#include "function_manager.h"
#include "transpile_helper.h"

using namespace clang;
using namespace llvm;

//==============================================================================
EOObject FunctionDefinition::GetEoObject() const {
  EOObject func_object(EOObjectType::EO_ABSTRACT);
  func_object.postfix = name;
  func_object.arguments = {"param-start", "param-size"};
  func_object.nested.push_back(GetBody());
  return func_object;
}

EOObject FunctionDefinition::GetBody() const {
  return GetFunctionBody(FD);
}

void FunctionDefinition::TestOut() const {
  std::cout << name << "\n";
}

void FunctionDeclaration::TestOut() const {
  std::cout << name << "\n";
}

//------------------------------------------------------------------------------
void FunctionManager::AddDefinition(const FunctionDefinition &func_def) {
  definitions.push_back(func_def);
}

//------------------------------------------------------------------------------
void FunctionManager::AddDeclaration(const FunctionDeclaration &func_decl) {
  declarations.push_back(func_decl);
}
//------------------------------------------------------------------------------
void FunctionManager::AddEoObject(const EOObject &func) {
  functions.push_back(func);
}

//------------------------------------------------------------------------------
const std::vector<EOObject> &FunctionManager::GetAllEoDefinitions() {
  return functions;
}

//------------------------------------------------------------------------------
EOObject FunctionManager::GetFunctionCall(const clang::FunctionDecl *FD, size_t param_size) const {
  auto res_def = std::find_if(definitions.begin(), definitions.end(), [&FD](const FunctionDefinition &decl) {
    return decl.FD == FD;
  });
  if (res_def != definitions.end()) {
    EOObject call(res_def->name);
    call.prefix = "^.^.^";
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size), EOObjectType::EO_LITERAL);
    return call;
  }
  auto res_decl = std::find_if(declarations.begin(), declarations.end(), [&FD](const FunctionDeclaration &decl) {
    return decl.FD == FD;
  });
  if (res_decl != declarations.end()) {
    EOObject call(res_decl->name);
    if (res_decl->FD->getDefinition()) {
      // TODO may be should count level of object
      call.prefix = "^.^.^";
    }
    call.nested.emplace_back("empty-local-position");
    call.nested.emplace_back(std::to_string(param_size), EOObjectType::EO_LITERAL);
    return call;
  }
  return EOObject(EOObjectType::EO_PLUG);
}

__attribute__((unused)) void FunctionManager::TestOut() {
  if (!declarations.empty()) {
    std::cout << "Declarations:\n";
    for (const auto &declaration : declarations) {
      declaration.TestOut();
    }
  } else {
    std::cout << "No function declarations\n";
  }
  if (!definitions.empty()) {
    std::cout << "Definitions:\n";
    for (const auto &definition : definitions) {
      definition.TestOut();
    }
  } else {
    std::cout << "No function definitions\n";
  }
}

