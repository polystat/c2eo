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

#include "src/transpiler/transpile_helper.h"

#include <map>
#include <queue>
#include <regex>
#include <sstream>
#include <utility>
#include <vector>

#include "src/transpiler/enumdecl.h"
#include "src/transpiler/memory_manager.h"
#include "src/transpiler/process_variables.h"
#include "src/transpiler/recorddecl.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/vardecl.h"

using clang::ArrayRef;
using clang::ArraySubscriptExpr;
using clang::ASTContext;
using clang::BinaryOperator;
using clang::BinaryOperatorKind;
using clang::CallExpr;
using clang::CaseStmt;
using clang::CastExpr;
using clang::CompoundAssignOperator;
using clang::CompoundStmt;
using clang::ConstantExpr;
using clang::Decl;
using clang::DeclRefExpr;
using clang::DeclStmt;
using clang::DefaultStmt;
using clang::DoStmt;
using clang::EnumConstantDecl;
using clang::Expr;
using clang::FloatingLiteral;
using clang::ForStmt;
using clang::IfStmt;
using clang::IntegerLiteral;
using clang::MemberExpr;
using clang::ParenExpr;
using clang::ParmVarDecl;
using clang::QualType;
using clang::RecordDecl;
using clang::ReturnStmt;
using clang::Stmt;
using clang::SwitchStmt;
using clang::TypeInfo;
using clang::UnaryOperator;
using clang::UnaryOperatorKind;
using clang::VarDecl;
using clang::WhileStmt;
using llvm::dyn_cast;
using std::string;
using std::to_string;
using std::vector;

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator);

EOObject GetAssignmentOperationOperatorEOObject(
    const CompoundAssignOperator *p_operator);

EOObject GetUnaryStmtEOObject(const UnaryOperator *p_operator);

EOObject GetIfElseStmtEOObject(const IfStmt *p_stmt);

EOObject GetIfStmtEOObject(const IfStmt *p_stmt);

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt);

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt);

EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal);

EOObject GetReturnStmtEOObject(const ReturnStmt *p_stmt);

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator);

EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator);

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal);

EOObject GetFunctionCallEOObject(const CallExpr *op);

EOObject GetPrintfCallEOObject(const CallExpr *op);

EOObject GetInitListEOObject(const clang::InitListExpr *list);

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params,
                                       size_t shift);

vector<EOObject> PrecessRecordTypes(CompoundStmt *CS);

size_t GetParamMemorySize(ArrayRef<ParmVarDecl *> params);

EOObject GetMemberExprEOObject(const MemberExpr *opr);

EOObject GetEODeclRefExpr(const DeclRefExpr *op);

EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op,
                                       std::vector<uint64_t> *dims,
                                       size_t depth);

std::pair<uint64_t, EOObject> getMultiDimArrayTypeSize(
    const ArraySubscriptExpr *op, std::vector<uint64_t> *dims);

EOObject GetForStmtEOObject(const ForStmt *p_stmt);

EOObject GetSeqForBodyEOObject(const Stmt *p_stmt);

uint64_t GetTypeSize(QualType qual_type);

EOObject GetCastEOObject(const CastExpr *op);

EOObject GetSwitchEOObject(const SwitchStmt *p_stmt);

EOObject GetCaseCondEOObject(const vector<const Expr *> &all_cases,
                             const EOObject &switch_exp, size_t i);

EOObject GetCharacterLiteralEOObject(const clang::CharacterLiteral *p_literal);
void AppendDeclStmt(const DeclStmt *stmt);

extern UnitTranspiler transpiler;
extern ASTContext *context;

std::string Escaped(const std::string &input) {
  std::string output;
  output.reserve(input.size());
  for (const char c : input) {
    switch (c) {
      case '\a':
        output += "\\a";
        break;
      case '\b':
        output += "\\b";
        break;
      case '\f':
        output += "\\f";
        break;
      case '\n':
        output += "\\n";
        break;
      case '\r':
        output += "\\r";
        break;
      case '\t':
        output += "\\t";
        break;
      case '\v':
        output += "\\v";
        break;
      case '\\':
        output += "\\\\";
        break;
      case '\'':
        output += "\\\'";
        break;
      case '\"':
        output += "\\\"";
        break;
      case '\0':
        output += "\\0";
        break;
      default:
        output += c;
        break;
    }
  }
  return output;
}

EOObject GetFunctionBody(const clang::FunctionDecl *FD) {
  if (!FD->hasBody()) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  auto *const func_body = dyn_cast<CompoundStmt>(FD->getBody());
  if (func_body == nullptr) {
    return EOObject(EOObjectType::EO_EMPTY);
  }
  size_t shift = transpiler.glob_.GetFreeSpacePointer();
  size_t param_memory_size = GetParamMemorySize(FD->parameters());
  vector<Variable> all_param = ProcessFunctionParams(FD->parameters(), shift);
  vector<EOObject> all_types = PrecessRecordTypes(func_body);
  vector<Variable> all_local;
  ProcessFunctionLocalVariables(func_body, all_local,
                                shift + param_memory_size);
  EOObject func_body_eo = EOObject(EOObjectType::EO_EMPTY);
  EOObject local_start("plus", "local-start");
  local_start.nested.emplace_back("param-start");
  local_start.nested.emplace_back("param-size");
  func_body_eo.nested.push_back(local_start);
  size_t free_pointer = transpiler.glob_.GetFreeSpacePointer();
  EOObject local_empty_position("plus", "empty-local-position");
  local_empty_position.nested.emplace_back("local-start");
  local_empty_position.nested.emplace_back(
      to_string(free_pointer - shift - param_memory_size),
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
      if (decl_stmt != nullptr) {
        for (auto *decl : decl_stmt->decls()) {
          Decl::Kind decl_kind = decl->getKind();
          if (decl_kind == Decl::Kind::Record) {
            auto *record_decl = dyn_cast<RecordDecl>(decl);
            auto types = ProcessRecordType(record_decl, true);
            for (auto &type : types) {
              auto eo_objs = type.GetEORecordDecl();
              local_type_decls.insert(local_type_decls.end(), eo_objs.begin(),
                                      eo_objs.end());
            }
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

vector<Variable> ProcessFunctionParams(ArrayRef<ParmVarDecl *> params,
                                       size_t shift) {
  vector<Variable> all_params;
  for (auto *param : params) {
    all_params.push_back(ProcessVariable(param, "param-start", shift));
  }
  return all_params;
}

// Function to get eo representation of CompoundStmt
EOObject GetCompoundStmt(const clang::CompoundStmt *CS,
                         bool is_decorator = false) {
  EOObject res{"seq"};
  if (is_decorator) {
    res.postfix = "@";
  }
  vector<Variable> all_local_in_block;
  ProcessCompoundStatementLocalVariables(CS, all_local_in_block);
  auto pos_it = res.nested.begin();
  for (const auto &var : all_local_in_block) {
    if (var.is_initialized) {
      pos_it = res.nested.insert(pos_it, var.GetInitializer());
      pos_it++;
    }
  }
  if (CS != nullptr) {
    for (auto *stmt : CS->body()) {
      EOObject stmt_obj = GetStmtEOObject(stmt);
      res.nested.push_back(stmt_obj);
    }
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
    if (op != nullptr) {
      return GetStmtEOObject(*op->child_begin());
    }
  }
  if (stmt_class == Stmt::DeclRefExprClass) {
    const auto *ref = dyn_cast<DeclRefExpr>(stmt);
    return GetEODeclRefExpr(ref);
  }
  if (stmt_class == Stmt::IfStmtClass) {
    const auto *op = dyn_cast<IfStmt>(stmt);
    if (op != nullptr) {
      if (op->hasElseStorage()) {
        return GetIfElseStmtEOObject(op);
      }
    }
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
  if (stmt_class == Stmt::CharacterLiteralClass) {
    const auto *op = dyn_cast<clang::CharacterLiteral>(stmt);
    return GetCharacterLiteralEOObject(op);
  }
  if (stmt_class == Stmt::FloatingLiteralClass) {
    const auto *op = dyn_cast<FloatingLiteral>(stmt);
    return GetFloatingLiteralEOObject(op);
  }
  if (stmt_class == Stmt::DeclStmtClass) {
    const auto *op = dyn_cast<DeclStmt>(stmt);
    AppendDeclStmt(op);
    return EOObject(EOObjectType::EO_EMPTY);
  }
  if (stmt_class == Stmt::CallExprClass) {
    const auto *op = dyn_cast<CallExpr>(stmt);
    if (op->getDirectCallee() != nullptr &&
        op->getDirectCallee()->getNameInfo().getAsString() == "printf") {
      return GetPrintfCallEOObject(op);
    }
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
  if (stmt_class == Stmt::ArraySubscriptExprClass) {
    const auto *op = dyn_cast<ArraySubscriptExpr>(stmt);
    std::vector<uint64_t> dims;
    return GetArraySubscriptExprEOObject(op, &dims, 0);
  }
  if (stmt_class == Stmt::ForStmtClass) {
    const auto *op = dyn_cast<ForStmt>(stmt);
    return GetForStmtEOObject(op);
  }
  if (stmt_class >= clang::Stmt::firstCastExprConstant &&
      stmt_class <= clang::Stmt::lastCastExprConstant) {
    const auto *op = dyn_cast<CastExpr>(stmt);
    return GetCastEOObject(op);
  }
  if (stmt_class == Stmt::BreakStmtClass) {
    return EOObject{"break"};
  }
  if (stmt_class == Stmt::ContinueStmtClass) {
    return EOObject{"continue"};
  }
  if (stmt_class == Stmt::SwitchStmtClass) {
    const auto *op = dyn_cast<SwitchStmt>(stmt);
    return GetSwitchEOObject(op);
  }
  if (stmt_class == Stmt::ConstantExprClass) {
    const auto *op = dyn_cast<ConstantExpr>(stmt);
    return GetStmtEOObject(op->getSubExpr());
  }
  if (stmt_class == Stmt::NullStmtClass) {
    // The empty statement
    return EOObject(EOObjectType::EO_EMPTY);
  }
  if (stmt_class == Stmt::InitListExprClass) {
    const auto *op = dyn_cast<clang::InitListExpr>(stmt);
    return GetInitListEOObject(op);
  }
  if (stmt_class == Stmt::StringLiteralClass) {
    const auto *op = dyn_cast<clang::StringLiteral>(stmt);
    std::string value = Escaped(op->getString().str());
    // TODO(nchuykin) remove lines below after fixing printf EOObject
    value = std::regex_replace(value, std::regex("%[lh]*[ud]"), "%d");
    value = std::regex_replace(value, std::regex("%[lh]*f"), "%f");
    return {"\"" + value + "\"", EOObjectType::EO_LITERAL};
  }
  llvm::errs() << "Warning: Unknown statement " << stmt->getStmtClassName()
               << "\n";

  return EOObject(EOObjectType::EO_PLUG);
}
EOObject GetCharacterLiteralEOObject(const clang::CharacterLiteral *p_literal) {
  if (p_literal != nullptr) {
    unsigned int an_int = p_literal->getValue();
    std::string str_val{std::to_string(an_int)};
    return EOObject{str_val, EOObjectType::EO_LITERAL};
  }
  return EOObject{EOObjectType::EO_PLUG};
}
EOObject GetInitListEOObject(const clang::InitListExpr *list) {
  EOObject eoList{"*", EOObjectType::EO_EMPTY};
  clang::QualType qualType = list->getType().getDesugaredType(*context);
  std::vector<EOObject> inits;
  std::string elementTypeName;
  std::map<std::string, std::pair<clang::QualType, size_t>>::iterator
      recElement;
  size_t elementSize = 0;
  if (qualType->isArrayType()) {
    clang::QualType elementQualType =
        llvm::dyn_cast<clang::ConstantArrayType>(qualType)->getElementType();
    elementSize = 1;
    while (elementQualType->isArrayType()) {
      const auto *cat =
          llvm::dyn_cast<clang::ConstantArrayType>(elementQualType);
      elementSize *= cat->getSize().getLimitedValue();
      elementQualType =
          llvm::dyn_cast<clang::ConstantArrayType>(elementQualType)
              ->getElementType();
    }
    elementTypeName = GetTypeName(elementQualType);
    elementSize *= context->getTypeInfo(elementQualType).Align / byte_size;
  } else if (qualType->isRecordType()) {
    auto *recordType = transpiler.record_manager_.GetById(
        qualType->getAsRecordDecl()->getID());
    recElement = recordType->fields.begin();
  }
  int i = 0;
  for (auto element = list->child_begin(); element != list->child_end();
       element++, i++) {
    EOObject shiftedAlias{"plus"};
    shiftedAlias.nested.emplace_back("list-init-name",
                                     EOObjectType::EO_TEMPLATE);
    if (qualType->isArrayType()) {
      EOObject newShift{"times"};
      newShift.nested.emplace_back(to_string(i), EOObjectType::EO_LITERAL);
      newShift.nested.emplace_back(to_string(elementSize),
                                   EOObjectType::EO_LITERAL);
      shiftedAlias.nested.push_back(newShift);
    } else if (qualType->isRecordType()) {
      shiftedAlias.nested.emplace_back(transpiler.record_manager_.GetShiftAlias(
          qualType->getAsRecordDecl()->getID(), recElement->first));
      elementTypeName = GetTypeName(recElement->second.first);
    }
    EOObject value = GetStmtEOObject(*element);
    if (value.type == EOObjectType::EO_EMPTY && value.name == "*") {
      EOObject newValue = ReplaceEmpty(value, shiftedAlias);
      eoList.nested.insert(eoList.nested.end(), newValue.nested.begin(),
                           newValue.nested.end());
    } else {
      EOObject res("write");
      if (!elementTypeName.empty()) {
        res.name += "-as-" + elementTypeName;
      }
      res.nested.emplace_back(shiftedAlias);
      res.nested.emplace_back(value);
      eoList.nested.push_back(res);
      if (qualType->isRecordType()) {
        recElement++;
      }
    }
  }
  return eoList;
}

EOObject ReplaceEmpty(const EOObject &eoObject, const EOObject &alias) {
  EOObject res;
  res.postfix = eoObject.postfix;
  res.arguments = eoObject.arguments;
  res.prefix = eoObject.prefix;
  if (eoObject.type == EOObjectType::EO_TEMPLATE &&
      eoObject.name == "list-init-name") {
    res = alias;
  } else {
    res.name = eoObject.name;
    res.type = eoObject.type;
  }
  for (const auto &nest : eoObject.nested) {
    res.nested.push_back(ReplaceEmpty(nest, alias));
  }
  return res;
}

EOObject GetSwitchEOObject(const SwitchStmt *p_stmt) {
  EOObject goto_object{"goto"};
  EOObject return_label{EOObjectType::EO_ABSTRACT};
  return_label.arguments.emplace_back("end");

  return_label.nested.emplace_back("end.forward TRUE", "break");
  return_label.nested.emplace_back("memory", "flag");

  EOObject seq_object{"seq", "@"};
  EOObject init_flag_object{"write"};
  init_flag_object.nested.emplace_back("flag");
  init_flag_object.nested.emplace_back("0", EOObjectType::EO_LITERAL);
  EOObject set_flag_object{"write"};
  set_flag_object.nested.emplace_back("flag");
  set_flag_object.nested.emplace_back("1", EOObjectType::EO_LITERAL);
  seq_object.nested.push_back(init_flag_object);
  const auto *switch_init = p_stmt->getInit();
  if (switch_init != nullptr) {
    seq_object.nested.push_back(GetStmtEOObject(switch_init));
  }

  EOObject switch_expr_object = GetStmtEOObject(p_stmt->getCond());

  // TODO(nkchuykin) if get body return null...
  auto end = p_stmt->getBody()->child_end();
  for (auto stmt = p_stmt->getBody()->child_begin(); stmt != end; ++stmt) {
    if ((*stmt)->getStmtClass() == Stmt::CaseStmtClass) {
      const auto *case_stmt = dyn_cast<CaseStmt>(*stmt);
      EOObject if_obj{"if"};
      vector<const Expr *> all_cases{case_stmt->getLHS()};
      const auto *nested = case_stmt->getSubStmt();
      while (nested != nullptr &&
             nested->getStmtClass() == Stmt::CaseStmtClass) {
        const auto *nested_case = dyn_cast<CaseStmt>(nested);
        all_cases.push_back(nested_case->getLHS());
        nested = nested_case->getSubStmt();
      }
      EOObject cond_obj{"or"};
      EOObject eq_obj{"eq"};
      cond_obj.nested.emplace_back("flag");
      eq_obj.nested.push_back(switch_expr_object);
      eq_obj.nested.push_back(GetStmtEOObject(case_stmt->getLHS()));
      cond_obj.nested.push_back(
          GetCaseCondEOObject(all_cases, switch_expr_object, 0));
      if_obj.nested.push_back(cond_obj);
      EOObject buffer_obj{"seq"};
      if (nested != nullptr) {
        buffer_obj.nested.push_back(GetStmtEOObject(nested));
      }
      auto tmp = stmt;
      tmp++;
      while (tmp != end && (*tmp)->getStmtClass() != Stmt::CaseStmtClass &&
             (*tmp)->getStmtClass() != Stmt::DefaultStmtClass) {
        buffer_obj.nested.push_back(GetStmtEOObject(*tmp));
        tmp++;
      }
      buffer_obj.nested.push_back(set_flag_object);
      buffer_obj.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
      if_obj.nested.push_back(buffer_obj);
      seq_object.nested.push_back(if_obj);
    } else if ((*stmt)->getStmtClass() == Stmt::DefaultStmtClass) {
      const auto *default_stmt = dyn_cast<DefaultStmt>(*stmt);
      EOObject buffer_obj{"seq"};
      if (default_stmt->getSubStmt() != nullptr) {
        buffer_obj.nested.push_back(
            GetStmtEOObject(default_stmt->getSubStmt()));
      }
      auto tmp = stmt;
      tmp++;
      while (tmp != end && (*tmp)->getStmtClass() != Stmt::CaseStmtClass &&
             (*tmp)->getStmtClass() != Stmt::DefaultStmtClass) {
        buffer_obj.nested.push_back(GetStmtEOObject(*tmp));
        tmp++;
      }
      buffer_obj.nested.push_back(set_flag_object);
      buffer_obj.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
      seq_object.nested.push_back(buffer_obj);
    }
  }
  return_label.nested.push_back(seq_object);
  goto_object.nested.push_back(return_label);

  return goto_object;
}

void AppendDeclStmt(const DeclStmt *stmt) {
  for (auto *decl : stmt->decls()) {
    if (decl->getKind() == Decl::Kind::Enum) {
      auto *enum_decl = dyn_cast<clang::EnumDecl>(decl);
      ProcessEnumDecl(enum_decl);
    }
  }
}

EOObject GetCaseCondEOObject(const vector<const Expr *> &all_cases,
                             const EOObject &switch_exp, size_t i) {
  EOObject eq_object{"eq"};
  eq_object.nested.push_back(switch_exp);
  eq_object.nested.push_back(GetStmtEOObject(all_cases[i]));
  if (i + 1 == all_cases.size()) {
    return eq_object;
  }
  EOObject or_object{"or"};
  or_object.nested.push_back(eq_object);
  or_object.nested.push_back(GetCaseCondEOObject(all_cases, switch_exp, i + 1));
  return or_object;
}

EOObject GetCastEOObject(const CastExpr *op) {
  if (op == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }

  auto cast_kind = op->getCastKind();
  if (cast_kind == clang::CK_LValueToRValue) {
    QualType qual_type = op->getType();
    string type = GetTypeName(qual_type);
    EOObject read{"read"};
    read.nested.push_back(GetStmtEOObject(*op->child_begin()));
    if (!qual_type->isRecordType()) {
      read.name += "-as-" + type;
    } else {
      read.nested.emplace_back(
          to_string(transpiler.record_manager_
                        .GetById(qual_type->getAsRecordDecl()->getID())
                        ->size),
          EOObjectType::EO_LITERAL);
    }
    return read;
  }
  if (cast_kind == clang::CK_FloatingToIntegral ||
      cast_kind == clang::CK_IntegralToFloating) {
    QualType qual_type = op->getType();
    string type = GetTypeName(qual_type);
    EOObject cast{"as-" + type};
    cast.nested.push_back(GetStmtEOObject(*op->child_begin()));
    return cast;
  }
  // TODO(nkchuykin) if cast kinds and also split it to another func
  return GetStmtEOObject(*op->child_begin());
}

EOObject GetForStmtEOObject(const ForStmt *p_stmt) {
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  EOObject for_stmt(EOObjectType::EO_EMPTY);
  const auto *init = p_stmt->getInit();
  if (init != nullptr) {
    for_stmt.nested.push_back(GetStmtEOObject(init));
  }
  EOObject goto_object_1{"goto"};
  EOObject return_label_1{EOObjectType::EO_ABSTRACT};
  return_label_1.arguments.emplace_back("for-loop-label-1");
  EOObject break_obj{"for-loop-label-1.forward TRUE", "break"};
  EOObject if_object{"if", "@"};
  const auto *cond = p_stmt->getCond();
  if (cond != nullptr) {
    if_object.nested.push_back(GetStmtEOObject(cond));
  } else {
    if_object.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  }
  EOObject seq_object_1{"seq"};
  EOObject goto_object_2{"goto"};
  EOObject return_label_2{EOObjectType::EO_ABSTRACT};
  return_label_2.arguments.emplace_back("for-loop-label-2");
  EOObject continue_obj{"for-loop-label-2.forward TRUE", "continue"};
  EOObject seq_object_2{"seq", "@"};
  seq_object_2.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  seq_object_2.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return_label_2.nested.push_back(continue_obj);
  return_label_2.nested.push_back(seq_object_2);
  goto_object_2.nested.push_back(return_label_2);
  seq_object_1.nested.push_back(goto_object_2);
  const auto *inc = p_stmt->getInc();
  if (inc != nullptr) {
    seq_object_1.nested.push_back(GetSeqForBodyEOObject(inc));
  }
  seq_object_1.nested.emplace_back("for-loop-label-1.backward",
                                   EOObjectType::EO_LITERAL);
  seq_object_1.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  if_object.nested.push_back(seq_object_1);
  return_label_1.nested.push_back(break_obj);
  return_label_1.nested.push_back(if_object);
  goto_object_1.nested.push_back(return_label_1);
  for_stmt.nested.push_back(goto_object_1);
  return for_stmt;
}

EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op,
                                       std::vector<uint64_t> *dims,
                                       size_t depth) {
  std::vector<uint64_t> tmp_dims;
  auto decl_info = getMultiDimArrayTypeSize(op, &tmp_dims);
  if (tmp_dims.size() > dims->size()) {
    dims = &tmp_dims;
  }
  uint64_t dim_size = decl_info.first;  // current dimension size.
  for (int i = 0; i < depth && i < dims->size(); ++i) {
    dim_size *= dims->at(i);
  }

  if (op != nullptr) {
    for (const auto *base_ch : op->getBase()->children()) {
      auto index_name = GetStmtEOObject(op->getIdx());

      EOObject curr_shift{"times"};
      EOObject type_size_obj{std::to_string(dim_size),
                             EOObjectType::EO_LITERAL};
      curr_shift.nested.emplace_back(index_name);
      curr_shift.nested.emplace_back(type_size_obj);

      auto stmt_class = base_ch->getStmtClass();
      if (stmt_class == Stmt::ArraySubscriptExprClass) {
        EOObject add_shift{"plus"};

        const auto *arr_sub_expr = dyn_cast<ArraySubscriptExpr>(base_ch);
        EOObject next_shift =
            GetArraySubscriptExprEOObject(arr_sub_expr, dims, depth + 1);

        add_shift.nested.emplace_back(curr_shift);
        add_shift.nested.emplace_back(next_shift);

        if (depth == 0) {
          EOObject final_write{"plus"};
          final_write.nested.emplace_back(decl_info.second);
          final_write.nested.emplace_back(add_shift);
          return final_write;
        }
        return add_shift;
      }
      if (stmt_class == Stmt::DeclRefExprClass ||
          stmt_class == Stmt::MemberExprClass) {
        if (depth == 0) {
          EOObject final_write{"plus"};
          final_write.nested.emplace_back(decl_info.second);
          final_write.nested.emplace_back(curr_shift);
          return final_write;
        }
      }
      return curr_shift;
    }
  }
  return EOObject{EOObjectType::EO_PLUG};
}

std::pair<uint64_t, EOObject> getMultiDimArrayTypeSize(
    const ArraySubscriptExpr *op, std::vector<uint64_t> *dims) {
  if (op == nullptr) {
    return std::make_pair(0, EOObject{EOObjectType::EO_PLUG});
  }
  for (const auto *base_ch : op->getBase()->children()) {
    auto stmt_class = base_ch->getStmtClass();
    if (stmt_class == Stmt::DeclRefExprClass) {
      const auto *decl_ref_expr = dyn_cast<DeclRefExpr>(base_ch);
      if (decl_ref_expr == nullptr) {
        continue;
      }
      auto qt = decl_ref_expr->getType();
      EOObject arr_name = GetStmtEOObject(op->getBase());
      size_t sz =
          decl_ref_expr->getDecl()->getASTContext().getTypeInfo(qt).Align /
          byte_size;
      return std::make_pair(sz, arr_name);
    }
    if (stmt_class == Stmt::ArraySubscriptExprClass) {
      const auto *arr_sub_expr = dyn_cast<ArraySubscriptExpr>(base_ch);
      if (arr_sub_expr == nullptr) {
        continue;
      }
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
      if (memb_expr == nullptr) {
        continue;
      }
      const auto *child = dyn_cast<Expr>(*memb_expr->child_begin());
      if (child == nullptr) {
        continue;
      }
      QualType qual_type = child->getType();
      EOObject arr_name = GetStmtEOObject(op->getBase());
      size_t sz = transpiler.record_manager_
                      .GetById(qual_type->getAsRecordDecl()->getID())
                      ->size;
      return std::make_pair(sz, arr_name);
    }
    std::cerr << base_ch->getStmtClassName() << "\n\n";
  }
  return std::make_pair(0, EOObject{"plug", EOObjectType::EO_PLUG});
}

EOObject GetMemberExprEOObject(const MemberExpr *op) {
  EOObject member{"plus"};
  if (op == nullptr) {
    return member;
  }
  const auto *child = dyn_cast<Expr>(*op->child_begin());
  if (child == nullptr) {
    return member;
  }
  QualType qual_type = child->getType();
  if (qual_type->isPointerType()) {
    EOObject record{"address"};
    qual_type = dyn_cast<clang::PointerType>(qual_type.getCanonicalType())
                    ->getPointeeType();
    record.nested.emplace_back("global-ram");
    record.nested.push_back(GetStmtEOObject(child));
    member.nested.push_back(record);
  } else {
    member.nested.push_back(GetStmtEOObject(child));
  }
  auto *field = llvm::dyn_cast<clang::FieldDecl>(op->getMemberDecl());

  if (field == nullptr) {
    return {};
  }
  std::string field_name;
  if (!field->isUnnamedBitfield() && !field->getNameAsString().empty()) {
    field_name = /* "f-" + */ field->getNameAsString();
  } else {
    field_name = "field" + std::to_string(field->getID());
  }
  member.nested.push_back(transpiler.record_manager_.GetShiftAlias(
      qual_type->getAsRecordDecl()->getID(), field_name));
  return member;
}

EOObject GetFunctionCallEOObject(const CallExpr *op) {
  EOObject call("seq");
  vector<std::size_t> var_sizes;
  if (op->getDirectCallee()->isCXXClassMember() ||
      op->getDirectCallee()->isCXXInstanceMember()) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  if (op != nullptr && op->getDirectCallee() != nullptr) {
    for (auto *VD : op->getDirectCallee()->parameters()) {
      TypeInfo type_info = VD->getASTContext().getTypeInfo(VD->getType());
      size_t type_size = type_info.Width / byte_size;
      var_sizes.push_back(type_size);
    }
  }
  size_t shift = 0;
  int i = 0;
  if (op != nullptr && op->getNumArgs() <= var_sizes.size()) {
    for (const auto *arg : op->arguments()) {
      EOObject param{"write"};
      string postfix = GetPostfix(arg->getType());
      if (!postfix.empty()) {
        param.name += "-as-" + postfix;
      }
      EOObject address{"address"};
      address.nested.emplace_back("global-ram");
      EOObject add{"plus"};
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
  }
  if (op != nullptr) {
    call.nested.push_back(
        transpiler.func_manager_.GetFunctionCall(op->getDirectCallee(), shift));
    QualType qual_type = op->getType();
    std::string postfix = GetTypeName(qual_type);
    if (postfix != "undefinedtype") {
      EOObject read_ret{"read"};
      EOObject ret_val{"return"};
      read_ret.nested.push_back(ret_val);
      if (qual_type->isRecordType() || qual_type->isArrayType()) {
        read_ret.nested.emplace_back(to_string(var_sizes[0]),
                                     EOObjectType::EO_LITERAL);
      } else {
        read_ret.name += "-as-" + postfix;
      }
      call.nested.push_back(read_ret);
    } else {
      call.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
    }
  }

  return call;
}

EOObject GetPrintfCallEOObject(const CallExpr *op) {
  EOObject printf{"printf"};
  for (const auto *arg : op->arguments()) {
    printf.nested.push_back(GetStmtEOObject(arg));
  }
  return printf;
}

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal) {
  std::ostringstream ss{};
  if (p_literal != nullptr) {
    llvm::APFloat an_float = p_literal->getValue();
    ss << std::fixed << an_float.convertToDouble();
  }
  return {ss.str(), EOObjectType::EO_LITERAL};
}

EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal) {
  if (p_literal != nullptr) {
    bool is_signed = p_literal->getType()->isSignedIntegerType();
    llvm::APInt an_int = p_literal->getValue();
    if (is_signed) {
      int64_t val = an_int.getSExtValue();
      std::string str_val{std::to_string(val)};
      return EOObject{str_val, EOObjectType::EO_LITERAL};
    }
    uint64_t val = an_int.getZExtValue();
    std::string str_val{std::to_string(val)};
    return EOObject{str_val, EOObjectType::EO_LITERAL};
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator) {
  auto op_code = p_operator->getOpcode();
  std::string operation;

  if (op_code == BinaryOperatorKind::BO_AddAssign) {
    operation = "plus";
  } else if (op_code == BinaryOperatorKind::BO_SubAssign) {
    operation = "minus";
  } else if (op_code == BinaryOperatorKind::BO_MulAssign) {
    operation = "times";
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
  EOObject eo_object{"read"};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  if (left != nullptr) {
    QualType qual_type = left->getType();
    eo_object.nested.push_back(GetStmtEOObject(left));
    if (!qual_type->isRecordType()) {
      eo_object.name += "-as-" + GetTypeName(qual_type);
    } else {
      eo_object.nested.emplace_back(
          to_string(transpiler.record_manager_
                        .GetById(qual_type->getAsRecordDecl()->getID())
                        ->size),
          EOObjectType::EO_LITERAL);
    }
  }
  binary_op.nested.emplace_back(eo_object);

  binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  std::string operation;
  if (p_operator == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  auto op_code = p_operator->getOpcode();
  if (op_code == BinaryOperatorKind::BO_Assign) {
    return GetAssignmentOperatorEOObject(p_operator);
  }
  if (op_code == BinaryOperatorKind::BO_Add) {
    operation = "plus";
  } else if (op_code == BinaryOperatorKind::BO_Sub) {
    operation = "minus";
  } else if (op_code == BinaryOperatorKind::BO_Mul) {
    operation = "times";
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
  } else if (op_code == BinaryOperatorKind::BO_LAnd) {
    operation = "and";
  } else if (op_code == BinaryOperatorKind::BO_LOr) {
    operation = "or";
  } else if (op_code == BinaryOperatorKind::BO_Shl) {
    operation = "shift-left";
  } else if (op_code == BinaryOperatorKind::BO_Shr) {
    operation = "shift-right";
  } else if (op_code == BinaryOperatorKind::BO_EQ) {
    operation = "eq";
  } else if (op_code == BinaryOperatorKind::BO_NE) {
    operation = "neq";
  } else if (op_code == BinaryOperatorKind::BO_LT) {
    operation = "lt";
  } else if (op_code == BinaryOperatorKind::BO_LE) {
    operation = "lte";
  } else if (op_code == BinaryOperatorKind::BO_GT) {
    operation = "gt";
  } else if (op_code == BinaryOperatorKind::BO_GE) {
    operation = "gte";
  } else if (op_code == BinaryOperatorKind::BO_Comma) {
    operation = "seq";
  } else {
    operation = "undefined";
    llvm::errs() << "Warning: Unknown operator " << p_operator->getOpcodeStr()
                 << "\n";
  }

  EOObject binary_op{operation};
  binary_op.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetUnaryStmtEOObject(const UnaryOperator *p_operator) {
  std::string operation;
  if (p_operator == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  auto op_code = p_operator->getOpcode();
  Stmt *stmt = nullptr;

  //   QualType argType = p_operator->getType();
  //   uint64_t typeSize = GetTypeSize(argType);

  // [C99 6.5.2.4] Postfix increment and decrement
  if (op_code ==
      UnaryOperatorKind::UO_PostInc) {  // UNARY_OPERATION(PostInc, "++")
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
  if (op_code ==
      UnaryOperatorKind::UO_PostDec) {  // UNARY_OPERATION(PostDec, "--")
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
  if (op_code ==
      UnaryOperatorKind::UO_PreInc) {  // UNARY_OPERATION(PreInc, "++")
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
  if (op_code ==
      UnaryOperatorKind::UO_PreDec) {  // UNARY_OPERATION(PreDec, "--")
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
  if (op_code ==
      UnaryOperatorKind::UO_AddrOf) {  // UNARY_OPERATION(AddrOf, "&")
    EOObject variable{"addr-of"};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  }
  if (op_code == UnaryOperatorKind::UO_Deref) {  // UNARY_OPERATION(Deref, "*")
    EOObject variable{"address"};
    EOObject ram{"global-ram"};
    variable.nested.push_back(ram);
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // [C99 6.5.3.3] Unary arithmetic
  }
  if (op_code == UnaryOperatorKind::UO_Plus) {  // UNARY_OPERATION(Plus, "+")
    operation = "pos";
  } else if (op_code ==
             UnaryOperatorKind::UO_Minus) {  // UNARY_OPERATION(Minus, "-")
    operation = "neg";
  } else if (op_code ==
             UnaryOperatorKind::UO_Not) {  // UNARY_OPERATION(Not, "~")
    operation = "bit-not";
  } else if (op_code ==
             UnaryOperatorKind::UO_LNot) {  // UNARY_OPERATION(LNot, "!")
    operation = "not";
    // "__real expr"/"__imag expr" Extension.
  } else if (op_code ==
             UnaryOperatorKind::UO_Real) {  // UNARY_OPERATION(Real, "__real")
    operation = "real";
  } else if (op_code ==
             UnaryOperatorKind::UO_Imag) {  // UNARY_OPERATION(Imag, "__imag")
    operation = "imag";
    // __extension__ marker.
  } else if (op_code ==
             UnaryOperatorKind::UO_Extension) {  // UNARY_OPERATION(Extension,
    // "__extension__")
    operation = "extension";
    // [C++ Coroutines] co_await operator
  } else if (op_code ==
             UnaryOperatorKind::UO_Coawait) {  // UNARY_OPERATION(Coawait,
    // "co_await")
    operation = "coawait";
    // Incorrect unary operator
  } else {
    operation = "undefined";
    llvm::errs() << "Warning: Unknown operator "
                 << UnaryOperator::getOpcodeStr(p_operator->getOpcode())
                 << "\n";
  }

  EOObject unary_op{operation};
  unary_op.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
  return unary_op;
}

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator) {
  EOObject binary_op{"write"};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  if (left != nullptr) {
    QualType qual_type = left->getType();
    if (!qual_type->isRecordType()) {
      binary_op.name += "-as-" + GetTypeName(left->getType());
    }
    binary_op.nested.emplace_back(GetStmtEOObject(left));
    binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
    return binary_op;
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetEODeclRefExpr(const DeclRefExpr *op) {
  if (op == nullptr) {
    return EOObject{EOObjectType::EO_EMPTY};
  }
  try {
    const auto *val = op->getFoundDecl();
    if (val->getKind() == clang::Decl::EnumConstant) {
      const auto *id = dyn_cast<EnumConstantDecl>(val);
      const auto &var = transpiler.enum_manager_.GetConstantById(id);
      return EOObject{std::to_string(var->value), EOObjectType::EO_LITERAL};
    }
    const auto *id = dyn_cast<VarDecl>(val);
    if (id->isStaticLocal()) {
      return EOObject{"s-" + id->getName().str()};
    }
    const auto &var = transpiler.glob_.GetVarById(id);
    return EOObject{var.alias};
  } catch (std::invalid_argument &) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetAssignmentOperationOperatorEOObject(
    const CompoundAssignOperator *p_operator) {
  EOObject binary_op{"write"};
  if (p_operator == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  if (left == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
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
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  const auto *ret_value = p_stmt->getRetValue();
  if (ret_value != nullptr) {
    EOObject ret{"write"};
    string postfix = GetPostfix(ret_value->getType());
    if (!postfix.empty()) {
      ret.name += "-as-" + postfix;
    }
    EOObject address{"return"};
    ret.nested.push_back(address);
    ret.nested.push_back(GetStmtEOObject(ret_value));
    result.nested.push_back(ret);
  }
  EOObject label{"goto-return-label.forward TRUE", EOObjectType::EO_LITERAL};
  result.nested.push_back(label);
  return result;
}

EOObject GetIfElseStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_else_stmt{"if-else"};
  if_else_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_else_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getThen()));
  if_else_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getElse()));
  return if_else_stmt;
}

EOObject GetIfStmtEOObject(const IfStmt *p_stmt) {
  EOObject if_stmt{"if"};
  if (p_stmt != nullptr) {
    if_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
    if_stmt.nested.push_back(GetSeqForBodyEOObject(p_stmt->getThen()));
    return if_stmt;
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetWhileStmtEOObject(const WhileStmt *p_stmt) {
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  EOObject goto_object{"goto"};
  EOObject return_label{EOObjectType::EO_ABSTRACT};
  string label_name = "while-loop-label";
  return_label.arguments.emplace_back(label_name);
  EOObject continue_obj{label_name + ".backward", "continue"};
  EOObject break_obj{label_name + ".forward TRUE", "break"};
  EOObject if_obj{"if", "@"};
  if_obj.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  EOObject seq_obj{"seq"};
  seq_obj.nested.push_back(GetSeqForBodyEOObject(p_stmt->getBody()));
  seq_obj.nested.emplace_back("continue");
  seq_obj.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  if_obj.nested.push_back(seq_obj);
  return_label.nested.push_back(continue_obj);
  return_label.nested.push_back(break_obj);
  return_label.nested.push_back(if_obj);
  goto_object.nested.push_back(return_label);
  return goto_object;
}

EOObject GetDoWhileStmtEOObject(const DoStmt *p_stmt) {
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  EOObject goto_object_1{"goto"};
  EOObject return_label_1{EOObjectType::EO_ABSTRACT};
  return_label_1.arguments.emplace_back("do-while-loop-label-1");
  EOObject break_obj{"do-while-loop-label-1.forward TRUE", "break"};

  EOObject seq_object_1{"seq", "@"};

  EOObject goto_object_2{"goto"};
  EOObject return_label_2{EOObjectType::EO_ABSTRACT};
  return_label_2.arguments.emplace_back("do-while-loop-label-2");
  EOObject continue_obj{"do-while-loop-label-2.forward TRUE", "continue"};
  EOObject seq_object_2{"seq", "@"};
  seq_object_2.nested.push_back(GetStmtEOObject(p_stmt->getBody()));
  seq_object_2.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return_label_2.nested.push_back(continue_obj);
  return_label_2.nested.push_back(seq_object_2);
  goto_object_2.nested.push_back(return_label_2);

  EOObject if_obj{"if"};
  if_obj.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_obj.nested.emplace_back("do-while-loop-label-1.backward",
                             EOObjectType::EO_LITERAL);

  seq_object_1.nested.push_back(goto_object_2);
  seq_object_1.nested.push_back(if_obj);
  seq_object_1.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return_label_1.nested.push_back(break_obj);
  return_label_1.nested.push_back(seq_object_1);

  goto_object_1.nested.push_back(return_label_1);

  return goto_object_1;
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
  const clang::Type *type_ptr = qual_type.getTypePtr();
  TypeInfo type_info = context->getTypeInfo(type_ptr);
  uint64_t type_size = type_info.Width;

  if (type_ptr->isPointerType()) {
    const clang::Type *pointer_type = type_ptr->getPointeeType().getTypePtr();
    TypeInfo pointer_type_info = context->getTypeInfo(pointer_type);
    uint64_t pointer_type_size = pointer_type_info.Width;
    return pointer_type_size / byte_size;
  }

  return type_size / byte_size;
}

std::string GetPostfix(QualType qual_type) {
  const clang::Type *type_ptr = qual_type.getTypePtr();
  TypeInfo type_info = context->getTypeInfo(type_ptr);
  uint64_t type_size = type_info.Width;
  std::string str;

  if (type_ptr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (type_ptr->isPointerType()) {
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
  //  if (type_ptr->isCharType()) {
  //    str += "int8";
  //    return str;
  //  }
  if (type_ptr->isIntegerType()) {
    str += "int" + std::to_string(type_size);
    return str;
  }
  return "";
}

std::string GetTypeName(QualType qual_type) {
  const clang::Type *type_ptr = qual_type.getTypePtr();
  TypeInfo type_info = context->getTypeInfo(type_ptr);
  uint64_t type_size = type_info.Width;
  std::string str;

  if (type_ptr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (type_ptr->isPointerType()) {
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
  //  if (type_ptr->isCharType()) {
  //    str += "char";
  //    return str;
  //  }
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
      str += std::to_string(reinterpret_cast<uint64_t>(RD));
    }
    return str;
  }

  return "undefinedtype";
}

std::set<std::string> FindAllExternalObjects(const EOObject &obj) {
  std::set<std::string> all_known = {obj.postfix};
  std::set<std::string> unknown{};
  // TODO(nkchuykin) maybe should use pointers or copy constructor to avoid
  // unnecessary copying of objects_
  std::queue<EOObject> not_visited;
  for (auto child : obj.nested) {
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty()) {
    EOObject cur = not_visited.front();
    not_visited.pop();
    switch (cur.type) {
      case EOObjectType::EO_ABSTRACT:
        all_known.insert(cur.postfix);
        for (const auto &arg : cur.arguments) {
          all_known.insert(arg);
        }
        break;
      case EOObjectType::EO_COMPLETE:
        all_known.insert(cur.postfix);
        if (all_known.find(cur.name) == all_known.end()) {
          unknown.insert(cur.name);
        }
        break;
      case EOObjectType::EO_EMPTY:
      case EOObjectType::EO_LITERAL:
      case EOObjectType::EO_TEMPLATE:
        break;
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
