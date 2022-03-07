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

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;


using namespace clang;

// Обработка переменной встреченной при проходе по дереву.
void ProcessVariable(const VarDecl *VD);

// Определение и тестовый вывод основных параметров описания переменных
// void getVarDeclParameters(const VarDecl *VD);
//-------------------------------------------------------------------------------------------------



//void getTypeName(const ValueDecl* VD, std::string &str);
void getListValue(const Stmt* stmt, std::string &str, ASTContext* context);

#endif // __VARDECL__
