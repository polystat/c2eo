#ifndef __FUNCDECL__
#define __FUNCDECL__

// Заголовочный файл с описанием параметров функции,
// формируемых при анализе класса FunctionDecl

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

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод параметров описания функции
void getFuncDeclParameters(const FunctionDecl *FD);

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод основных формальных параметров
void getParmVarDeclParameters(const ParmVarDecl *PVD);

#endif // __FUNCDECL__
