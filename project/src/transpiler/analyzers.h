#ifndef C2EO_SRC_TRANSPILER_ANALYZERS_H_
#define C2EO_SRC_TRANSPILER_ANALYZERS_H_

#include "vardecl.h"
#include "recorddecl.h"

using namespace clang;
using namespace clang::ast_matchers;

//------------------------------------------------------------------------------
class FuncDeclAnalyzer : public MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(const ValueDecl *first, const ValueDecl *second) {
    return first && second && first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(const MatchFinder::MatchResult &result) override;
};

//------------------------------------------------------------------------------
class RecordDeclAnalyzer : public MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(const ValueDecl *first, const ValueDecl *second) {
    return first && second && first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(const MatchFinder::MatchResult &result) override;
};

//------------------------------------------------------------------------------
class DeclBaseVarGlobalMemoryAnalyzer : public MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(const ValueDecl *first, const ValueDecl *second) {
    return first && second && first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(const MatchFinder::MatchResult &result) override;
};

#endif // C2EO_SRC_TRANSPILER_ANALYZERS_H_

