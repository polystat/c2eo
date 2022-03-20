//
// Created by nchuykin on 28.02.2022.
//

#include "function_manager.h"
#include "transpile_helper.h"


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
const std::vector<FunctionDefinition> &FunctionManager::GetAllDefinitions() {
  return definitions;
}
EOObject FunctionManager::GetFunctionCall(const clang::FunctionDecl* FD) const{
  auto res_def = std::find_if(definitions.begin(), definitions.end(), [&FD] (const FunctionDefinition& decl){
    return decl.FD == FD;
  });
  if (res_def!=definitions.end())
  {
    EOObject call (res_def->name);
    call.nested.emplace_back("local-empty-position");
    call.nested.emplace_back("0",EOObjectType::EO_LITERAL);
    return call;
  }
  auto res_decl = std::find_if(declarations.begin(), declarations.end(), [&FD] (const FunctionDeclaration& decl){
    return decl.FD == FD;
  });
  if (res_decl!=declarations.end())
  {
    EOObject call (res_decl->name);
    call.nested.emplace_back("local-empty-position");
    call.nested.emplace_back("0",EOObjectType::EO_LITERAL);
    return call;
  }
  return EOObject(EOObjectType::EO_PLUG);
}

EOObject FunctionDefinition::GetEOObject() const {
   EOObject func_object(EOObjectType::EO_ABSTRACT);
   func_object.postfix = name;
   func_object.arguments = {"param-start","param-size"};
   func_object.nested.push_back(GetBody());
   return func_object;
}
EOObject FunctionDefinition::GetBody() const{
  return GetFunctionBody(FD);
}


