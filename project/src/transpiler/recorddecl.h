#ifndef C2EO_SRC_TRANSPILER_RECORDDECL_H_
#define C2EO_SRC_TRANSPILER_RECORDDECL_H_

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "record_manager.h"

std::vector<RecordType> ProcessRecordType(const clang::RecordDecl *RD, bool is_local = false);

#endif // C2EO_SRC_TRANSPILER_RECORDDECL_H_
