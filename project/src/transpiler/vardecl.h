#ifndef __VARDECL__
#define __VARDECL__

// Заголовочный файл с описанием параметров переменных

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "memory_manager.h"

// Обработка переменной встреченной при проходе по дереву.
Variable ProcessVariable(const clang::VarDecl *VD, std::string local_name = "", size_t shift = 0);

#endif // __VARDECL__
