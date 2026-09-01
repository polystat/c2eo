/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#include "src/transpiler/matchers.h"

using clang::ast_matchers::DeclarationMatcher;
using clang::ast_matchers::enumDecl;
using clang::ast_matchers::functionDecl;
using clang::ast_matchers::MatchFinder;
using clang::ast_matchers::recordDecl;
using clang::ast_matchers::varDecl;

void AddFuncDeclMatcher(MatchFinder &finder) {
  static FuncDeclAnalyzer func_decl_analyzer;
  static const DeclarationMatcher func_decl_matcher =
      functionDecl().bind("funcDecl");

  finder.addMatcher(func_decl_matcher, &func_decl_analyzer);
}

void AddRecordDeclMatcher(MatchFinder &finder) {
  static RecordDeclAnalyzer record_decl_analyzer;
  static const DeclarationMatcher record_decl_matcher =
      recordDecl().bind("recordDecl");

  finder.addMatcher(record_decl_matcher, &record_decl_analyzer);
}

void AddDeclBaseVarGlobalMemoryMatcher(MatchFinder &finder) {
  static DeclBaseVarGlobalMemoryAnalyzer decl_base_var_global_memory_analyzer;
  static const DeclarationMatcher decl_base_var_global_memory_matcher =
      varDecl().bind("declBaseVarGlobalMemory");

  finder.addMatcher(decl_base_var_global_memory_matcher,
                    &decl_base_var_global_memory_analyzer);
}

void AddEnumDeclMatcher(MatchFinder &finder) {
  static EnumDeclAnalyzer enum_decl_analyzer;
  static const DeclarationMatcher enum_decl_matcher =
      enumDecl().bind("enumDecl");

  finder.addMatcher(enum_decl_matcher, &enum_decl_analyzer);
}

//==============================================================================
void AddMatchers(MatchFinder &finder) {
  AddFuncDeclMatcher(finder);
  AddDeclBaseVarGlobalMemoryMatcher(finder);
  AddRecordDeclMatcher(finder);
  AddEnumDeclMatcher(finder);
}
