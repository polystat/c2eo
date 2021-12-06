#include "generator.h"

#ifndef C2EO_DECL_H
#define C2EO_DECL_H

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "generator.h"
#include "stmt.h"

using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

AbstractGen* getDeclGen(Decl* decloration);
VarGen *getVarDeclGen(VarDecl *VD);



#endif //C2EO_DECL_H
