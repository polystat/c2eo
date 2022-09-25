/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

  clang::DeclarationNameInfo decl_name_info{FD->getNameInfo()};
  std::string func_name{decl_name_info.getAsString()};
  if (func_name != "main") {
    func_name = "f-" + func_name;
  }
  // Add the function name to func_name_map if it is new name
  transpiler.func_manager_.AddToMap(&func_name);

  auto is_definition = FD->isThisDeclarationADefinition();
  if (is_definition) {
    FunctionDefinition func_def{FD, func_name};
    transpiler.func_manager_.AddDefinition(func_def);

    EOObject eo_func_def = func_def.GetEoObject();
    transpiler.func_manager_.AddEoObject(eo_func_def);
  } else {
    FunctionDeclaration func_decl{FD, func_name};
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
