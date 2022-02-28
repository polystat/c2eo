//
// Created by nchuykin on 28.02.2022.
//

#include "function_manager.h"


void FunctionManager::Add(const clang::FunctionDecl *FD) {
  auto funcName = FD->getNameAsString();
  if(funcName != "main")
    funcName = "f-" + funcName;
  auto isDefinition = FD->isThisDeclarationADefinition();
  if(std::find_if(definitions.begin(), definitions.end(),
                   [&funcName](FunctionDefinition f){ return f.name == funcName; }) != definitions.end())
  {
    return;
  }
  if (isDefinition)
  {
    auto declaration = std::remove_if(declarations.begin(), declarations.end(),
                                      [&funcName](FunctionDeclaration f){ return f.name == funcName; });
    declarations.erase(declaration,declarations.end());
    definitions.push_back({FD, funcName});
  }
  else
  {
    declarations.push_back({FD, funcName});
  }

}
std::vector<FunctionDefinition>::const_iterator FunctionManager::begin() const {
  return definitions.begin();
}

std::vector<FunctionDefinition>::const_iterator FunctionManager::end() const {
  return definitions.end();
}

 EOObject FunctionDefinition::GetEOObject() const {
   EOObject func_object(EOObjectType::EO_ABSTRACT);
   func_object.postfix = name;
   return func_object;
}


