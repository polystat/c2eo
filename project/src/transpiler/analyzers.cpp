#include "analyzers.h"
#include "unit_transpiler.h"
#include "tracer.h"
#include "transpile_helper.h"

extern UnitTranspiler transpiler;
ASTContext* context;

//------------------------------------------------------------------------------
// Анализ на функцию
void FuncDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context) {
    context = Result.Context;
  }
  const FunctionDecl *FD = Result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  // We do not want to convert header files!
  // TODO !FD->isDefined() now only plug and should be fixed later
  //   if (!FD || !FD->isDefined() || !context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
  if (!FD) {
    return;
  }

  #ifdef TRACEOUT_FUNC_DEF
  TraceOutFunctionDecl(FD);   // Тестовый вывод содержимого функции
  #endif

  // Формирование определения или объявления.
  // Перенесено в точку их анализа, так как формируются разные объекты
  DeclarationNameInfo declNameInfo{FD->getNameInfo()};
  std::string funcName{declNameInfo.getAsString()};
//   auto funcName = FD->getNameAsString();
  if (funcName != "main")
    funcName = "f-" + funcName;
  auto isDefinition = FD->isThisDeclarationADefinition();
  if (isDefinition) {
    // При наличии определения осуществляется формирование функции
    // Можно также сохранить ее здесь в списке имен, чтобы не дублировать ниже
    FunctionDefinition funcDef{FD, funcName};
    // Добавление в список определений функций
    transpiler.func_manager.AddDefinition(funcDef);

    EOObject eoFuncDef = funcDef.GetEOObject();
    transpiler.func_manager.AddEOObject(eoFuncDef);
//     std::cout << eoFuncDef;
    // Формирование действий, связанных с объявлением. Пока никак. Но нужно думать о добавлении алиасов
    // Наверное нужно сохранять только имена
//     definitions.push_back({FD, funcName});
  } else {
    // Прототипы тоже должны сохраняться, чтобы потом сформировать алиасы. И убрать неиспользуемые алиасы,
    // так как может быть много лишних из внешних заголовочных файлов
    // Добавление в список определений функций
    FunctionDeclaration funcDecl{FD, funcName};
    transpiler.func_manager.AddDeclaration(funcDecl);
  }

//   transpiler.func_manager.Add(FD);  // Этот код пока дублируется. В дальнейшем можно будет заменить.
  // ProcessFunction(FD);
  // getFuncDeclParameters(FD);

}

//------------------------------------------------------------------------------
// Анализ на структуру или объединение
void RecordDeclAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto *RD = Result.Nodes.getNodeAs<RecordDecl>("recordDecl");
  // We do not want to convert header files!
  // if (!RD || !Context->getSourceManager().isWrittenInMainFile(RD->getForLoc()))
  if (!RD)
    return;
  // getRecordDeclSubObjects(RD);
  // RD->dump();
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

  // getVarDeclParameters(VD);
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
  // if (!VD || !Context->getSourceManager().isWrittenInMainFile(VD->getForLoc()))
  if (!VD)
    return;
  llvm::outs() << "Integer variable.\n";
  // Определение и тестовый вывод основных параметров описания переменных
  ProcessVariable(VD);

  // VD->dump();
}

//------------------------------------------------------------------------------
// Анализ на глобальную целочисленную переменную
void IntVarDeclGlobalMemoryAnalyzer::run(const MatchFinder::MatchResult &Result) {
  if (!context)
    context = Result.Context;
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("intVarGlobalMemoryDecl");
  // We do not want to convert header files!
  // if (!VD || !Context->getSourceManager().isWrittenInMainFile(VD->getForLoc()))
  if (!VD)
    return;

  llvm::outs() << "I`m variable. My name is " << VD->getNameAsString() << "\n";

  if (VD->hasLocalStorage()) {
    llvm::outs() << "   hasLocalStorage.\n";
  } else {
    llvm::outs() << "   not hasLocalStorage.\n";
  }

  if (VD->isStaticLocal()) {
    llvm::outs() << "   isStaticLocal.\n";
  } else {
    llvm::outs() << "   not isStaticLocal.\n";
  }

  if (VD->hasExternalStorage()) {
    llvm::outs() << "   hasExternalStorage.\n";
  } else {
    llvm::outs() << "   not hasExternalStorage.\n";
  }

  if (VD->hasGlobalStorage()) {
    llvm::outs() << "   hasGlobalStorage.\n";
  } else {
    llvm::outs() << "   not hasGlobalStorage.\n";
  }

  if (VD->isLocalVarDecl()) {
    llvm::outs() << "   isLocalVarDecl.\n";
  } else {
    llvm::outs() << "   not isLocalVarDecl.\n";
  }

  if (VD->isLocalVarDeclOrParm()) {
    llvm::outs() << "   isLocalVarDeclOrParm.\n";
  } else {
    llvm::outs() << "   not isLocalVarDeclOrParm.\n";
  }

  if (VD->getAnyInitializer()) {
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

  // VD->dump();
}
