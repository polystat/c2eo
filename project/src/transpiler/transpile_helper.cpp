
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

EOObject GetUnaryStmtEOObject(const UnaryOperator *p_operator);

EOObject GetIfStmtEOObject(const IfStmt *p_stmt);

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt);

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt);

EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal);

EOObject GetReturnStmpEOObject(const ReturnStmt *p_stmt);

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator);

EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator);

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal);

EOObject GetFunctionCallEOObject(const CallExpr *op);

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift);

size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params);

EOObject GetMemberExprEOObject(const MemberExpr *opr);

EOObject GetEODeclRefExpr(const DeclRefExpr *op);

extern UnitTranspiler transpiler;

EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  if (!FD->hasBody())
    // TODO if not body may be need to create simple complete or abstract object with correct name
    return EOObject(EOObjectType::EO_EMPTY);
  const auto funcBody = dyn_cast<CompoundStmt>(FD->getBody());
  size_t shift = transpiler.glob.RealMemorySize();
  size_t param_memory_size = GetParamMemorySize(FD->parameters());
  vector<Variable> all_param = ProcessFunctionParams(FD->parameters(), shift);
  vector<Variable> all_local = ProcessFunctionLocalVariables(funcBody, shift + param_memory_size);
  EOObject func_body_eo = EOObject(EOObjectType::EO_EMPTY);
  EOObject local_start("add", "local-start");
  local_start.nested.emplace_back("param-start");
  local_start.nested.emplace_back("param-size");
  func_body_eo.nested.push_back(local_start);
  size_t free_pointer = transpiler.glob.RealMemorySize();
  EOObject local_empty_position("add", "empty-local-position");
  local_empty_position.nested.emplace_back("local-start");
  local_empty_position.nested.emplace_back(to_string(free_pointer - shift - param_memory_size),
                                           EOObjectType::EO_LITERAL);
  func_body_eo.nested.push_back(local_empty_position);
  for (const auto &param: all_param) {
    func_body_eo.nested.push_back(param.GetAddress(transpiler.glob.name));
  }
  for (const auto &var: all_local) {
    func_body_eo.nested.push_back(var.GetAddress(transpiler.glob.name));
  }
  EOObject body_seq = GetCompoundStmt(funcBody, true);
  std::reverse(all_local.begin(), all_local.end());
  for (const auto &var: all_local) {
    if (var.is_initialized)
      body_seq.nested.insert(body_seq.nested.begin(), var.GetInitializer());
  }
  func_body_eo.nested.push_back(body_seq);
  transpiler.glob.RemoveAllUsed(all_param);
  transpiler.glob.RemoveAllUsed(all_local);

  return func_body_eo;
}

size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params) {
  size_t res = 0;
  for (auto VD: params) {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / 8;
    res += type_size;
  }
  return res;
}

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift) {
  vector<Variable> all_params;
  for (auto param: params) {
    all_params.push_back(ProcessVariable(param, "param-start", shift));
  }
  return all_params;
}

vector<Variable> ProcessFunctionLocalVariables(const clang::CompoundStmt *CS, size_t shift) {
  vector<Variable> all_local;
  for (auto stmt: CS->body()) {
    Stmt::StmtClass stmtClass = stmt->getStmtClass();
    if (stmtClass == Stmt::DeclStmtClass) {
      auto decl_stmt = dyn_cast<DeclStmt>(stmt);
      for (auto decl: decl_stmt->decls()) {
        // TODO if var created in nested statement we don't found it. Fix
        // TODO duplicate names problem should be resoved
        Decl::Kind decl_kind = decl->getKind();
        if (decl_kind == Decl::Kind::Var) {
          auto var_decl = dyn_cast<VarDecl>(decl);
          all_local.push_back(ProcessVariable(var_decl, "local-start", shift));
        }
      }
    }
  }
  return all_local;
}

// Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS, bool is_decorator) {
  EOObject res{"seq"};
  if (is_decorator)
    res.postfix = "@";
  for (auto stmt: CS->body()) {
    Stmt::StmtClass stmtClass = stmt->getStmtClass();
    // Костыльное решение для тестового вывода
    if (stmtClass == Stmt::ImplicitCastExprClass) // Нужно разобраться с именами перечислимых типов
    {
      auto ref = dyn_cast<Expr>(*stmt->child_begin());
      string type = GetTypeName(ref->getType());
      string formatter = "?"; // todo
      if (type == "float32" || type == "float64")
        formatter = "f";
      else
        formatter = "d";
      EOObject printer{"printf"};
      printer.nested.emplace_back("\"%" + formatter + "\\n\"", EOObjectType::EO_LITERAL);
      EOObject read_val{"read-as-" + type};
      read_val.nested.emplace_back(GetStmtEOObject(ref));
      printer.nested.push_back(read_val);
      res.nested.push_back(printer);
      continue;
    }
    EOObject stmt_obj = GetStmtEOObject(stmt);
    res.nested.push_back(stmt_obj);
  }
  res.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return res;
}

EOObject GetStmtEOObject(const Stmt *stmt) {

  Stmt::StmtClass stmtClass = stmt->getStmtClass();
  if (stmtClass == Stmt::BinaryOperatorClass) {
    const auto *op = dyn_cast<BinaryOperator>(stmt);
    return GetBinaryStmtEOObject(op);
  } else if (stmtClass == Stmt::UnaryOperatorClass) {
    const auto *op = dyn_cast<UnaryOperator>(stmt);
    return GetUnaryStmtEOObject(op);
  } else if (stmtClass == Stmt::CompoundAssignOperatorClass) {
    const auto *op = dyn_cast<CompoundAssignOperator>(stmt);
    return GetAssignmentOperationOperatorEOObject(op);
  } else if (stmtClass == Stmt::ParenExprClass) {
    const auto *op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::ImplicitCastExprClass) {
    const auto *op = dyn_cast<ImplicitCastExpr>(stmt);
    if (op->getCastKind() == clang::CK_LValueToRValue) {
      string type = op->getType()->isPointerType() ? "ptr" : GetTypeName(op->getType());
      EOObject read{"read-as-" + type};
      read.nested.push_back(GetStmtEOObject(*op->child_begin()));
      return read;
    }
    // TODO if cast kinds and also split it to another func
    return GetStmtEOObject(*op->child_begin());
  } else if (stmtClass == Stmt::DeclRefExprClass) {
    auto ref = dyn_cast<DeclRefExpr>(stmt);
    return GetEODeclRefExpr(ref);
  } else if (stmtClass == Stmt::IfStmtClass) {
    const auto *op = dyn_cast<IfStmt>(stmt);
    return GetIfStmtEOObject(op);
  } else if (stmtClass == Stmt::WhileStmtClass) {
    const auto *op = dyn_cast<WhileStmt>(stmt);
    return GetWhileStmtEOObject(op);
  } else if (stmtClass == Stmt::DoStmtClass) {
    const auto *op = dyn_cast<DoStmt>(stmt);
    return GetDoWhileStmtEOObject(op);
  } else if (stmtClass == Stmt::CompoundStmtClass) {
    const auto *op = dyn_cast<CompoundStmt>(stmt);
    return GetCompoundStmt(op);
  } else if (stmtClass == Stmt::IntegerLiteralClass) {
    const auto *op = dyn_cast<IntegerLiteral>(stmt);
    return GetIntegerLiteralEOObject(op);
  } else if (stmtClass == Stmt::FloatingLiteralClass) {
    const auto *op = dyn_cast<FloatingLiteral>(stmt);
    return GetFloatingLiteralEOObject(op);
  } else if (stmtClass == Stmt::DeclStmtClass) {
    return EOObject(EOObjectType::EO_EMPTY);
  } else if (stmtClass == Stmt::CallExprClass) {
    const auto *op = dyn_cast<CallExpr>(stmt);
    return GetFunctionCallEOObject(op);
  } else if (stmtClass == Stmt::ReturnStmtClass) {
    const auto *op = dyn_cast<ReturnStmt>(stmt);
    return GetReturnStmpEOObject(op);
  } else if (stmtClass == Stmt::MemberExprClass) {
    const auto *op = dyn_cast<MemberExpr>(stmt);
    return GetMemberExprEOObject(op);
  }  else if (stmtClass == Stmt::MemberExprClass) {
    const auto *op = dyn_cast<MemberExpr>(stmt);
    return GetMemberExprEOObject(op);
  } else if (stmtClass == Stmt::ArraySubscriptExprClass) {
    const auto *op = dyn_cast<ArraySubscriptExpr>(stmt);
    // todo
  }
  return EOObject(EOObjectType::EO_PLUG);
}

EOObject GetMemberExprEOObject(const MemberExpr *op) {
  EOObject member{"add"};
  auto child = dyn_cast<Expr>(*op->child_begin());
  QualType qualType = child->getType();
  member.nested.push_back(GetStmtEOObject(child));
  member.nested.push_back(transpiler.record_manager.getShiftAlias(qualType->getAsRecordDecl(),
                                                                  op->getMemberDecl()->getNameAsString()));
  return member;
}

EOObject GetFunctionCallEOObject(const CallExpr *op) {
  // EOObject call(EOObjectType::EO_EMPTY);
  EOObject call("seq");
  vector<std::size_t> var_sizes;
  for (auto VD: op->getDirectCallee()->parameters()) {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / 8;
    var_sizes.push_back(type_size);
  }
  // Формирование оператора, задающего последовательность действий
  size_t shift = 0;
  int i = 0;
  for (auto arg: op->arguments()) {
    EOObject param{"write"};
    EOObject address{"address"};
    address.nested.emplace_back("global-ram");
    EOObject add{"add"};
    add.nested.emplace_back("empty-local-position");
    add.nested.emplace_back(to_string(shift), EOObjectType::EO_LITERAL);
    address.nested.push_back(add);
    param.nested.push_back(address);
    param.nested.push_back(GetStmtEOObject(arg));
    shift += var_sizes[i];
    // may be it will works with param.
    i = i == var_sizes.size() - 1 ? i : i + 1;
    call.nested.push_back(param);
  }
  call.nested.push_back(transpiler.func_manager.GetFunctionCall(op->getDirectCallee(), shift));

  std::string postfix = GetTypeName(op->getType());
  if (postfix != "undefinedtype") { // считается, что если тип не void,то генерация чтения данных нужна
    EOObject read_ret{"read-as-" + postfix};
    EOObject ret_val{"return"};
    read_ret.nested.push_back(ret_val);
    call.nested.push_back(read_ret);
  } else { // если тип void,то возвращается TRUE
    call.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  }

  return call;
}

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal) {
  APFloat an_float = p_literal->getValue();
  ostringstream ss{};
  ss << fixed << an_float.convertToDouble();
  return {ss.str(), EOObjectType::EO_LITERAL};
}

EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal) {
  bool is_signed = p_literal->getType()->isSignedIntegerType();
  APInt an_int = p_literal->getValue();
  return {an_int.toString(10, is_signed), EOObjectType::EO_LITERAL};
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
  Expr* left = dyn_cast<Expr>(p_operator->getLHS());
  EOObject eoObject{"read-as-"};
  eoObject.name += GetTypeName(left->getType());
  eoObject.nested.push_back(GetStmtEOObject(left));
  binop.nested.emplace_back(eoObject);

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

  EOObject binop{operation};
  binop.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}

EOObject GetUnaryStmtEOObject(const UnaryOperator *p_operator) {
  auto opCode = p_operator->getOpcode();
  std::string operation;
  Stmt *stmt = nullptr;

  // [C99 6.5.2.4] Postfix increment and decrement
  if (opCode == UnaryOperatorKind::UO_PostInc) { // UNARY_OPERATION(PostInc, "++")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  } else if (opCode == UnaryOperatorKind::UO_PostDec) { // UNARY_OPERATION(PostDec, "--")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // [C99 6.5.3.1] Prefix increment and decrement
  } else if (opCode == UnaryOperatorKind::UO_PreInc) { // UNARY_OPERATION(PreInc, "++")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  } else if (opCode == UnaryOperatorKind::UO_PreDec) { // UNARY_OPERATION(PreDec, "--")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // [C99 6.5.3.2] Address and indirection
  } else if (opCode == UnaryOperatorKind::UO_AddrOf) { // UNARY_OPERATION(AddrOf, "&")
    // stmt = p_operator->getSubExpr();
    EOObject variable{"addr-of"};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  } else if (opCode == UnaryOperatorKind::UO_Deref) { // UNARY_OPERATION(Deref, "*")
    EOObject variable{"address"};
    EOObject ram{"global-ram"};
    variable.nested.push_back(ram);
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // operation = "read-as-address";
    // EOObject unoop {operation};
    return GetStmtEOObject(p_operator->getSubExpr());
    // [C99 6.5.3.3] Unary arithmetic
  } else if (opCode == UnaryOperatorKind::UO_Plus) { // UNARY_OPERATION(Plus, "+")
    operation = "plus";
  } else if (opCode == UnaryOperatorKind::UO_Minus) { // UNARY_OPERATION(Minus, "-")
    operation = "neg";
  } else if (opCode == UnaryOperatorKind::UO_Not) { // UNARY_OPERATION(Not, "~")
    operation = "bit-not";
  } else if (opCode == UnaryOperatorKind::UO_LNot) { // UNARY_OPERATION(LNot, "!")
    operation = "not";
    // "__real expr"/"__imag expr" Extension.
  } else if (opCode == UnaryOperatorKind::UO_Real) { // UNARY_OPERATION(Real, "__real")
    operation = "real";
  } else if (opCode == UnaryOperatorKind::UO_Imag) { // UNARY_OPERATION(Imag, "__imag")
    operation = "imag";
    // __extension__ marker.
  } else if (opCode == UnaryOperatorKind::UO_Extension) { // UNARY_OPERATION(Extension, "__extension__")
    operation = "extension";
    // [C++ Coroutines] co_await operator
  } else if (opCode == UnaryOperatorKind::UO_Coawait) { // UNARY_OPERATION(Coawait, "co_await")
    operation = "coawait";
    // Incorrect unary operator
  } else {
    operation = "undefined";
  }

  EOObject unoop{operation};
  unoop.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
  return unoop;
}

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator) {
  EOObject binop{"write-as-"};
  Expr* left = dyn_cast<Expr>(p_operator->getLHS());
  binop.name += GetTypeName(left->getType());
  binop.nested.emplace_back(GetStmtEOObject(left));
  binop.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binop;
}

EOObject GetEODeclRefExpr(const DeclRefExpr *op) {
  if (!op)
    return EOObject{EOObjectType::EO_EMPTY};
  try {
    const Variable &var = transpiler.glob.GetVarByID(dyn_cast<VarDecl>(op->getFoundDecl()));
    return EOObject{var.alias};
  } catch (invalid_argument &) {
    return EOObject{EOObjectType::EO_LITERAL};
  }
}

EOObject GetAssignmentOperationOperatorEOObject(const CompoundAssignOperator *p_operator) {
  EOObject binop{"write-as-"};
  Expr* left = dyn_cast<Expr>(p_operator->getLHS());
  binop.name += GetTypeName(left->getType());
  binop.nested.emplace_back(GetStmtEOObject(left));
  binop.nested.push_back(GetCompoundAssignEOObject(p_operator));
  return binop;
}

EOObject GetReturnStmpEOObject(const ReturnStmt *p_stmt) {
  // TODO: Нужно сделать write-as-...
  EOObject ret{"write"};
  EOObject addr{"return"};
  ret.nested.push_back(addr);
  ret.nested.push_back(GetStmtEOObject(p_stmt->getRetValue()));
  return ret;
}

EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_stmt{"if"};
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  // TODO then and else is seq everytime!
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getThen()));
  if (p_stmt->hasElseStorage()) {
    if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getElse()));
  } else {
    EOObject empty_seq{"seq"};
    empty_seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
    if_stmt.nested.push_back(empty_seq);
  }
  return if_stmt;
}

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt) {
  EOObject while_stmt{"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  // TODO body is seq everytime!
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  return while_stmt;
}

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt) {
  EOObject do_stmt{"seq"};
  do_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  EOObject while_stmt{"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  // TODO body is seq everytime!
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  do_stmt.nested.push_back(while_stmt);
  do_stmt.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return do_stmt;
}

std::string GetTypeName(QualType qualType) {
  extern ASTContext *context;
  const clang::Type *typePtr = qualType.getTypePtr();
  TypeInfo typeInfo = context->getTypeInfo(typePtr);
  uint64_t typeSize = typeInfo.Width;
  std::string str{""};

  if (typePtr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (typePtr->isPointerType()) {
    // str += "int64";
    str += "ptr";
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
    RecordDecl *RD = typePtr->getAsRecordDecl();
    if (RD->hasNameForLinkage())
      str += RD->getNameAsString();
    else
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    return str;
  }

  return "undefinedtype";
}

std::set<std::string> FindAllExternalObjects(const EOObject &obj) {
  std::set<std::string> all_known = {obj.postfix};
  std::set<std::string> unknown = {};
  // TODO maybe should use pointers or copy constructor to avoid unnecessary copying of objects
  std::queue<EOObject> not_visited;
  for (auto child: obj.nested) {
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty()) {
    EOObject cur = not_visited.front();
    not_visited.pop();
    switch (cur.type) {
      case EOObjectType::EO_ABSTRACT:

        all_known.insert(cur.postfix);
        for (const auto &arg: cur.arguments)
          all_known.insert(arg);
        break;
      case EOObjectType::EO_COMPLETE:
        all_known.insert(cur.postfix);
        if (all_known.find(cur.name) == all_known.end())
          unknown.insert(cur.name);
        break;
      case EOObjectType::EO_EMPTY:
      case EOObjectType::EO_LITERAL:
        break;
      case EOObjectType::EO_PLUG:
        if (cur.nested.empty())
          unknown.insert("plug");
        break;
    }
    for (auto child: cur.nested) {
      not_visited.push(std::move(child));
    }
  }
  for (const auto &known_obj: all_known) {
    unknown.erase(known_obj);
  }

  return unknown;
}


