/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
