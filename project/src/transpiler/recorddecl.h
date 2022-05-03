#ifndef __RECORDDECL__
#define __RECORDDECL__

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "record_manager.h"

// Обработка переменной встреченной при проходе по дереву.
std::vector<RecordType> ProcessRecordType(const clang::RecordDecl* RD, bool is_local=false);

#endif // __RECORDDECL__
