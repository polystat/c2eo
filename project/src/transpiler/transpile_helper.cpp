#include "transpile_helper.h"
#include "memory_manager.h"
#include "unit_transpiler.h"
#include "vardecl.h"
#include <queue>
#include <sstream>
using namespace clang;
using namespace llvm;
using namespace std;

vector<Variable> ProcessFunctionLocalVariables(const clang::CompoundStmt *CS, size_t shift);

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator);
EOObject GetAssignmentOperationOperatorEOObject(const CompoundAssignOperator *p_operator);

EOObject GetIfStmtEOObject(const IfStmt *p_stmt);
EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt);
EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt);
EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal);
EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator);
EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator);

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal);
EOObject GetFunctionCallEOObject(const CallExpr *op);
vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift);
size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params);
extern UnitTranspiler transpiler;

EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  if(!FD->hasBody())
    //TODO if not body may be need to create simple complete or abstract object with correct name
    return EOObject(EOObjectType::EO_EMPTY);
  const auto funcBody = dyn_cast<CompoundStmt>(FD->getBody());
  size_t shift = transpiler.glob.RealMemorySize();
  size_t param_memory_size = GetParamMemorySize(FD->parameters());
  vector<Variable> all_param = ProcessFunctionParams(FD->parameters(), shift);
  vector<Variable> all_local = ProcessFunctionLocalVariables(funcBody, shift + param_memory_size);
  EOObject func_body_eo = EOObject(EOObjectType::EO_EMPTY);
  EOObject local_start("add","local-start");
  local_start.nested.emplace_back("param-start");
  local_start.nested.emplace_back("param-size");
  func_body_eo.nested.push_back(local_start);
  size_t free_pointer = transpiler.glob.RealMemorySize();
  EOObject local_empty_position("add","empty-local-position");
  local_empty_position.nested.emplace_back("local-start");
  local_empty_position.nested.emplace_back(to_string(free_pointer - shift - param_memory_size),
                                           EOObjectType::EO_LITERAL);
  func_body_eo.nested.push_back(local_empty_position);
  for (const auto& param : all_param)
  {
    func_body_eo.nested.push_back(param.GetAddress(transpiler.glob.name));
  }
  for (const auto& var : all_local)
  {
    func_body_eo.nested.push_back(var.GetAddress(transpiler.glob.name));
  }
  EOObject body_seq = GetCompoundStmt(funcBody,true);
  std::reverse(all_local.begin(), all_local.end());
  for(const auto& var :all_local)
  {
    if (var.is_initialized)
      body_seq.nested.insert(body_seq.nested.begin(),var.GetInitializer());
  }
  func_body_eo.nested.push_back(body_seq);
  transpiler.glob.RemoveAllUsed(all_param);
  transpiler.glob.RemoveAllUsed(all_local);

  return func_body_eo;
}
size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params) {
  size_t res = 0;
  for (auto VD : params)
  {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / 8;
    res += type_size;
  }
  return res;
}
vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift) {
  vector<Variable> all_params;
  for (auto param : params ) {
    all_params.push_back(ProcessVariable(param, "param-start",shift));
  }
  return all_params;
}

vector<Variable> ProcessFunctionLocalVariables(const clang::CompoundStmt *CS, size_t shift)
{
  vector<Variable> all_local;
  for (auto stmt : CS->body() ) {
    Stmt::StmtClass stmtClass = stmt->getStmtClass();
    if (stmtClass == Stmt::DeclStmtClass)
    {
      auto decl_stmt = dyn_cast<DeclStmt>(stmt);
      for( auto decl : decl_stmt->decls())
      {
        // TODO if var created in nested statement we don't found it. Fix
        // TODO duplicate names problem should be resoved
        Decl::Kind decl_kind = decl->getKind();
        if (decl_kind == Decl::Kind::Var)
        {
          auto var_decl = dyn_cast<VarDecl>(decl);
          all_local.push_back(ProcessVariable(var_decl,"local-start", shift));
        }
      }
    }
  }
  return all_local;
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
      auto ref = dyn_cast<DeclRefExpr>(*stmt->child_begin());
      if (!ref)
        continue;
      try {
        const Variable &var = transpiler.glob.GetVarByID(dyn_cast<VarDecl>(ref->getFoundDecl()));
        string formatter = "d";
        if (var.type_postfix == "float32" || var.type_postfix == "float64")
          formatter = "f";
        EOObject printer{"printf"};
        printer.nested.emplace_back("\"%" + formatter + "\\n\"", EOObjectType::EO_LITERAL);
        EOObject read_val{"read-as-" + var.type_postfix};
        read_val.nested.emplace_back(var.alias);
        printer.nested.push_back(read_val);
        res.nested.push_back(printer);
      }
      catch (invalid_argument&)
      {
        res.nested.emplace_back(EOObjectType::EO_PLUG);
      }
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
  } else if (stmtClass == Stmt::CompoundAssignOperatorClass) {
      const auto *op = dyn_cast<CompoundAssignOperator>(stmt);
      return GetAssignmentOperationOperatorEOObject(op);
  } else if (stmtClass == Stmt::ParenExprClass) {
    const auto* op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::ImplicitCastExprClass) {
    const auto* op = dyn_cast<ImplicitCastExpr>(stmt);
    //TODO if cast kinds and also split it to another func
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::DeclRefExprClass) {
    auto ref = dyn_cast<DeclRefExpr>(stmt);
    if (!ref)
      return EOObject{EOObjectType::EO_PLUG};
    try {
      const Variable& var = transpiler.glob.GetVarByID(dyn_cast<VarDecl>(ref->getFoundDecl()));
      EOObject variable {"read-as-"+var.type_postfix};
      variable.nested.emplace_back(var.alias);
      return variable;
    }
    catch (invalid_argument& er)
    {
      cerr << er.what() << "\n";
      return EOObject(EOObjectType::EO_PLUG);
    }

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
  } else if (stmtClass == Stmt::FloatingLiteralClass) {
    const auto* op = dyn_cast<FloatingLiteral>(stmt);
    return GetFloatingLiteralEOObject(op);
  } else if (stmtClass == Stmt::DeclStmtClass) {
    return EOObject(EOObjectType::EO_EMPTY);
  }else if (stmtClass == Stmt::CallExprClass) {
    const auto* op = dyn_cast<CallExpr>(stmt);
    return GetFunctionCallEOObject(op);
  }
  return EOObject(EOObjectType::EO_PLUG);
}
EOObject GetFunctionCallEOObject(const CallExpr *op) {
  EOObject call(EOObjectType::EO_EMPTY);
  vector<std::size_t> var_sizes;
  for (auto VD :op->getDirectCallee()->parameters())
  {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / 8;
    var_sizes.push_back(type_size);
  }
  size_t shift = 0;
  int i = 0;
  for (auto arg : op->arguments()) {
    EOObject param{"write"};
    EOObject address {"address"};
    address.nested.emplace_back("global-ram");
    EOObject add {"add"};
    add.nested.emplace_back("empty-local-position");
    add.nested.emplace_back(to_string(shift),EOObjectType::EO_LITERAL);
    address.nested.push_back(add);
    param.nested.push_back(address);
    param.nested.push_back(GetStmtEOObject(arg));
    shift += var_sizes[i];
    //may be it will works with param.
    i = i == var_sizes.size()-1 ? i : i + 1;
    call.nested.push_back(param);
  }
  call.nested.push_back(transpiler.func_manager.GetFunctionCall(op->getDirectCallee(), shift));
  return call;
}

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal) {
  APFloat an_float = p_literal->getValue();
  ostringstream ss{};
  ss << fixed << an_float.convertToDouble() ;
  return {ss.str(),EOObjectType::EO_LITERAL};
}
EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal) {
  bool is_signed = p_literal->getType()->isSignedIntegerType();
  APInt an_int = p_literal->getValue();
  return {an_int.toString(10, is_signed),EOObjectType::EO_LITERAL};
}
EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator) {
    auto op_code = p_operator->getOpcode();
    std::string operation;

    if (op_code == BinaryOperatorKind::BO_AddAssign) {
        operation = "add";
    } else if (op_code == BinaryOperatorKind::BO_SubAssign) {
        operation = "sub";
    } else if (op_code == BinaryOperatorKind::BO_MulAssign) {
        operation = "mul";
    } else if (op_code == BinaryOperatorKind::BO_DivAssign) {
        operation = "div";
    } else if (op_code == BinaryOperatorKind::BO_RemAssign) {
        operation = "mod";
    } else if (op_code == BinaryOperatorKind::BO_AndAssign) {
        operation = "bit-and";
    } else if (op_code == BinaryOperatorKind::BO_XorAssign) {
        operation = "bit-xor";
    } else if (op_code == BinaryOperatorKind::BO_OrAssign) {
        operation = "bit-or";
    } else if (op_code == BinaryOperatorKind::BO_ShlAssign) {
        operation = "shift-left";
    } else if (op_code == BinaryOperatorKind::BO_ShrAssign) {
        operation = "shift-right";
    }

    EOObject binop{operation};
    binop.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
    binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
    return binop;
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
  const auto *op = dyn_cast<DeclRefExpr>(p_operator->getLHS());
  if (op)
  {
    try{
      const Variable& var = transpiler.glob.GetVarByID(dyn_cast<VarDecl>(op->getFoundDecl()));
      binop.nested.emplace_back(var.alias);
    }
    catch (invalid_argument&)
    {
      binop.nested.emplace_back(EOObjectType::EO_LITERAL);
    }
  } else {
    binop.nested.emplace_back(EOObjectType::EO_EMPTY);
  }

  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}
EOObject GetAssignmentOperationOperatorEOObject(const CompoundAssignOperator *p_operator) {
    EOObject binop {"write"};
    const auto *op = dyn_cast<DeclRefExpr>(p_operator->getLHS());
    if (op)
    {
        try{
            const Variable& var = transpiler.glob.GetVarByID(dyn_cast<VarDecl>(op->getFoundDecl()));
            binop.nested.emplace_back(var.alias);
        }
        catch (invalid_argument&)
        {
            binop.nested.emplace_back(EOObjectType::EO_LITERAL);
        }
    } else {
        binop.nested.emplace_back(EOObjectType::EO_EMPTY);
    }

    binop.nested.push_back(GetCompoundAssignEOObject(p_operator));
    return binop;
}
EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_stmt {"if"};
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  //TODO then and else is seq everytime!
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
  //TODO body is seq everytime!
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  return while_stmt;
}

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt) {
  EOObject do_stmt {"seq"};
  do_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  EOObject while_stmt {"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  //TODO body is seq everytime!
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

std::set<std::string> FindAllExternalObjects(const EOObject& obj) {
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
        for(const auto& arg : cur.arguments)
          all_known.insert(arg);
        break;
      case EOObjectType::EO_COMPLETE:
        all_known.insert(cur.postfix);
        if (all_known.find(cur.name)==all_known.end())
          unknown.insert(cur.name);
        break;
      case EOObjectType::EO_EMPTY:
      case EOObjectType::EO_LITERAL: break;
      case EOObjectType::EO_PLUG:
        if(cur.nested.empty())
          unknown.insert("plug");
        break;
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

