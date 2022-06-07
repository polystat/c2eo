#include "matchers.h"

using namespace clang;
using namespace clang::ast_matchers;

void AddFuncDeclMatcher(MatchFinder &finder) {
  static FuncDeclAnalyzer func_decl_analyzer;
  static DeclarationMatcher func_decl_matcher = functionDecl().bind("funcDecl");

  finder.addMatcher(func_decl_matcher, &func_decl_analyzer);
}

void AddRecordDeclMatcher(MatchFinder &finder) {
  static RecordDeclAnalyzer record_decl_analyzer;
  static DeclarationMatcher record_decl_matcher =
      recordDecl().bind("recordDecl");

  finder.addMatcher(record_decl_matcher, &record_decl_analyzer);
}

void AddDeclBaseVarGlobalMemoryMatcher(MatchFinder &finder) {
  static DeclBaseVarGlobalMemoryAnalyzer decl_base_var_global_memory_analyzer;
  static DeclarationMatcher decl_base_var_global_memory_matcher =
      varDecl().bind("declBaseVarGlobalMemory");

  finder.addMatcher(decl_base_var_global_memory_matcher,
                    &decl_base_var_global_memory_analyzer);
}

//==============================================================================
void AddMatchers(MatchFinder &finder) {
  AddFuncDeclMatcher(finder);
  AddDeclBaseVarGlobalMemoryMatcher(finder);
  AddRecordDeclMatcher(finder);
}
