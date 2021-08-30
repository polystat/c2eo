#ifndef __ANALYZERS__
#define __ANALYZERS__

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

class IntVarDeclAnalyzer : public MatchFinder::MatchCallback {
public: 
    static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
        return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();
    }
    virtual void run(const MatchFinder::MatchResult &Result);
};

class LoopAnalyzer : public MatchFinder::MatchCallback {
public: 
    static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
        return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();
    }
    virtual void run(const MatchFinder::MatchResult &Result);
};

#endif // __ANALYZERS__

