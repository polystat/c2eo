/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/analyzers.h"

#include <string>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"

using clang::ast_matchers::MatchFinder;

extern UnitTranspiler transpiler;
clang::ASTContext *context;

//------------------------------------------------------------------------------
// Function analysis
void FuncDeclAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (context == nullptr) {
    context = result.Context;
  }
  const auto *FD = result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl");
  // We do not want to convert header files!
  //   if (!FD || !FD->isDefined() ||
  //   !context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
  if (FD == nullptr) {
    return;
  }

#ifdef TRACEOUT_FUNC_DEF
  TraceOutFunctionDecl(FD);  // Test output of the function content
#endif

  const clang::DeclarationNameInfo decl_name_info{FD->getNameInfo()};
  std::string func_name{decl_name_info.getAsString()};
  if (func_name != "main") {
    func_name = "f-" + func_name;
  }
  // Add the function name to func_name_map if it is new name
  transpiler.func_manager_.AddToMap(&func_name);

  auto is_definition = FD->isThisDeclarationADefinition();
  if (is_definition) {
    const FunctionDefinition func_def{FD, func_name};
    transpiler.func_manager_.AddDefinition(func_def);

    const EOObject eo_func_def = func_def.GetEoObject();
    transpiler.func_manager_.AddEoObject(eo_func_def);
  } else {
    const FunctionDeclaration func_decl{FD, func_name};
    transpiler.func_manager_.AddDeclaration(func_decl);
  }
}

void RecordDeclAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (context == nullptr) {
    context = result.Context;
  }
  const auto *RD = result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl");
  if (RD == nullptr) {
    return;
  }
  ProcessRecordType(RD);
}

void DeclBaseVarGlobalMemoryAnalyzer::run(
    const MatchFinder::MatchResult &result) {
  if (context == nullptr) {
    context = result.Context;
  }
  const auto *VD =
      result.Nodes.getNodeAs<clang::VarDecl>("declBaseVarGlobalMemory");
  // We do not want to convert header files!
  if ((VD == nullptr) ||
      !context->getSourceManager().isWrittenInMainFile(VD->getLocation())) {
    return;
  }
  ProcessVariable(VD);
}

void EnumDeclAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (context == nullptr) {
    context = result.Context;
  }
  const auto *ED = result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl");
  if (ED == nullptr) {
    return;
  }
  ProcessEnumDecl(ED);
}
