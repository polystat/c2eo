#ifndef C2EO_SRC_TRANSPILER_VARDECL_H_
#define C2EO_SRC_TRANSPILER_VARDECL_H_

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "memory_manager.h"

Variable ProcessVariable(const clang::VarDecl *VD, const std::string &local_name = "", size_t shift = 0);

#endif // C2EO_SRC_TRANSPILER_VARDECL_H_
