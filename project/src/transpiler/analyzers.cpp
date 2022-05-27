#include "analyzers.h"
#include "unit_transpiler.h"
#include "tracer.h"
#include "transpile_helper.h"

extern UnitTranspiler transpiler;
ASTContext* context;

//------------------------------------------------------------------------------
// Function analysis
void FuncDeclAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (!context) {
    context = result.Context;
  }
  const auto *FD = result.Nodes.getNodeAs<FunctionDecl>("funcDecl");
  // We do not want to convert header files!
  // TODO !FD->isDefined() now only plug and should be fixed later
  //   if (!FD || !FD->isDefined() || !context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
  if (!FD) {
    return;
  }

#ifdef TRACEOUT_FUNC_DEF
  TraceOutFunctionDecl(FD);   // Тестовый вывод содержимого функции
#endif

  DeclarationNameInfo decl_name_info{FD->getNameInfo()};
  std::string func_name{decl_name_info.getAsString()};
  if (func_name != "main")
    func_name = "f-" + func_name;
  auto is_definition = FD->isThisDeclarationADefinition();
  if (is_definition) {
    FunctionDefinition func_def{FD, func_name};
    transpiler.func_manager_.AddDefinition(func_def);

    EOObject eo_func_def = func_def.GetEoObject();
    transpiler.func_manager_.AddEoObject(eo_func_def);
  } else {
    FunctionDeclaration func_decl{FD, func_name};
    transpiler.func_manager_.AddDeclaration(func_decl);
  }
}

void RecordDeclAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (!context)
    context = result.Context;
  const auto *RD = result.Nodes.getNodeAs<RecordDecl>("recordDecl");
  if (!RD)
    return;
  ProcessRecordType(RD);
}

void DeclBaseVarGlobalMemoryAnalyzer::run(const MatchFinder::MatchResult &result) {
  if (!context)
    context = result.Context;
  const auto *VD = result.Nodes.getNodeAs<VarDecl>("declBaseVarGlobalMemory");
  // We do not want to convert header files!
  if (!VD || !context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
    return;
  ProcessVariable(VD);
}
