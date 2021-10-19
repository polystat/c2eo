#ifndef __VISITORS__
#define __VISITORS__

// #include <iostream>
// #include <fstream>
// #include <sstream>
// 
// #include "clang/AST/ASTConsumer.h"
// #include "clang/AST/OperationKinds.h"
// #include "clang/AST/RecursiveASTVisitor.h"
// #include "clang/Frontend/CompilerInstance.h"
// #include "clang/Frontend/FrontendAction.h"
// #include "clang/Tooling/Tooling.h"
// #include "llvm/ADT/StringRef.h"
// 
// using namespace clang;

#include "vardecl.h"

class Visitors:
    public RecursiveASTVisitor<Visitors>
{

public:
    explicit Visitors(ASTContext *Context): Context(Context) {}

//    bool VisitRecordDecl(RecordDecl *Declaration);
    bool VisitVarDecl(VarDecl *Declaration);
//    bool VisitExpr(Expr *Declaration);
//     bool VisitFullExpr(FullExpr *Declaration);
//    bool VisitDeclRefExpr(DeclRefExpr *Declaration);
//    bool VisitBinaryOperator(BinaryOperator *Declaration);

private:
    ASTContext *Context;
};

class Consumer : public clang::ASTConsumer {
public:
    explicit Consumer(ASTContext *Context): Visitor(Context) {}

    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    Visitors Visitor;
};

class Action : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &Compiler, llvm::StringRef InFile) 
    {
        return std::make_unique<Consumer>(&Compiler.getASTContext());
    }
};

#endif // __VISITORS__
