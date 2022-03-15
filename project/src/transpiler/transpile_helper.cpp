#include "transpile_helper.h"
#include "unit_transpiler.h"
#include <queue>
using namespace clang;
using namespace llvm;
using namespace std;

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator);

EOObject GetIfStmtEOObject(const IfStmt *p_stmt);
EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt);
EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt);
EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal);
EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator);
EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  const CompoundStmt* funcBody = dyn_cast<CompoundStmt>(FD->getBody());
  if(!funcBody)
    return EOObject(EOObjectType::EO_EMPTY);
  return GetCompoundStmt(funcBody,true);
}
//Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS, bool is_decorator) {
  EOObject res {"seq"};
  if (is_decorator)
    res.postfix = "@";
  for (auto stmt : CS->body() ) {
    Stmt::StmtClass stmtClass = stmt->getStmtClass();
    // Костыльное решение для тестового выводо
    if (stmtClass == Stmt::ImplicitCastExprClass) // Нужно разобраться с именами перчислимых типов
    {
      EOObject printer {"printf"};
      printer.nested.emplace_back(R"("%d\n")",EOObjectType::EO_LITERAL);
      EOObject read_val {"read-as-int64"};
      extern UnitTranspiler transpiler;
      //TODO fix unsafety code
      read_val.nested.emplace_back(transpiler.glob.GetVarByID(
          reinterpret_cast<uint64_t>(dyn_cast<DeclRefExpr>(*stmt->child_begin())->getFoundDecl())).alias);
      printer.nested.push_back(read_val);
      res.nested.push_back(printer);
      continue;
    }
    EOObject stmt_obj = GetStmtEOObject(stmt);
    res.nested.push_back(stmt_obj);
  }
  res.nested.emplace_back("TRUE",EOObjectType::EO_LITERAL);
  return res;
}

EOObject GetStmtEOObject(const Stmt* stmt) {
  Stmt::StmtClass stmtClass = stmt->getStmtClass();
  if (stmtClass == Stmt::BinaryOperatorClass) {
    const auto* op = dyn_cast<BinaryOperator>(stmt);
    return GetBinaryStmtEOObject(op);
  } else if (stmtClass == Stmt::ParenExprClass) {
    const auto* op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::ImplicitCastExprClass) {
    const auto* op = dyn_cast<ImplicitCastExpr>(stmt);
    //TODO if cast kinds and also split it to another func
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::DeclRefExprClass) {
    const auto *op = dyn_cast<DeclRefExpr>(stmt);
    extern UnitTranspiler transpiler;
    string type_name = "-"+GetTypeName(op->getType());
    EOObject variable {"read-as"+type_name};
    //TODO fix unsafety code
    variable.nested.emplace_back(transpiler.glob.GetVarByID(reinterpret_cast<uint64_t>(op->getFoundDecl())).alias);
    return variable;
  } else if (stmtClass == Stmt::IfStmtClass) {
    const auto* op = dyn_cast<IfStmt>(stmt);
    return GetIfStmtEOObject(op);
  } else if (stmtClass == Stmt::WhileStmtClass) {
    const auto* op = dyn_cast<WhileStmt>(stmt);
    return GetWhileStmtEOObject(op);
  } else if (stmtClass == Stmt::DoStmtClass) {
    const auto* op = dyn_cast<DoStmt>(stmt);
    return GetDoWhileStmtEOObject(op);
  } else if (stmtClass == Stmt::CompoundStmtClass) {
    const auto* op = dyn_cast<CompoundStmt>(stmt);
    return GetCompoundStmt(op);
  } else if (stmtClass == Stmt::IntegerLiteralClass) {
    const auto* op = dyn_cast<IntegerLiteral>(stmt);
    return GetIntegerLiteralEOObject(op);
  }
  return EOObject(EOObjectType::EO_EMPTY);
}
EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal) {
  APInt an_int = p_literal->getValue();
  bool is_signed = p_literal->getType()->isSignedIntegerType();
  return {an_int.toString(10, is_signed),EOObjectType::EO_LITERAL};
}
EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  auto opCode = p_operator->getOpcode();
  std::string operation;
  if (opCode == BinaryOperatorKind::BO_Assign) {
    return GetAssignmentOperatorEOObject(p_operator);
  } else if (opCode == BinaryOperatorKind::BO_Add) {
    operation = "add";
  } else if (opCode == BinaryOperatorKind::BO_Sub) {
    operation = "sub";
  } else if (opCode == BinaryOperatorKind::BO_Mul) {
    operation = "mul";
  } else if (opCode == BinaryOperatorKind::BO_Div) {
    operation = "div";
  } else if (opCode == BinaryOperatorKind::BO_Rem) {
    operation = "mod";
  } else if (opCode == BinaryOperatorKind::BO_And) {
    operation = "bit-and";
  } else if (opCode == BinaryOperatorKind::BO_Or) {
    operation = "bit-or";
  } else if (opCode == BinaryOperatorKind::BO_Xor) {
    operation = "bit-xor";
  } else if (opCode == BinaryOperatorKind::BO_Shl) {
    operation = "shift-left";
  } else if (opCode == BinaryOperatorKind::BO_Shr) {
    operation = "shift-right";
  } else if (opCode == BinaryOperatorKind::BO_EQ) {
    operation = "eq";
  } else if (opCode == BinaryOperatorKind::BO_NE) {
    operation = "neq";
  } else if (opCode == BinaryOperatorKind::BO_LT) {
    operation = "less";
  } else if (opCode == BinaryOperatorKind::BO_LE) {
    operation = "leq";
  } else if (opCode == BinaryOperatorKind::BO_GT) {
    operation = "greater";
  } else if (opCode == BinaryOperatorKind::BO_GE) {
    operation = "geq";
  } else {
    operation = "undefined";
  }

  EOObject binop {operation};
  binop.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}
EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator) {
  EOObject binop {"write"};
  extern UnitTranspiler transpiler;
  const auto *op = dyn_cast<DeclRefExpr>(p_operator->getLHS());
  if (op)
  {
    binop.nested.emplace_back(transpiler.glob.GetVarByID(reinterpret_cast<uint64_t>(op->getFoundDecl())).alias);
  } else {
    binop.nested.emplace_back(EOObjectType::EO_EMPTY);
  }
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}
EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_stmt {"if"};
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getThen()));
  if (p_stmt->hasElseStorage()) {
    if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getElse()));
  } else {
    EOObject empty_seq {"seq"};
    empty_seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
    if_stmt.nested.push_back(empty_seq);
  }
  return if_stmt;
}

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt) {
  EOObject while_stmt {"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  return while_stmt;
}

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt) {
  EOObject do_stmt {"seq"};
  do_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  EOObject while_stmt {"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  do_stmt.nested.push_back(while_stmt);
  do_stmt.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return do_stmt;
}

std::string GetTypeName(QualType qualType)
{
  extern ASTContext* context;
  const clang::Type* typePtr = qualType.getTypePtr();
  TypeInfo typeInfo = context->getTypeInfo(typePtr);
  uint64_t typeSize = typeInfo.Width;
  std::string str;

  if (typePtr->isBooleanType()) {
    str += "bool";
    return str;
  }
  if (typePtr->isFloatingType()) {
    str += "float" + std::to_string(typeSize);
    return str;
  }

  if (!typePtr->isSignedIntegerType())
    str += "u";
  if (typePtr->isCharType()) {
    str += "char";
    return str;
  }
  if (typePtr->isIntegerType()) {
    str += "int" + std::to_string(typeSize);
    return str;
  }


  if (typePtr->isUnionType())
    str = "un_";
  if (typePtr->isStructureType())
    str = "st_";
  if (typePtr->isUnionType() || typePtr->isStructureType()) {
    RecordDecl* RD = typePtr->getAsRecordDecl();
    if (RD->hasNameForLinkage())
      str += RD->getNameAsString();
    else
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    return str;
  }
  return "undefinedtype";
}

std::set<std::string> FindAllExternalObjects(EOObject obj) {
  std::set<std::string> all_known = {obj.postfix};
  std::set<std::string> unknown = {};
  //TODO maybe should use pointers or copy constructor to avoid unnecessary copying of objects
  std::queue<EOObject> not_visited ;
  for(auto child : obj.nested)
  {
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty())
  {
    EOObject cur = not_visited.front();
    not_visited.pop();
    switch (cur.type) {
      case EOObjectType::EO_ABSTRACT:
        all_known.insert(cur.postfix);
        break;
      case EOObjectType::EO_COMPLETE:
        all_known.insert(cur.postfix);
        if (all_known.find(cur.name)==all_known.end())
          unknown.insert(cur.name);
        break;
      case EOObjectType::EO_EMPTY:
        unknown.insert("plug");
        break;
      case EOObjectType::EO_LITERAL: break;
    }
    for(auto child : cur.nested)
    {
      not_visited.push(std::move(child));
    }
  }
  for (const auto& known_obj : all_known) {
    unknown.erase(known_obj);
  }

  return unknown;
}


