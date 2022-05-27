#ifndef C2EO_SRC_TRANSPILER_TRACER_H_
#define C2EO_SRC_TRANSPILER_TRACER_H_

#include <iostream>
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"
#include "llvm/ADT/APInt.h"

// #define TRACEOUT
// #define TRACEOUT_EO
#define TRACEOUT_NEW_EO
// #define TRACEOUT_FUNC_DEF

#include "eo_object.h"

using namespace clang;
using namespace llvm;
using namespace std;

__attribute__((unused)) void TraceOutASTnode(clang::Stmt::StmtClass stmt_class);

__attribute__((unused)) void TraceOutBinaryOperator(BinaryOperatorKind kind);

__attribute__((unused)) void TraceOutIntegerLiteral(APInt &v, bool is_signed);

__attribute__((unused)) void TraceOutEOObject(EOObject &eo_object);

void TraceOutFunctionDecl(const clang::FunctionDecl* FD);

#endif // C2EO_SRC_TRANSPILER_TRACER_H_

