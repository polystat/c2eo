#include "analyzers.h"
#include "unit_transpiler.h"

extern UnitTranspiler transpiler;
ASTContext* context;
//------------------------------------------------------------------------------
// Анализ на функцию
void FuncDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  // We do not want to convert header files!
  // TODO !FD->isDefined() now only plug and should be fixed later
  if (!FD || !FD->isDefined() || !context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
      return;

  transpiler.func_manager.Add(FD);
  //ProcessFunction(FD);
    //getFuncDeclParameters(FD);

}

//------------------------------------------------------------------------------
// Анализ на структуру или объединение
void RecordDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto *RD = Result.Nodes.getNodeAs<RecordDecl>("recordDecl");
  // We do not want to convert header files!
  ////if (!RD || !Context->getSourceManager().isWrittenInMainFile(RD->getForLoc()))
  if (!RD)
    return;
  //TODO Return record decl analysis
  //getRecordDeclSubObjects(RD);
  //RD->dump();
  ProcessRecordType(RD);
}

//------------------------------------------------------------------------------
// Анализ на глобальную переменную
void DeclBaseVarGlobalMemoryAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("declBaseVarGlobalMemory");
  // We do not want to convert header files!
  if (!VD || !context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
      return;

  //getVarDeclParameters(VD);
  ProcessVariable(VD);
}

//------------------------------------------------------------------------------
// Анализатор цикла с заданными инициализатором, условием... Заимствован из примера
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

//------------------------------------------------------------------------------
// Анализ на целочисленную переменную
void IntVarDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto VD = Result.Nodes.getNodeAs<VarDecl>("intVarDecl");
    // We do not want to convert header files!
    ////if (!VD || !Context->getSourceManager().isWrittenInMainFile(VD->getForLoc()))
    if (!VD)
        return;
    llvm::outs() << "Integer variable.\n";
    // Определение и тестовый вывод основных параметров описания переменных
    ProcessVariable(VD);

    //VD->dump();
}

//------------------------------------------------------------------------------
// Анализ на глобальную целочисленную переменную
void IntVarDeclGlobalMemoryAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("intVarGlobalMemoryDecl");
  // We do not want to convert header files!
  ////if (!VD || !Context->getSourceManager().isWrittenInMainFile(VD->getForLoc()))
  if (!VD)
    return;

  llvm::outs() << "I`m variable. My name is " << VD->getNameAsString() << "\n";

  if(VD->hasLocalStorage()) {
    llvm::outs() << "   hasLocalStorage.\n";
  } else {
    llvm::outs() << "   not hasLocalStorage.\n";
  }

  if(VD->isStaticLocal()) {
    llvm::outs() << "   isStaticLocal.\n";
  } else {
    llvm::outs() << "   not isStaticLocal.\n";
  }

  if(VD->hasExternalStorage()) {
    llvm::outs() << "   hasExternalStorage.\n";
  } else {
    llvm::outs() << "   not hasExternalStorage.\n";
  }

  if(VD->hasGlobalStorage()) {
    llvm::outs() << "   hasGlobalStorage.\n";
  } else {
    llvm::outs() << "   not hasGlobalStorage.\n";
  }

  if(VD->isLocalVarDecl()) {
    llvm::outs() << "   isLocalVarDecl.\n";
  } else {
    llvm::outs() << "   not isLocalVarDecl.\n";
  }

  if(VD->isLocalVarDeclOrParm()) {
    llvm::outs() << "   isLocalVarDeclOrParm.\n";
  } else {
    llvm::outs() << "   not isLocalVarDeclOrParm.\n";
  }

  if(VD->getAnyInitializer()) {
    llvm::outs() << "   getAnyInitializer.\n";
  } else {
    llvm::outs() << "   not getAnyInitializer.\n";
  }

//   APValue *initVal = VD->evaluateValue();
//   if(initVal != nullptr) {
//     llvm::outs() << "   evaluateValue = ";
//     if(initVal->isInt()) {
//         llvm::outs() << initVal->getInt();
//     }
//     llvm::outs() << ".\n";
//   } else {
//     llvm::outs() << "   not evaluateValue.\n";
//   }

  //VD->dump();
}
