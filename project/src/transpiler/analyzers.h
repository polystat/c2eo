#ifndef C2EO_SRC_TRANSPILER_ANALYZERS_H_
#define C2EO_SRC_TRANSPILER_ANALYZERS_H_

#include "enumdecl.h"
#include "recorddecl.h"
#include "vardecl.h"

//------------------------------------------------------------------------------
class FuncDeclAnalyzer
    : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(
      const clang::ValueDecl *first, const clang::ValueDecl *second) {
    return (first != nullptr) && (second != nullptr) &&
           first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(
      const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};

//------------------------------------------------------------------------------
class RecordDeclAnalyzer
    : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(
      const clang::ValueDecl *first, const clang::ValueDecl *second) {
    return (first != nullptr) && (second != nullptr) &&
           first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(
      const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};

//------------------------------------------------------------------------------
class DeclBaseVarGlobalMemoryAnalyzer
    : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(
      const clang::ValueDecl *first, const clang::ValueDecl *second) {
    return (first != nullptr) && (second != nullptr) &&
           first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(
      const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};

class EnumDeclAnalyzer
    : public clang::ast_matchers::MatchFinder::MatchCallback {
 public:
  __attribute__((unused)) static bool AreSameVariable(
      const clang::ValueDecl *first, const clang::ValueDecl *second) {
    return (first != nullptr) && (second != nullptr) &&
           first->getCanonicalDecl() == second->getCanonicalDecl();
  }

  void run(
      const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};

#endif  // C2EO_SRC_TRANSPILER_ANALYZERS_H_
