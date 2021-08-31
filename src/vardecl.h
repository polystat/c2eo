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

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;


using namespace clang;

// Определение и тестовый вывод основных параметров описания переменных
void getVarDeclParameters(const VarDecl *VD);

// Анализ полученного начального значения с последующим использованием
void initValueAnalysis(const VarDecl *VD);

#endif // __VARDECL__
