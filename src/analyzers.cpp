#include "analyzers.h"

void LoopAnalyzer::run(const MatchFinder::MatchResult &Result) {
  ASTContext *Context = Result.Context;
  const ForStmt *FS = Result.Nodes.getNodeAs<ForStmt>("forLoop");
  // We do not want to convert header files!
  if (!FS || !Context->getSourceManager().isWrittenInMainFile(FS->getForLoc()))
    return;
  const VarDecl *IncVar = Result.Nodes.getNodeAs<VarDecl>("incVarName");
  const VarDecl *CondVar = Result.Nodes.getNodeAs<VarDecl>("condVarName");
  const VarDecl *InitVar = Result.Nodes.getNodeAs<VarDecl>("initVarName");

  if (!areSameVariable(IncVar, CondVar) || !areSameVariable(IncVar, InitVar))
    return;
  llvm::outs() << "Potential array-based loop discovered.\n";
}

void IntVarDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  ASTContext *Context = Result.Context;
  const VarDecl *FS = Result.Nodes.getNodeAs<VarDecl>("intVarDecl");
  // We do not want to convert header files!
  ////if (!FS || !Context->getSourceManager().isWrittenInMainFile(FS->getForLoc()))
  if (!FS)
    return;
  llvm::outs() << "Declaration of Integer variable.\n";
}
