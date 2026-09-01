/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_ANALYZERS_H_
#define SRC_TRANSPILER_ANALYZERS_H_

#include "src/transpiler/enumdecl.h"
#include "src/transpiler/recorddecl.h"
#include "src/transpiler/vardecl.h"

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

#endif  // SRC_TRANSPILER_ANALYZERS_H_
