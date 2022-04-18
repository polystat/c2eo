// Вспомогательные функции, осуществляющие трассировку программы
#include "tracer.h"

// Вывод узла синтаксического дерева
void TraceOutASTnode(Stmt::StmtClass stmtClass) {
  std::string message;
  switch(stmtClass) {
    case Stmt::BinaryOperatorClass: message = "BinaryOperator"; break;
    case Stmt::CallExprClass: message = "CallExpr"; break;
    case Stmt::CompoundAssignOperatorClass: message = "CompoundAssignOperator"; break;
    case Stmt::CompoundStmtClass: message = "CompoundStmt"; break;
    case Stmt::DeclRefExprClass: message = "DeclRefExpr"; break;
    case Stmt::DeclStmtClass: message = "DeclStmt"; break;
    case Stmt::DoStmtClass: message = "DoStmt"; break;
    case Stmt::FloatingLiteralClass: message = "FloatingLiteral"; break;
    case Stmt::IfStmtClass: message = "IfStmt"; break;
    case Stmt::ImplicitCastExprClass: message = "ImplicitCastExpr"; break;
    case Stmt::IntegerLiteralClass: message = "IntegerLiteral"; break;
    case Stmt::ParenExprClass: message = "ParenExpr"; break;
    case Stmt::ReturnStmtClass: message = "ReturnStmt"; break;
    case Stmt::UnaryOperatorClass: message = "UnaryOperator"; break;
    case Stmt::WhileStmtClass: message = "WhileStmt"; break;
//     case : message = ""; break;
    default:
      message = "Unknown Object Class";
  }
  std::cout << "AST stmt class = " << message << "\n";
}

// Тестовый вывод информации о бинарном операторе
void TraceOutBinaryOperator(BinaryOperatorKind kind) {
  std::string message;
  switch(kind) {
    case BinaryOperatorKind::BO_Assign: message = "Assign (=)"; break;
    case BinaryOperatorKind::BO_Add: message = "Add (+)"; break;
    case BinaryOperatorKind::BO_Sub: message = "Sub (-)"; break;
    case BinaryOperatorKind::BO_Mul: message = "Mul (*)"; break;
    case BinaryOperatorKind::BO_Div: message = "Div (/)"; break;
    case BinaryOperatorKind::BO_Rem: message = "Rem (%)"; break;
    case BinaryOperatorKind::BO_And: message = "And (&)"; break;
    case BinaryOperatorKind::BO_Or: message = "Or (|)"; break;
    case BinaryOperatorKind::BO_Xor: message = "Xor (^)"; break;
    case BinaryOperatorKind::BO_Shl: message = "Shl (<<)"; break;
    case BinaryOperatorKind::BO_Shr: message = "Shr (>>)"; break;
    case BinaryOperatorKind::BO_EQ: message = "EQ (==)"; break;
    case BinaryOperatorKind::BO_NE: message = "NE (!=)"; break;
    case BinaryOperatorKind::BO_LT: message = "LT (<)"; break;
    case BinaryOperatorKind::BO_LE: message = "LE (<=)"; break;
    case BinaryOperatorKind::BO_GT: message = "GT (>)"; break;
    case BinaryOperatorKind::BO_GE: message = "GE (>=)"; break;
//    case : message = ""; break;
    default:
      message = "Unknown Binary Operantion Kind";
  }
  std::cout << "  AST binary operation = " << message << "\n";
}

// Тестовый вывод информации о целочисленных литералах
void TraceOutIntegerLiteral(APInt &v, bool is_signed) {
//   std::string message{v.toString(10, is_signed)};
//   toString(message, 10, true);
  int64_t val = 0;
  if(is_signed) {
    val = v.getSExtValue();
  }
  else {
    val = v.getZExtValue();
  }
  std::cout << "    APInt Literal = " << val << "\n";
}

// Тестовый вывод объекта EO
void TraceOutEOObject(EOObject &eoObject) {
  cout << eoObject;
}

// Тестовый вывод объекта содержимого функции
void TraceOutFunctionDecl(const clang::FunctionDecl* FD) {
  if (FD == nullptr) {
    std::cout << "  Incorrect pointer to definition\n";
  }
  else {
    // Вывод содержимого функции
    DeclarationNameInfo declNameInfo{FD->getNameInfo()};
    std::string func_name{declNameInfo.getAsString()};
    std::cout << func_name << ": ";
    std::cout.flush();

    if(FD->isDefined()) {
      std::cout << "  Defined!\n";
    }
    else {
      std::cout << "  No defined!\n";
    }

    if(FD->hasBody()) {
      std::cout << "  Has body!\n";
      Stmt* body = FD->getBody();
      CompoundStmt* funcBody = dyn_cast<CompoundStmt>(body);
      if(funcBody != nullptr) {
        std::cout << "  Has body! Body pointer =  " << body << "\n";
        if(funcBody->size() > 0) {
          int i = 0;
          for (auto stmt: funcBody->body()) {
            Stmt::StmtClass stmtClass = stmt->getStmtClass();
            std::cout << "    Statement # " << i++ << "\n";
          }
        }
        else {
          std::cout << "    The body is empty\n";
        }
      }
    }
    else {
      std::cout << "  Body is absent!\n";
    }
  }
}
