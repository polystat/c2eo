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

#include "src/transpiler/tracer.h"

#include <string>

using clang::BinaryOperatorKind;
using clang::Stmt;

__attribute__((unused)) void TraceOutASTNode(Stmt::StmtClass stmt_class) {
  std::string message;
  switch (stmt_class) {
    case Stmt::BinaryOperatorClass:
      message = "BinaryOperator";
      break;
    case Stmt::CallExprClass:
      message = "CallExpr";
      break;
    case Stmt::CompoundAssignOperatorClass:
      message = "CompoundAssignOperator";
      break;
    case Stmt::CompoundStmtClass:
      message = "CompoundStmt";
      break;
    case Stmt::DeclRefExprClass:
      message = "DeclRefExpr";
      break;
    case Stmt::DeclStmtClass:
      message = "DeclStmt";
      break;
    case Stmt::DoStmtClass:
      message = "DoStmt";
      break;
    case Stmt::FloatingLiteralClass:
      message = "FloatingLiteral";
      break;
    case Stmt::IfStmtClass:
      message = "IfStmt";
      break;
    case Stmt::ImplicitCastExprClass:
      message = "ImplicitCastExpr";
      break;
    case Stmt::IntegerLiteralClass:
      message = "IntegerLiteral";
      break;
    case Stmt::ParenExprClass:
      message = "ParenExpr";
      break;
    case Stmt::ReturnStmtClass:
      message = "ReturnStmt";
      break;
    case Stmt::UnaryOperatorClass:
      message = "UnaryOperator";
      break;
    case Stmt::WhileStmtClass:
      message = "WhileStmt";
      break;
    default:
      message = "Unknown Object Class";
  }
  std::cout << "AST stmt class = " << message << "\n";
}

__attribute__((unused)) void TraceOutBinaryOperator(BinaryOperatorKind kind) {
  std::string message;
  switch (kind) {
    case BinaryOperatorKind::BO_Assign:
      message = "Assign (=)";
      break;
    case BinaryOperatorKind::BO_Add:
      message = "Add (+)";
      break;
    case BinaryOperatorKind::BO_Sub:
      message = "Sub (-)";
      break;
    case BinaryOperatorKind::BO_Mul:
      message = "Mul (*)";
      break;
    case BinaryOperatorKind::BO_Div:
      message = "Div (/)";
      break;
    case BinaryOperatorKind::BO_Rem:
      message = "Rem (%)";
      break;
    case BinaryOperatorKind::BO_And:
      message = "And (&)";
      break;
    case BinaryOperatorKind::BO_Or:
      message = "Or (|)";
      break;
    case BinaryOperatorKind::BO_Xor:
      message = "Xor (^)";
      break;
    case BinaryOperatorKind::BO_Shl:
      message = "Shl (<<)";
      break;
    case BinaryOperatorKind::BO_Shr:
      message = "Shr (>>)";
      break;
    case BinaryOperatorKind::BO_EQ:
      message = "EQ (==)";
      break;
    case BinaryOperatorKind::BO_NE:
      message = "NE (!=)";
      break;
    case BinaryOperatorKind::BO_LT:
      message = "LT (<)";
      break;
    case BinaryOperatorKind::BO_LE:
      message = "LE (<=)";
      break;
    case BinaryOperatorKind::BO_GT:
      message = "GT (>)";
      break;
    case BinaryOperatorKind::BO_GE:
      message = "GE (>=)";
      break;
    default:
      message = "Unknown Binary Operation Kind";
  }
  std::cout << "  AST binary operation = " << message << "\n";
}

__attribute__((unused)) void TraceOutIntegerLiteral(llvm::APInt &v,
                                                    bool is_signed) {
  if (is_signed) {
    std::cout << "    APInt Literal = " << v.getSExtValue() << "\n";
  } else {
    std::cout << "    APInt Literal = " << v.getZExtValue() << "\n";
  }
}

__attribute__((unused)) void TraceOutEOObject(EOObject &eo_object) {
  std::cout << eo_object;
}

void TraceOutFunctionDecl(const clang::FunctionDecl *FD) {
  if (FD == nullptr) {
    std::cout << "  Incorrect pointer_ to definition\n";
  } else {
    clang::DeclarationNameInfo decl_name_info{FD->getNameInfo()};
    std::string func_name{decl_name_info.getAsString()};
    std::cout << func_name << ": ";
    std::cout.flush();

    if (FD->isDefined()) {
      std::cout << "  Defined!\n";
    } else {
      std::cout << "  No defined!\n";
    }

    if (FD->hasBody()) {
      std::cout << "  Has body!\n";
      Stmt *body = FD->getBody();
      auto *func_body = llvm::dyn_cast<clang::CompoundStmt>(body);
      if (func_body != nullptr) {
        std::cout << "  Has body! Body pointer_ =  " << body << "\n";
        if (func_body->size() > 0) {
          int i = 0;
          for (__attribute__((unused)) auto *stmt : func_body->body()) {
            std::cout << "    Statement # " << i++ << "\n";
          }
        } else {
          std::cout << "    The body is empty\n";
        }
      }
    } else {
      std::cout << "  Body is absent!\n";
    }
  }
}
