#include "tracer.h"

using namespace clang;
using namespace llvm;
using namespace std;

__attribute__((unused)) void TraceOutASTnode(Stmt::StmtClass stmt_class) {
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

__attribute__((unused)) void TraceOutIntegerLiteral(APInt &v, bool is_signed) {
  if (is_signed) {
    std::cout << "    APInt Literal = " << v.getSExtValue() << "\n";
  } else {
    std::cout << "    APInt Literal = " << v.getZExtValue() << "\n";
  }
}

__attribute__((unused)) void TraceOutEOObject(EOObject &eo_object) {
  cout << eo_object;
}

void TraceOutFunctionDecl(const clang::FunctionDecl *FD) {
  if (FD == nullptr) {
    std::cout << "  Incorrect pointer_ to definition\n";
  } else {
    DeclarationNameInfo decl_name_info{FD->getNameInfo()};
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
      auto *func_body = dyn_cast<CompoundStmt>(body);
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
