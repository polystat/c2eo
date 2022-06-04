
#include "transpile_helper.h"
#include "memory_manager.h"
#include "unit_transpiler.h"
#include "vardecl.h"
#include "recorddecl.h"
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

EOObject GetReturnStmtEOObject(const ReturnStmt *p_stmt);

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator);

EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator);

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal);

EOObject GetFunctionCallEOObject(const CallExpr *op);

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift);

vector<EOObject> PrecessRecordTypes(CompoundStmt *CS);

size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params);

EOObject GetMemberExprEOObject(const MemberExpr *opr);

EOObject GetEODeclRefExpr(const DeclRefExpr *op);

EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op, std::vector<uint64_t> *dims, size_t depth);
std::pair<size_t, EOObject> getMultiDimArrayTypeSize(const ArraySubscriptExpr *op, std::vector<uint64_t> *dims);

EOObject GetForStmtEOObject(const ForStmt *p_stmt);

EOObject GetSeqForBodyEOObject(const Stmt *p_stmt);

uint64_t GetTypeSize(QualType qual_type);

EOObject GetCastEOObject(const CastExpr *op);
extern UnitTranspiler transpiler;

EOObject GetFunctionBody(const clang::FunctionDecl *FD) {

  if (!FD->hasBody()) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  auto *const func_body = dyn_cast<CompoundStmt>(FD->getBody());
  if (func_body == nullptr) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  size_t shift = transpiler.glob_.RealMemorySize();
  size_t param_memory_size = GetParamMemorySize(FD->parameters());
  vector<Variable> all_param = ProcessFunctionParams(FD->parameters(), shift);
  vector<EOObject> all_types = PrecessRecordTypes(func_body);
  vector<Variable> all_local = ProcessFunctionLocalVariables(func_body, shift + param_memory_size);
  EOObject func_body_eo = EOObject(EOObjectType::EO_EMPTY);
  EOObject local_start("add", "local-start");
  local_start.nested.emplace_back("param-start");
  local_start.nested.emplace_back("param-size");
  func_body_eo.nested.push_back(local_start);
  size_t free_pointer = transpiler.glob_.RealMemorySize();
  EOObject local_empty_position("add", "empty-local-position");
  local_empty_position.nested.emplace_back("local-start");
  local_empty_position.nested.emplace_back(to_string(free_pointer - shift - param_memory_size),
                                           EOObjectType::EO_LITERAL);
  func_body_eo.nested.push_back(local_empty_position);
  for (const auto &param : all_param) {
    func_body_eo.nested.push_back(param.GetAddress(transpiler.glob_.name_));
  }
  for (const auto &var : all_types) {
    func_body_eo.nested.push_back(var);
  }
  for (const auto &var : all_local) {
    func_body_eo.nested.push_back(var.GetAddress(transpiler.glob_.name_));
  }
  EOObject goto_object{"goto", "@"};
  EOObject return_label{EOObjectType::EO_ABSTRACT};
  return_label.arguments.emplace_back("goto-return-label");
  EOObject body_seq = GetCompoundStmt(func_body, true);
  std::reverse(all_local.begin(), all_local.end());
  for (const auto &var : all_local) {
    if (var.is_initialized) {
      body_seq.nested.insert(body_seq.nested.begin(), var.GetInitializer());
    }
  }
  return_label.nested.push_back(body_seq);
  goto_object.nested.push_back(return_label);
  func_body_eo.nested.push_back(goto_object);
  transpiler.glob_.RemoveAllUsed(all_param);
  transpiler.glob_.RemoveAllUsed(all_local);

  return func_body_eo;
}

vector<EOObject> PrecessRecordTypes(CompoundStmt *const CS) {
  vector<EOObject> local_type_decls;
  for (auto *stmt : CS->body()) {
    Stmt::StmtClass stmt_class = stmt->getStmtClass();
    if (stmt_class == Stmt::DeclStmtClass) {
      auto *decl_stmt = dyn_cast<DeclStmt>(stmt);
      for (auto *decl : decl_stmt->decls()) {
        Decl::Kind decl_kind = decl->getKind();
        if (decl_kind == Decl::Kind::Record) {
          auto *record_decl = dyn_cast<RecordDecl>(decl);
          auto types = ProcessRecordType(record_decl, true);
          for (auto &type : types) {
            auto eo_objs = type.GetEORecordDecl();
            local_type_decls.insert(local_type_decls.end(), eo_objs.begin(), eo_objs.end());
          }
        }
      }
    }
  }
  return local_type_decls;
}

size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params) {
  size_t res = 0;
  for (auto *VD : params) {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / byte_size;
    res += type_size;
  }
  return res;
}

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params, size_t shift) {
  vector<Variable> all_params;
  for (auto *param : params) {
    all_params.push_back(ProcessVariable(param, "param-start", shift));
  }
  return all_params;
}

vector<Variable> ProcessFunctionLocalVariables(const clang::CompoundStmt *CS, size_t shift) {
  vector<Variable> all_local;
  for (auto *stmt : CS->body()) {
    Stmt::StmtClass stmt_class = stmt->getStmtClass();
    if (stmt_class == Stmt::DeclStmtClass) {
      auto *decl_stmt = dyn_cast<DeclStmt>(stmt);
      for (auto *decl : decl_stmt->decls()) {
        // TODO if var created in nested statement we don't found it. Fix
        // TODO duplicate names problem should be resolved
        Decl::Kind decl_kind = decl->getKind();
        if (decl_kind == Decl::Kind::Var) {
          auto *var_decl = dyn_cast<VarDecl>(decl);
          all_local.push_back(ProcessVariable(var_decl, "local-start", shift));
        }
      }
    }
  }
  return all_local;
}

// Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS, bool is_decorator = false) {
  EOObject res{"seq"};
  if (is_decorator) {
    res.postfix = "@";
  }
  for (auto *stmt : CS->body()) {
    Stmt::StmtClass stmt_class = stmt->getStmtClass();
    if (stmt_class == Stmt::ImplicitCastExprClass) {
      auto *ref = dyn_cast<Expr>(*stmt->child_begin());
      QualType qual_type = ref->getType();
      string type = GetTypeName(qual_type);
      string formatter = "?"; // todo
      if (type == "float32" || type == "float64") {
        formatter = "f";
      } else {
        formatter = "d";
      }
      EOObject printer{"printf"};
      printer.nested.emplace_back("\"%" + formatter + "\\n\"", EOObjectType::EO_LITERAL);
      EOObject read_val{"read"};
      read_val.nested.emplace_back(GetStmtEOObject(ref));
      if (!qual_type->isRecordType()) {
        read_val.name += "-as-" + type;
      } else {
        read_val.nested.emplace_back(to_string(
                                         transpiler.record_manager_.GetById(qual_type->getAsRecordDecl()->getID())->size),
                                     EOObjectType::EO_LITERAL);
      }
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
  if (stmt == nullptr) {
    llvm::errs() << "Warning: Try to construct EOObject for nullptr\n";
    return EOObject(EOObjectType::EO_PLUG);
  }
  Stmt::StmtClass stmt_class = stmt->getStmtClass();
  if (stmt_class == Stmt::BinaryOperatorClass) {
    const auto *op = dyn_cast<BinaryOperator>(stmt);
    return GetBinaryStmtEOObject(op);
  }
  if (stmt_class == Stmt::UnaryOperatorClass) {
    const auto *op = dyn_cast<UnaryOperator>(stmt);
    return GetUnaryStmtEOObject(op);
  }
  if (stmt_class == Stmt::CompoundAssignOperatorClass) {
    const auto *op = dyn_cast<CompoundAssignOperator>(stmt);
    return GetAssignmentOperationOperatorEOObject(op);
  }
  if (stmt_class == Stmt::ParenExprClass) {
    const auto *op = dyn_cast<ParenExpr>(stmt);
    return GetStmtEOObject(*op->child_begin());
  }
  if (stmt_class == Stmt::DeclRefExprClass) {
    const auto *ref = dyn_cast<DeclRefExpr>(stmt);
    return GetEODeclRefExpr(ref);
  }
  if (stmt_class == Stmt::IfStmtClass) {
    const auto *op = dyn_cast<IfStmt>(stmt);
    return GetIfStmtEOObject(op);
  }
  if (stmt_class == Stmt::WhileStmtClass) {
    const auto *op = dyn_cast<WhileStmt>(stmt);
    return GetWhileStmtEOObject(op);
  }
  if (stmt_class == Stmt::DoStmtClass) {
    const auto *op = dyn_cast<DoStmt>(stmt);
    return GetDoWhileStmtEOObject(op);
  }
  if (stmt_class == Stmt::CompoundStmtClass) {
    const auto *op = dyn_cast<CompoundStmt>(stmt);
    return GetCompoundStmt(op);
  }
  if (stmt_class == Stmt::IntegerLiteralClass) {
    const auto *op = dyn_cast<IntegerLiteral>(stmt);
    return GetIntegerLiteralEOObject(op);
  }
  if (stmt_class == Stmt::FloatingLiteralClass) {
    const auto *op = dyn_cast<FloatingLiteral>(stmt);
    return GetFloatingLiteralEOObject(op);
  }
  if (stmt_class == Stmt::DeclStmtClass) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  if (stmt_class == Stmt::CallExprClass) {
    const auto *op = dyn_cast<CallExpr>(stmt);
    return GetFunctionCallEOObject(op);
  }
  if (stmt_class == Stmt::ReturnStmtClass) {
    const auto *op = dyn_cast<ReturnStmt>(stmt);
    return GetReturnStmtEOObject(op);
  }
  if (stmt_class == Stmt::MemberExprClass) {
    const auto *op = dyn_cast<MemberExpr>(stmt);
    return GetMemberExprEOObject(op);
  }
  if (stmt_class == Stmt::MemberExprClass) {
    const auto *op = dyn_cast<MemberExpr>(stmt);
    return GetMemberExprEOObject(op);
  }
  if (stmt_class == Stmt::ArraySubscriptExprClass) {
    const auto *op = dyn_cast<ArraySubscriptExpr>(stmt);
    std::vector<uint64_t> dims;
    return GetArraySubscriptExprEOObject(op, &dims, 0);
  }
  if (stmt_class == Stmt::ForStmtClass) {
    const auto *op = dyn_cast<ForStmt>(stmt);
    return GetForStmtEOObject(op);
  }
  if (stmt_class >= clang::Stmt::firstCastExprConstant && stmt_class <= clang::Stmt::lastCastExprConstant) {
    const auto *op = dyn_cast<CastExpr>(stmt);
    return GetCastEOObject(op);
  }
  llvm::errs() << "Warning: Unknown statement " << stmt->getStmtClassName() << "\n";

  return EOObject(EOObjectType::EO_PLUG);
}
EOObject GetCastEOObject(const CastExpr *op) {
  auto cast_kind = op->getCastKind();
  if (cast_kind == clang::CK_LValueToRValue) {
    QualType qual_type = op->getType();
    string type = GetTypeName(qual_type);
    EOObject read{"read"};
    read.nested.push_back(GetStmtEOObject(*op->child_begin()));
    if (!qual_type->isRecordType()) {
      read.name += "-as-" + type;
    } else {
      read.nested.emplace_back(to_string(
                                   transpiler.record_manager_.GetById(qual_type->getAsRecordDecl()->getID())->size),
                               EOObjectType::EO_LITERAL);
    }
    return read;
  }
  if (cast_kind == clang::CK_FloatingToIntegral || cast_kind == clang::CK_IntegralToFloating) {
    QualType qual_type = op->getType();
    string type = GetTypeName(qual_type);
    EOObject cast{"as-" + type};
    cast.nested.push_back(GetStmtEOObject(*op->child_begin()));
    return cast;
  }
  // TODO if cast kinds and also split it to another func
  return GetStmtEOObject(*op->child_begin());
}
EOObject GetForStmtEOObject(const ForStmt *p_stmt) {
  EOObject for_stmt(EOObjectType::EO_EMPTY);

  const auto *init = p_stmt->getInit();
  const auto *cond = p_stmt->getCond();
  const auto *inc = p_stmt->getInc();
  const auto *body = p_stmt->getBody();
  if (init != nullptr) {
    for_stmt.nested.push_back(GetStmtEOObject(p_stmt->getInit()));
  }
  EOObject while_stmt{"while"};
  if (cond != nullptr) {
    while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  } else {
    while_stmt.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  }
  EOObject seq{"seq"};
  seq.nested.push_back(GetSeqForBodyEOObject(p_stmt->getBody()));
  if (inc != nullptr) {
    seq.nested.push_back(GetSeqForBodyEOObject(p_stmt->getInc()));
  }
  while_stmt.nested.push_back(seq);
  for_stmt.nested.push_back(while_stmt);
  return for_stmt;
}

EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op,
                                       std::vector<uint64_t> *dims, size_t depth) {
  std::vector<uint64_t> tmp_dims;
  auto decl_info = getMultiDimArrayTypeSize(op, &tmp_dims);
  if (tmp_dims.size() > dims->size()) {
    dims = &tmp_dims;
  }
  uint64_t dim_size = decl_info.first; // current dimension size.
  for (int i = 0; i < depth && i < dims->size(); ++i) { // NOLINT(altera-id-dependent-backward-branch)
    dim_size *= dims->at(i);
  }

  for (const auto *base_ch : op->getBase()->children()) {
    auto index_name = GetStmtEOObject(op->getIdx());

    EOObject curr_shift{"mul"};
    EOObject type_size_obj{std::to_string(dim_size), EOObjectType::EO_LITERAL};
    curr_shift.nested.emplace_back(index_name);
    curr_shift.nested.emplace_back(type_size_obj);

    auto stmt_class = base_ch->getStmtClass();
    if (stmt_class == Stmt::ArraySubscriptExprClass) {
      EOObject add_shift{"add"};

      const auto *arr_sub_expr = dyn_cast<ArraySubscriptExpr>(base_ch);
      EOObject next_shift = GetArraySubscriptExprEOObject(arr_sub_expr, dims, depth + 1);

      add_shift.nested.emplace_back(curr_shift);
      add_shift.nested.emplace_back(next_shift);

      if (depth == 0) {
        EOObject final_write{"add"};
        final_write.nested.emplace_back(decl_info.second);
        final_write.nested.emplace_back(add_shift);
        return final_write;
      }
      return add_shift;
    }
    if (stmt_class == Stmt::DeclRefExprClass || stmt_class == Stmt::MemberExprClass) {
      if (depth == 0) {
        EOObject final_write{"add"};
        final_write.nested.emplace_back(decl_info.second);
        final_write.nested.emplace_back(curr_shift);
        return final_write;
      }
      return curr_shift;
    }
    return curr_shift;
  }
  return EOObject{EOObjectType::EO_PLUG};
}

std::pair<uint64_t, EOObject> getMultiDimArrayTypeSize(const ArraySubscriptExpr *op, std::vector<uint64_t> *dims) {
  for (const auto *base_ch : op->getBase()->children()) {
    auto stmt_class = base_ch->getStmtClass();
    if (stmt_class == Stmt::DeclRefExprClass) {
      const auto *decl_ref_expr = dyn_cast<DeclRefExpr>(base_ch);
      auto qt = decl_ref_expr->getType();
      EOObject arr_name = GetStmtEOObject(op->getBase());
      size_t sz = decl_ref_expr->getDecl()->getASTContext().getTypeInfo(qt).Align / byte_size;
      return std::make_pair(sz, arr_name);
    }
    if (stmt_class == Stmt::ArraySubscriptExprClass) {
      const auto *arr_sub_expr = dyn_cast<ArraySubscriptExpr>(base_ch);
      auto qt = arr_sub_expr->getType();
      if (qt->isArrayType()) {
        const auto *arr = qt->getAsArrayTypeUnsafe();
        if (arr->isConstantArrayType()) {
          const auto *const_arr = dyn_cast<clang::ConstantArrayType>(qt);
          dims->emplace_back(const_arr->getSize().getLimitedValue());
        }
      }
      return getMultiDimArrayTypeSize(arr_sub_expr, dims);
    }
    if (stmt_class == Stmt::MemberExprClass) {
      const auto *memb_expr = dyn_cast<MemberExpr>(base_ch);
      const auto *child = dyn_cast<Expr>(*memb_expr->child_begin());
      QualType qual_type = child->getType();
      EOObject arr_name = GetStmtEOObject(op->getBase());
      size_t sz = transpiler.record_manager_.GetById(qual_type->getAsRecordDecl()->getID())->size;
      return std::make_pair(sz, arr_name);
    }
    cerr << base_ch->getStmtClassName() << "\n\n";
  }
  return std::make_pair(0, EOObject{"plug", EOObjectType::EO_PLUG});
}

EOObject GetMemberExprEOObject(const MemberExpr *op) {
  EOObject member{"add"};
  const auto *child = dyn_cast<Expr>(*op->child_begin());
  QualType qual_type = child->getType();
  if (qual_type->isPointerType()) {
    EOObject record{"address"};
    qual_type = dyn_cast<clang::PointerType>(qual_type.getCanonicalType())->getPointeeType();
    record.nested.emplace_back("global-ram");
    record.nested.push_back(GetStmtEOObject(child));
    member.nested.push_back(record);
  } else {
    member.nested.push_back(GetStmtEOObject(child));
  }
  member.nested.push_back(transpiler.record_manager_.GetShiftAlias(qual_type->getAsRecordDecl()->getID(),
                                                                   op->getMemberDecl()->getNameAsString()));
  return member;
}

EOObject GetFunctionCallEOObject(const CallExpr *op) {
  EOObject call("seq");
  vector<std::size_t> var_sizes;
  for (auto *VD : op->getDirectCallee()->parameters()) {
    TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
    size_t type_size = type_info.Width / byte_size;
    var_sizes.push_back(type_size);
  }
  size_t shift = 0;
  int i = 0;
  for (const auto *arg : op->arguments()) {
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
    // maybe it will work with param.
    i = i == var_sizes.size() - 1 ? i : i + 1;
    call.nested.push_back(param);
  }
  call.nested.push_back(transpiler.func_manager_.GetFunctionCall(op->getDirectCallee(), shift));
  QualType qual_type = op->getType();
  std::string postfix = GetTypeName(qual_type);
  if (postfix != "undefinedtype") {
    EOObject read_ret{"read"};
    EOObject ret_val{"return"};
    read_ret.nested.push_back(ret_val);
    if (qual_type->isRecordType() || qual_type->isArrayType()) {
      read_ret.nested.emplace_back(
          to_string(var_sizes[0]),
          EOObjectType::EO_LITERAL
      );
    } else {
      read_ret.name += "-as-" + postfix;
    }
    call.nested.push_back(read_ret);
  } else {
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
  if (is_signed) {
    int64_t val = an_int.getSExtValue();
    std::string str_val{std::to_string(val)};
    return EOObject{str_val, EOObjectType::EO_LITERAL};
  }
  uint64_t val = an_int.getZExtValue();
  std::string str_val{std::to_string(val)};
  return EOObject{str_val, EOObjectType::EO_LITERAL};
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

  EOObject binary_op{operation};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  EOObject eo_object{"read"};
  QualType qual_type = left->getType();
  eo_object.nested.push_back(GetStmtEOObject(left));
  if (!qual_type->isRecordType()) {
    eo_object.name += "-as-" + GetTypeName(qual_type);
  } else {
    eo_object.nested.emplace_back(to_string(
                                      transpiler.record_manager_.GetById(qual_type->getAsRecordDecl()->getID())->size),
                                  EOObjectType::EO_LITERAL);
  }
  binary_op.nested.emplace_back(eo_object);

  binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  auto op_code = p_operator->getOpcode();
  std::string operation;
  if (op_code == BinaryOperatorKind::BO_Assign) {
    return GetAssignmentOperatorEOObject(p_operator);
  }
  if (op_code == BinaryOperatorKind::BO_Add) {
    operation = "add";
  } else if (op_code == BinaryOperatorKind::BO_Sub) {
    operation = "sub";
  } else if (op_code == BinaryOperatorKind::BO_Mul) {
    operation = "mul";
  } else if (op_code == BinaryOperatorKind::BO_Div) {
    operation = "div";
  } else if (op_code == BinaryOperatorKind::BO_Rem) {
    operation = "mod";
  } else if (op_code == BinaryOperatorKind::BO_And) {
    operation = "bit-and";
  } else if (op_code == BinaryOperatorKind::BO_Or) {
    operation = "bit-or";
  } else if (op_code == BinaryOperatorKind::BO_Xor) {
    operation = "bit-xor";
  } else if (op_code == BinaryOperatorKind::BO_Shl) {
    operation = "shift-left";
  } else if (op_code == BinaryOperatorKind::BO_Shr) {
    operation = "shift-right";
  } else if (op_code == BinaryOperatorKind::BO_EQ) {
    operation = "eq";
  } else if (op_code == BinaryOperatorKind::BO_NE) {
    operation = "neq";
  } else if (op_code == BinaryOperatorKind::BO_LT) {
    operation = "less";
  } else if (op_code == BinaryOperatorKind::BO_LE) {
    operation = "leq";
  } else if (op_code == BinaryOperatorKind::BO_GT) {
    operation = "greater";
  } else if (op_code == BinaryOperatorKind::BO_GE) {
    operation = "geq";
  } else {
    operation = "undefined";
    llvm::errs() << "Warning: Unknown operator " << p_operator->getOpcodeStr() << "\n";
  }

  EOObject binary_op{operation};
  binary_op.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetUnaryStmtEOObject(const UnaryOperator *p_operator) {
  auto op_code = p_operator->getOpcode();
  std::string operation;
  Stmt *stmt = nullptr;

//   QualType argType = p_operator->getType();
//   uint64_t typeSize = GetTypeSize(argType);

  // [C99 6.5.2.4] Postfix increment and decrement
  if (op_code == UnaryOperatorKind::UO_PostInc) { // UNARY_OPERATION(PostInc, "++")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      QualType arg_type = p_operator->getType();
      uint64_t type_size = GetTypeSize(arg_type);
      EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
  }
  if (op_code == UnaryOperatorKind::UO_PostDec) { // UNARY_OPERATION(PostDec, "--")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      QualType arg_type = p_operator->getType();
      uint64_t type_size = GetTypeSize(arg_type);
      EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
    // [C99 6.5.3.1] Prefix increment and decrement
  }
  if (op_code == UnaryOperatorKind::UO_PreInc) { // UNARY_OPERATION(PreInc, "++")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      QualType arg_type = p_operator->getType();
      uint64_t type_size = GetTypeSize(arg_type);
      EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
  }
  if (op_code == UnaryOperatorKind::UO_PreDec) { // UNARY_OPERATION(PreDec, "--")
    std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      QualType arg_type = p_operator->getType();
      uint64_t type_size = GetTypeSize(arg_type);
      EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
    // [C99 6.5.3.2] Address and indirection
  }
  if (op_code == UnaryOperatorKind::UO_AddrOf) { // UNARY_OPERATION(AddrOf, "&")
    EOObject variable{"addr-of"};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  }
  if (op_code == UnaryOperatorKind::UO_Deref) { // UNARY_OPERATION(Deref, "*")
    EOObject variable{"address"};
    EOObject ram{"global-ram"};
    variable.nested.push_back(ram);
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // [C99 6.5.3.3] Unary arithmetic
  }
  if (op_code == UnaryOperatorKind::UO_Plus) { // UNARY_OPERATION(Plus, "+")
    operation = "plus";
  } else if (op_code == UnaryOperatorKind::UO_Minus) { // UNARY_OPERATION(Minus, "-")
    operation = "neg";
  } else if (op_code == UnaryOperatorKind::UO_Not) { // UNARY_OPERATION(Not, "~")
    operation = "bit-not";
  } else if (op_code == UnaryOperatorKind::UO_LNot) { // UNARY_OPERATION(LNot, "!")
    operation = "not";
    // "__real expr"/"__imag expr" Extension.
  } else if (op_code == UnaryOperatorKind::UO_Real) { // UNARY_OPERATION(Real, "__real")
    operation = "real";
  } else if (op_code == UnaryOperatorKind::UO_Imag) { // UNARY_OPERATION(Imag, "__imag")
    operation = "imag";
    // __extension__ marker.
  } else if (op_code == UnaryOperatorKind::UO_Extension) { // UNARY_OPERATION(Extension, "__extension__")
    operation = "extension";
    // [C++ Coroutines] co_await operator
  } else if (op_code == UnaryOperatorKind::UO_Coawait) { // UNARY_OPERATION(Coawait, "co_await")
    operation = "coawait";
    // Incorrect unary operator
  } else {
    operation = "undefined";
    llvm::errs() << "Warning: Unknown operator " << UnaryOperator::getOpcodeStr(p_operator->getOpcode()) << "\n";
  }

  EOObject unary_op{operation};
  unary_op.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
  return unary_op;
}

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator) {
  EOObject binary_op{"write"};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  QualType qual_type = left->getType();
  if (!qual_type->isRecordType()) {
    binary_op.name += "-as-" + GetTypeName(left->getType());
  }
  binary_op.nested.emplace_back(GetStmtEOObject(left));
  binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetEODeclRefExpr(const DeclRefExpr *op) {
  if (op == nullptr) {
    return EOObject{EOObjectType::EO_EMPTY};
  }
  try {
    const Variable &var = transpiler.glob_.GetVarById(dyn_cast<VarDecl>(op->getFoundDecl()));
    return EOObject{var.alias};
  } catch (invalid_argument &) {
    return EOObject{EOObjectType::EO_LITERAL};
  }
}

EOObject GetAssignmentOperationOperatorEOObject(const CompoundAssignOperator *p_operator) {
  EOObject binary_op{"write"};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  QualType qual_type = left->getType();
  if (!qual_type->isRecordType()) {
    binary_op.name += "-as-" + GetTypeName(left->getType());
  }
  binary_op.nested.emplace_back(GetStmtEOObject(left));
  binary_op.nested.push_back(GetCompoundAssignEOObject(p_operator));
  return binary_op;
}

EOObject GetReturnStmtEOObject(const ReturnStmt *p_stmt) {
  EOObject result{EOObjectType::EO_EMPTY};
  // TODO: Should make write-as-...
  const auto *ret_value = p_stmt->getRetValue();
  if (ret_value != nullptr) {
    EOObject ret{"write"};
    EOObject address{"return"};
    ret.nested.push_back(address);
    ret.nested.push_back(GetStmtEOObject(ret_value));
    result.nested.push_back(ret);
  }
  EOObject label{"goto-return-label.forward TRUE", EOObjectType::EO_LITERAL};
  result.nested.push_back(label);
  return result;
}

EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  if (p_stmt->hasElseStorage()) {
    EOObject if_else_stmt{"if-else"};
    if_else_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
    if_else_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getThen()));
    if_else_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getElse()));
    return if_else_stmt;
  }

  EOObject if_stmt{"if"};
  if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getThen()));
  return if_stmt;

}

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt) {
  EOObject while_stmt{"while"};
  while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  while_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getBody()));
  return while_stmt;
}

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt) {
  EOObject do_while_stmt{"do-while"};
  do_while_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  do_while_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getBody()));
  return do_while_stmt;
}

EOObject GetSeqForBodyEOObject(const Stmt *p_stmt) {
  if (p_stmt->getStmtClass() == clang::Stmt::CompoundStmtClass) {
    return GetStmtEOObject(p_stmt);
  }
  EOObject seq("seq");
  seq.nested.push_back(GetStmtEOObject(p_stmt));
  seq.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return seq;
}

uint64_t GetTypeSize(QualType qual_type) {
  extern ASTContext *context;
  const clang::Type *type_ptr = qual_type.getTypePtr();
  TypeInfo type_info = context->getTypeInfo(type_ptr);
  uint64_t type_size = type_info.Width;

  if (type_ptr->isPointerType()) {
    const clang::Type *pointee_type = type_ptr->getPointeeType().getTypePtr();
    TypeInfo pointee_type_info = context->getTypeInfo(pointee_type);
    uint64_t pointee_type_size = pointee_type_info.Width;
    return pointee_type_size / byte_size;
  }

  return type_size / byte_size;
}

std::string GetTypeName(QualType qual_type) {
  extern ASTContext *context; // NOLINT(readability-redundant-declaration)
  const clang::Type *type_ptr = qual_type.getTypePtr();
  TypeInfo type_info = context->getTypeInfo(type_ptr);
  uint64_t type_size = type_info.Width;
  std::string str;

  if (type_ptr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (type_ptr->isPointerType()) {
    // str += "int64";
    str += "ptr";
    return str;
  }

  if (type_ptr->isFloatingType()) {
    str += "float" + std::to_string(type_size);
    return str;
  }

  if (!type_ptr->isSignedIntegerType()) {
    str += "u";
  }
  if (type_ptr->isCharType()) {
    str += "char";
    return str;
  }
  if (type_ptr->isIntegerType()) {
    str += "int" + std::to_string(type_size);
    return str;
  }

  if (type_ptr->isUnionType()) {
    str = "un-";
  }
  if (type_ptr->isStructureType()) {
    str = "st-";
  }
  if (type_ptr->isUnionType() || type_ptr->isStructureType()) {
    RecordDecl *RD = type_ptr->getAsRecordDecl();
    if (RD->hasNameForLinkage()) {
      str += RD->getNameAsString();
    } else {
      str += std::to_string(reinterpret_cast<uint64_t>(RD)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    }
    return str;
  }

  return "undefinedtype";
}

std::set<std::string> FindAllExternalObjects(const EOObject &obj) {
  std::set<std::string> all_known = {obj.postfix};
  std::set<std::string> unknown{};
  // TODO maybe should use pointers or copy constructor to avoid unnecessary copying of objects_
  std::queue<EOObject> not_visited;
  for (auto child : obj.nested) {
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty()) {
    EOObject cur = not_visited.front();
    not_visited.pop();
    switch (cur.type) {
      case EOObjectType::EO_ABSTRACT:all_known.insert(cur.postfix);
        for (const auto &arg : cur.arguments) {
          all_known.insert(arg);
        }
        break;
      case EOObjectType::EO_COMPLETE:all_known.insert(cur.postfix);
        if (all_known.find(cur.name) == all_known.end()) {
          unknown.insert(cur.name);
        }
        break;
      case EOObjectType::EO_EMPTY:
      case EOObjectType::EO_LITERAL:break;
      case EOObjectType::EO_PLUG:
        if (cur.nested.empty()) {
          unknown.insert("plug");
        }
        break;
    }
    for (auto child : cur.nested) {
      not_visited.push(std::move(child));
    }
  }
  for (const auto &known_obj : all_known) {
    unknown.erase(known_obj);
  }

  return unknown;
}


