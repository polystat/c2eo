#ifndef __RECORDDECL__
#define __RECORDDECL__

// Заголовочный файл с описанием структур,
// формируемых при анализе класса RecordDecl

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"


using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#include "generator.h"


using namespace clang;

//-------------------------------------------------------------------------------------------------
// Определение и тестовый объектов структуры или объединения
void getRecordDeclSubObjects(const RecordDecl *RD);
std::vector<RecordGen*> getAllRecordDeclSubObjects(const RecordDecl* RD);

void setSubFields(RecordGen* RG, const RecordDecl* RD);


#endif //__RECORDDECL__
