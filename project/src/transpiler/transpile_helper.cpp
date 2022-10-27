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
#include <tuple>
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
// using clang::BinaryOperatorKind;
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
using clang::FunctionDecl;
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
// using clang::UnaryOperatorKind;
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

EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op);

std::pair<uint64_t, EOObject> getMultiDimArrayTypeSize(
    const ArraySubscriptExpr *op, std::vector<uint64_t> *dims);

EOObject GetForStmtEOObject(const ForStmt *p_stmt);

EOObject GetSeqForBodyEOObject(const Stmt *p_stmt);

uint64_t GetTypeSize(QualType qual_type);

uint64_t GetSizeOfType(QualType qual_type);

EOObject GetCastEOObject(const CastExpr *op);

EOObject GetSwitchEOObject(const SwitchStmt *p_stmt);

EOObject GetCaseCondEOObject(const vector<const Expr *> &all_cases,
                             const EOObject &switch_exp, size_t i);

EOObject GetCharacterLiteralEOObject(const clang::CharacterLiteral *p_literal);
void AppendDeclStmt(const DeclStmt *stmt);

EOObject GetUnaryExprOrTypeTraitExprEOObject(
    const clang::UnaryExprOrTypeTraitExpr *p_expr);

EOObject GetGotoStmtEOObject(const clang::GotoStmt *p_stmt);
EOObject GetLabelStmtEOObject(const clang::LabelStmt *p_stmt);
EOObject GetConditionalStmtEOObject(const clang::ConditionalOperator *p_stmt);
bool IsLeftNestedObjectsContainLabel(const EOObject &object, const char *label);
const Expr *GetPureStmtNode(const Expr *pExpr);
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
  vector<Variable> all_static_local;
  ProcessFunctionLocalVariables(func_body, all_static_local, shift, true);
  shift = transpiler.glob_.GetFreeSpacePointer();
  const size_t param_memory_size = GetParamMemorySize(FD->parameters());
  const vector<Variable> all_param =
      ProcessFunctionParams(FD->parameters(), shift);
  const vector<EOObject> all_types = PrecessRecordTypes(func_body);

  vector<Variable> all_local;
  ProcessFunctionLocalVariables(func_body, all_local, shift + param_memory_size,
                                false);
  EOObject func_body_eo = EOObject(EOObjectType::EO_EMPTY);
  EOObject local_start("plus", "local-start");
  local_start.nested.emplace_back("param-start");
  local_start.nested.emplace_back("param-size");
  func_body_eo.nested.push_back(local_start);
  size_t local_static_size = 0;
  for (const auto &var : all_local) {
    if (var.id != nullptr && var.id->isStaticLocal()) {
      local_static_size +=
          transpiler.type_manger_.GetById(var.typeInfoID).GetSizeOfType();
    }
  }
  const size_t free_pointer =
      transpiler.glob_.GetFreeSpacePointer() - local_static_size;
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
    if (var.id != nullptr && var.id->isStaticLocal()) {
      continue;
    }
    func_body_eo.nested.push_back(var.GetAddress(transpiler.glob_.name_));
  }
  EOObject goto_object{"goto", "@"};
  EOObject return_label{EOObjectType::EO_ABSTRACT};
  return_label.arguments.emplace_back("goto-return-label");
  const EOObject body_seq = GetCompoundStmt(func_body, true);
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
    const Stmt::StmtClass stmt_class = stmt->getStmtClass();
    if (stmt_class == Stmt::DeclStmtClass) {
      auto *decl_stmt = dyn_cast<DeclStmt>(stmt);
      if (decl_stmt != nullptr) {
        for (auto *decl : decl_stmt->decls()) {
          const Decl::Kind decl_kind = decl->getKind();
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
    const TypeSimpl typeInfo =
        transpiler.type_manger_.Add(VD->getType().getTypePtr());
    res += typeInfo.GetSizeOfType();
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
  if (CS != nullptr) {
    for (auto *stmt : CS->body()) {
      const EOObject stmt_obj = GetStmtEOObject(stmt);
      res.nested.push_back(stmt_obj);
    }
  }
  res.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
  return res;
}

EOObject GetStmtEOObject(const Stmt *stmt) {
  if (stmt == nullptr) {
    std::cerr << "Warning: Try to construct EOObject for nullptr\n";
    // llvm::errs() << "Warning: Try to construct EOObject for nullptr\n";
    return EOObject(EOObjectType::EO_PLUG);
  }
  const Stmt::StmtClass stmt_class = stmt->getStmtClass();
  // TEST
  // const char *stmt_class_name = stmt->getStmtClassName();
  // std::cout << "Statement Class Name = " << stmt_class_name << "\n";

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
    EOObject result = EOObject{EOObjectType::EO_EMPTY};
    for (auto *decl : op->decls()) {
      if (decl->getKind() == clang::Decl::Var) {
        auto *VD = dyn_cast<VarDecl>(decl);
        result.nested.push_back(
            transpiler.glob_.GetVarById(VD).GetInitializer());
      } else if (decl->getKind() == Decl::Kind::Record) {
        auto *record_decl = dyn_cast<RecordDecl>(decl);
        auto types = ProcessRecordType(record_decl, true);
        for (auto &type : types) {
          auto eo_objs = type.GetEORecordDecl();
          result.nested.insert(result.nested.end(), eo_objs.begin(),
                               eo_objs.end());
        }
      }
    }
    return result;
  }
  if (stmt_class == Stmt::CallExprClass) {
    const auto *op = dyn_cast<CallExpr>(stmt);
    if (op->getDirectCallee() != nullptr &&
        op->getDirectCallee()->getNameInfo().getAsString() == "printf") {
      EOObject eo = GetPrintfCallEOObject(op);
      return eo;
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
    return GetArraySubscriptExprEOObject(op);
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
  if (stmt_class == Stmt::UnaryExprOrTypeTraitExprClass) {
    const auto *op = dyn_cast<clang::UnaryExprOrTypeTraitExpr>(stmt);
    return GetUnaryExprOrTypeTraitExprEOObject(op);
  }
  if (stmt_class == Stmt::StringLiteralClass) {
    const auto *op = dyn_cast<clang::StringLiteral>(stmt);
    //    op->getCharByteWidth() is important and can be > 1
    std::string value = Escaped(op->getBytes().str());
    // TODO(nchuykin) remove lines below after fixing printf EOObject
    value = std::regex_replace(value, std::regex("%[lh]{1,2}"), "%");
    value = std::regex_replace(value, std::regex("%u"), "%d");

    return {"\"" + value + "\"", EOObjectType::EO_LITERAL};
  }
  if (stmt_class == Stmt::ImplicitValueInitExprClass) {
    //    const auto *op = dyn_cast<clang::ImplicitValueInitExpr>(stmt);
    // do i need type or other info?
    return {"0", EOObjectType::EO_LITERAL};
  }
  if (stmt_class == Stmt::GotoStmtClass) {
    const auto *op = dyn_cast<clang::GotoStmt>(stmt);
    return GetGotoStmtEOObject(op);
  }
  if (stmt_class == Stmt::LabelStmtClass) {
    const auto *op = dyn_cast<clang::LabelStmt>(stmt);
    return GetLabelStmtEOObject(op);
  }
  if (stmt_class == Stmt::ConditionalOperatorClass) {
    const auto *op = dyn_cast<clang::ConditionalOperator>(stmt);
    return GetConditionalStmtEOObject(op);
  }
  llvm::errs() << "Warning: Unknown statement " << stmt->getStmtClassName()
               << "\n";
  return EOObject(EOObjectType::EO_PLUG);
}
EOObject GetLabelStmtEOObject(const clang::LabelStmt *p_stmt) {
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  EOObject res{EOObjectType::EO_EMPTY};
  EOObject c_label{"c-label"};
  c_label.nested.emplace_back("\"" + string(p_stmt->getName()) + "\"",
                              EOObjectType::EO_LITERAL);
  res.nested.push_back(c_label);
  if (p_stmt->getSubStmt() != nullptr) {
    res.nested.push_back(GetStmtEOObject(p_stmt->getSubStmt()));
  }
  return res;
}
EOObject GetGotoStmtEOObject(const clang::GotoStmt *p_stmt) {
  if (p_stmt == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  // EOObject res{EOObjectType::EO_EMPTY};
  EOObject c_goto{"c-goto"};
  c_goto.nested.emplace_back(
      "\"" + p_stmt->getLabel()->getNameAsString() + "\"",
      EOObjectType::EO_LITERAL);
  return c_goto;
}
EOObject GetCharacterLiteralEOObject(const clang::CharacterLiteral *p_literal) {
  if (p_literal != nullptr) {
    const unsigned int an_int = p_literal->getValue();
    const std::string str_val{std::to_string(an_int)};
    return EOObject{str_val, EOObjectType::EO_LITERAL};
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetInitListEOObject(const clang::InitListExpr *list) {
  EOObject eoList{"*", EOObjectType::EO_EMPTY};
  const TypeSimpl typeInfo =
      transpiler.type_manger_.Add(list->getType().getTypePtrOrNull());
  const std::vector<EOObject> inits;
  TypeSimpl elementType;
  std::vector<std::tuple<std::string, TypeSimpl, size_t>>::iterator recElement;
  size_t elementSize = 0;
  if (typeInfo.typeStyle == ComplexType::ARRAY) {
    elementType = transpiler.type_manger_.GetById(typeInfo.subTypeId);
    elementSize = elementType.GetSizeOfType();
  } else if (typeInfo.typeStyle == ComplexType::RECORD) {
    auto *recordType = transpiler.record_manager_.GetById(typeInfo.recordId);
    recElement = recordType->fields.begin();
  }
  int i = 0;
  for (auto element = list->child_begin(); element != list->child_end();
       element++, i++) {
    EOObject shiftedAlias{"plus"};
    shiftedAlias.nested.emplace_back("list-init-name",
                                     EOObjectType::EO_TEMPLATE);
    if (typeInfo.typeStyle == ComplexType::ARRAY) {
      EOObject newShift{"times"};
      newShift.nested.emplace_back(to_string(i), EOObjectType::EO_LITERAL);
      newShift.nested.emplace_back(to_string(elementSize),
                                   EOObjectType::EO_LITERAL);
      shiftedAlias.nested.push_back(newShift);
    } else if (typeInfo.typeStyle == ComplexType::RECORD) {
      shiftedAlias.nested.emplace_back(transpiler.record_manager_.GetShiftAlias(
          typeInfo.recordId, std::get<0>(*recElement)));
      elementType = std::get<1>(*recElement);
    }
    EOObject value = GetStmtEOObject(*element);
    if (value.type == EOObjectType::EO_EMPTY && value.name == "*") {
      EOObject newValue = ReplaceEmpty(value, shiftedAlias);
      eoList.nested.insert(eoList.nested.end(), newValue.nested.begin(),
                           newValue.nested.end());
    } else if (elementType.typeStyle != ComplexType::RECORD &&
               elementType.typeStyle != ComplexType::ARRAY) {
      EOObject constData{"write"};
      EOObject res("write");
      if (elementType.name == "ptr" && value.nested.empty()) {
        const TypeSimpl item_type =
            transpiler.type_manger_.GetById(elementType.subTypeId);
        if (item_type.name != "undefinedtype") {
          uint64_t type_size = 0;
          if (item_type.name == "int8") {
            constData.name += "-as-string";
            type_size = value.name.length() - 1;
          } else {
            if (item_type.typeStyle != ComplexType::RECORD &&
                item_type.typeStyle != ComplexType::ARRAY) {
              constData.name += "-as-" + item_type.name;
            }
            type_size = item_type.GetSizeOfType();
          }
          {
            EOObject address{"address"};
            address.nested.emplace_back("global-ram");
            address.nested.emplace_back(
                to_string(transpiler.glob_.GetFreeSpacePointer()),
                EOObjectType::EO_LITERAL);
            transpiler.glob_.ShiftFreeSpacePointer(type_size);
            constData.nested.push_back(address);
            constData.nested.push_back(value);
            value = EOObject{"addr-of"};
            value.nested.push_back(address);
          }
        }
        eoList.nested.push_back(constData);
      }
      if (elementType.name != "undefinedtype" && !elementType.name.empty() &&
          elementType.typeStyle != ComplexType::RECORD &&
          !(elementType.typeStyle == ComplexType::ARRAY &&
            elementType.name != "string")) {
        res.name += "-as-" + elementType.name;
      } else /*if (elementType.name.empty())*/ {
        //        list->dump();
        //        std::cerr << ' ' << typeInfo.id << ' ' << typeInfo.name
        //                  << ' ' << typeInfo.size << '\n'
        //                  << '\n';
      }
      res.nested.emplace_back(shiftedAlias);
      res.nested.emplace_back(value);
      eoList.nested.push_back(res);
    }
    if (typeInfo.typeStyle == ComplexType::RECORD) {
      recElement++;
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

  const EOObject switch_expr_object = GetStmtEOObject(p_stmt->getCond());

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
      if (nested != nullptr &&
          nested->getStmtClass() == Stmt::DefaultStmtClass) {
        EOObject always_true_obj{"or"};
        always_true_obj.nested.push_back(cond_obj);
        always_true_obj.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
        if_obj.nested.push_back(always_true_obj);
        const auto *def_stmt = dyn_cast<DefaultStmt>(nested);
        nested = def_stmt->getSubStmt();
      } else {
        if_obj.nested.push_back(cond_obj);
      }
      EOObject buffer_obj{"seq"};
      if (nested != nullptr &&
          nested->getStmtClass() != Stmt::DefaultStmtClass) {
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

      if (nested != nullptr &&
          nested->getStmtClass() == Stmt::DefaultStmtClass) {
        const auto *default_stmt = dyn_cast<DefaultStmt>(nested);
        EOObject buffer_obj_def{"seq"};
        if (default_stmt->getSubStmt() != nullptr) {
          buffer_obj_def.nested.push_back(
              GetStmtEOObject(default_stmt->getSubStmt()));
        }
        auto tmp_def = stmt;
        tmp_def++;
        while (tmp_def != end &&
               (*tmp_def)->getStmtClass() != Stmt::CaseStmtClass &&
               (*tmp_def)->getStmtClass() != Stmt::DefaultStmtClass) {
          buffer_obj_def.nested.push_back(GetStmtEOObject(*tmp_def));
          tmp_def++;
        }
        buffer_obj_def.nested.push_back(set_flag_object);
        buffer_obj_def.nested.emplace_back("TRUE", EOObjectType::EO_LITERAL);
        seq_object.nested.push_back(buffer_obj_def);
      }
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
  const TypeSimpl typeInfo =
      transpiler.type_manger_.Add(op->getType().getTypePtr());
  const string type = typeInfo.name;
  if (cast_kind == clang::CK_LValueToRValue) {
    EOObject read{"read"};
    const EOObject value = GetStmtEOObject(*op->child_begin());
    read.nested.emplace_back(value);
    if (typeInfo.typeStyle == ComplexType::RECORD) {
      read.nested.emplace_back(
          to_string(
              transpiler.record_manager_.GetById(typeInfo.recordId)->size),
          EOObjectType::EO_LITERAL);

    } else if (type == "string") {
      read.name += "-as-ptr";
    } else {
      read.name += "-as-" + type;
    }
    return read;
  }
  if (cast_kind == clang::CK_FloatingToIntegral ||
      cast_kind == clang::CK_IntegralToFloating) {
    EOObject cast{"as-" + type};
    cast.nested.push_back(GetStmtEOObject(*op->child_begin()));
    return cast;
  }
  if (cast_kind == clang::CK_NullToPointer ||
      cast_kind == clang::CK_IntegralToPointer) {
    EOObject ptr{"read-as-ptr"};  // addr-of?
    EOObject address{"address"};
    address.nested.emplace_back("global-ram");
    address.nested.emplace_back(GetStmtEOObject(*op->child_begin()));
    ptr.nested.emplace_back(address);
    return ptr;
  }
  //  if (cast_kind == clang::CK_ArrayToPointerDecay) {
  //    EOObject read{"addr-of"};
  //    read.nested.push_back(GetStmtEOObject(*op->child_begin()));
  //    return read;
  //  }
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
  const EOObject break_obj{"for-loop-label-1.forward TRUE", "break"};
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
  const EOObject continue_obj{"for-loop-label-2.forward TRUE", "continue"};
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
EOObject GetArraySubscriptExprEOObject(const ArraySubscriptExpr *op) {
  auto index_name = GetStmtEOObject(op->getIdx());
  const uint64_t dim_size =
      transpiler.type_manger_.Add(op->getType().getTypePtr()).GetSizeOfType();
  EOObject addr{"plus"};
  const Expr *expr = op->getBase();
  //  const Expr *pureExpr = GetPureStmtNode(expr);
  addr.nested.emplace_back(GetStmtEOObject(expr));
  EOObject shift{"times"};
  shift.nested.emplace_back(index_name);
  shift.nested.emplace_back(std::to_string(dim_size), EOObjectType::EO_LITERAL);
  addr.nested.emplace_back(shift);

  const clang::Type *type = expr->IgnoreCasts()->getType().getTypePtrOrNull();
  //  op->dump();
  //  std::cerr << "-----\n";
  //  type->dump();
  //  std::cerr << '\n';
  const TypeSimpl typeInfo = transpiler.type_manger_.Add(type);
  if ((typeInfo.name == "ptr" ||
       IsLeftNestedObjectsContainLabel(addr, "addr-of")) &&
      !IsLeftNestedObjectsContainLabel(addr, "address")) {
    EOObject globAddr{"address"};
    globAddr.nested.emplace_back("global-ram");
    globAddr.nested.emplace_back(addr);
    return globAddr;
  }
  return addr;
}
// const Expr *GetPureStmtNode(const Expr *pExpr) {
//   Stmt::StmtClass stmt_class = pExpr->getStmtClass();
//   if (stmt_class >= clang::Stmt::firstCastExprConstant &&
//       stmt_class <= clang::Stmt::lastCastExprConstant) {
//     const auto *op = dyn_cast<CastExpr>(pExpr);
//     return op.
//   }
//   return pExpr;
// }
bool IsLeftNestedObjectsContainLabel(const EOObject &object,
                                     const char *label = "") {
  if (object.name == label) {
    return true;
  }
  if (object.nested.empty()) {
    return false;
  }
  return IsLeftNestedObjectsContainLabel(object.nested[0], label);
}

EOObject GetMemberExprEOObject(const MemberExpr *op) {
  // TEST
  // !!std::cout << "GetMemberExprEOObject: start\n";
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

size_t GetEOParamsList(const CallExpr *op, EOObject &call) {
  size_t shift = 0;
  for (const auto *arg : op->arguments()) {
    // TEST
    // std::cout << "Begin GetEOParamsList\n";
    if (arg == nullptr) {
      call.nested.emplace_back(EOObjectType::EO_PLUG);
      return shift;
    }
    auto arg_type = arg->getType();
    size_t type_size = 0;
    if (arg_type->isPointerType()) {
      type_size = 8;
      // TEST
      // std::cout << "it is Pointer Type\n";
    } else {
      type_size = GetTypeSize(arg_type);
    }
    EOObject param{"write"};
    const string postfix = GetPostfix(arg_type);
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
    shift += type_size;
    call.nested.push_back(param);
    // TEST
    // std::cout << "End GetEOParamsList\n";
  }
  return shift;
}

EOObject GetEOReturnValue(const CallExpr *op) {
  const QualType ret_type = op->getType();
  const size_t type_size = GetTypeSize(ret_type);
  // TEST
  // std::cout << "Return type_size = " << type_size << "\n";
  const std::string postfix = GetTypeName(ret_type);
  if (postfix != "undefinedtype") {
    EOObject read_ret{"read"};
    const EOObject ret_val{"return"};
    read_ret.nested.push_back(ret_val);
    if (ret_type->isRecordType() || ret_type->isArrayType()) {
      read_ret.nested.emplace_back(to_string(type_size),
                                   EOObjectType::EO_LITERAL);
    } else {
      read_ret.name += "-as-" + postfix;
    }
    return read_ret;
  }
  return EOObject{"TRUE", EOObjectType::EO_LITERAL};
}

EOObject GetFunctionCallEOObject(const CallExpr *op) {
  EOObject call("seq");
  const vector<std::size_t> var_sizes;

  if (op == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  // TEST
  // std::cout << "NamArgs = " << op->getNumArgs() << "\n";
  const auto *func_decl = op->getDirectCallee();
  // ======= The function call =======
  if (func_decl != nullptr) {  // The direct function call generation
                               // TEST
                               // auto func_name{func_decl->getNameAsString()};
                               // std::cout << "It is Direct Function Call " <<
                               // func_name << "\n";

    const size_t shift = GetEOParamsList(op, call);
    call.nested.push_back(
        transpiler.func_manager_.GetFunctionCall(func_decl, shift));
    call.nested.push_back(GetEOReturnValue(op));
    return call;
  }
  // ======= The function call using pointer =======
  const auto *func_ptr_decl = op->getCalleeDecl();
  if (func_ptr_decl == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  if (func_ptr_decl->getKind() == clang::Decl::Var) {
    const auto *varDecl = clang::dyn_cast<clang::VarDecl>(func_ptr_decl);
    auto func_ptr_qualtype{varDecl->getType()};
    if (func_ptr_qualtype->isFunctionPointerType()) {
      auto pointee_type = func_ptr_qualtype->getPointeeType();
      if (pointee_type->isFunctionNoProtoType() ||
          pointee_type->isFunctionProtoType()) {
        const size_t shift = GetEOParamsList(op, call);
        EOObject call_ptr{"call", EOObjectType::EO_LITERAL};
        EOObject func_ptr_value{"read-as-ptr"};
        auto var{transpiler.glob_.GetVarById(varDecl)};
        func_ptr_value.nested.emplace_back(var.alias);
        call_ptr.nested.push_back(func_ptr_value);
        call_ptr.nested.emplace_back("empty-local-position");
        call_ptr.nested.emplace_back(to_string(shift),
                                     EOObjectType::EO_LITERAL);
        call.nested.push_back(call_ptr);
      }
    }
    call.nested.push_back(GetEOReturnValue(op));
    return call;
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetPrintfCallEOObject(const CallExpr *op) {
  EOObject printf{"printf"};
  int idx = 0;
  vector<string> formats;
  for (const auto *arg : op->arguments()) {
    auto param = GetStmtEOObject(arg);
    if (idx == 0 && param.type == EOObjectType::EO_LITERAL) {
      const std::regex re("%([lh]*)([cdfs])");
      auto formats_begin =
          std::sregex_iterator(param.name.begin(), param.name.end(), re);
      auto formats_end = std::sregex_iterator();
      for (std::sregex_iterator i = formats_begin; i != formats_end; ++i) {
        const char match_format = i->str(2)[0];
        switch (match_format) {
          case 's':
            formats.emplace_back("read-as-string");
            break;
          case 'c':
            formats.emplace_back("as-char");
            break;
          default:
            formats.emplace_back("");
            break;
        }
      }
      printf.nested.push_back(param);
    } else if (idx > 0 && idx <= formats.size() && !formats[idx - 1].empty()) {
      if (param.type != EOObjectType::EO_LITERAL) {
        EOObject cast{formats[idx - 1]};
        EOObject addr{"address"};
        const EOObject ram{"global-ram"};
        addr.nested.push_back(ram);
        addr.nested.push_back(param);
        cast.nested.push_back(addr);
        printf.nested.push_back(cast);
      } else if (formats[idx - 1] == "as-char") {
        EOObject cast{formats[idx - 1]};
        cast.nested.push_back(param);
        printf.nested.push_back(cast);
      } else {
        printf.nested.push_back(param);
      }
    } else {
      //      if (idx == 0) {
      //        op->dump();
      //      }
      printf.nested.push_back(param);
    }
    idx++;
  }
  while (idx <= formats.size()) {
    if (formats[idx - 1] == "as-char") {
      EOObject cast{formats[idx - 1]};
      cast.nested.emplace_back(to_string(0));
      printf.nested.push_back(cast);
    } else if (formats[idx - 1] == "read-as-string") {
      printf.nested.emplace_back("\"\"");
    } else {
      printf.nested.emplace_back(to_string(0));
    }
    idx++;
  }
  return printf;
}

EOObject GetFloatingLiteralEOObject(const FloatingLiteral *p_literal) {
  std::ostringstream ss{};
  if (p_literal != nullptr) {
    // TEST out
    std::cout << "Checkout 01\n";
    const llvm::APFloat an_float = p_literal->getValue();
    std::cout << "Checkout 02\n";
    if (&an_float.getSemantics() ==
        static_cast<const llvm::fltSemantics *>(&llvm::APFloat::IEEEdouble())) {
      ss << std::fixed << an_float.convertToDouble();
    } else {
      ss << std::fixed << an_float.convertToFloat();
    }
    std::cout << "Checkout 03\n";
  }
  return {ss.str(), EOObjectType::EO_LITERAL};
}

EOObject GetIntegerLiteralEOObject(const IntegerLiteral *p_literal) {
  if (p_literal != nullptr) {
    const bool is_signed = p_literal->getType()->isSignedIntegerType();
    const llvm::APInt an_int = p_literal->getValue();
    if (is_signed) {
      const int64_t val = an_int.getSExtValue();
      const std::string str_val{std::to_string(val)};
      return EOObject{str_val, EOObjectType::EO_LITERAL};
    }
    const uint64_t val = an_int.getZExtValue();
    if (val >= (1UL << 63)) {
      EOObject plus{"plus"};
      EOObject times{"times"};
      times.nested.emplace_back(std::to_string(val / (1UL << 32)));
      times.nested.emplace_back(std::to_string((1UL << 32)));
      plus.nested.push_back(times);
      plus.nested.emplace_back(std::to_string(val % (1UL << 32)));
    } else {
      const std::string str_val{std::to_string(val)};
      return EOObject{str_val, EOObjectType::EO_LITERAL};
    }
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetCompoundAssignEOObject(const CompoundAssignOperator *p_operator) {
  auto op_code = p_operator->getOpcode();
  std::string operation;

  auto *opd1 = p_operator->getLHS();
  auto *opd2 = p_operator->getRHS();
  auto eo_opd1 = GetStmtEOObject(opd1);
  auto eo_opd2 = GetStmtEOObject(opd2);
  auto qual_type1 = opd1->getType();

  if (op_code == clang::BinaryOperatorKind::BO_AddAssign ||
      op_code == clang::BinaryOperatorKind::BO_SubAssign) {
    operation =
        (op_code == clang::BinaryOperatorKind::BO_AddAssign) ? "plus" : "minus";
    // is 1st pointer or array?
    const clang::Type *type1 = qual_type1.getTypePtrOrNull();
    if (type1->isArrayType() || type1->isPointerType()) {
      // set size of pointer shift
      const uint64_t type_size = GetTypeSize(qual_type1);
      // TEST type size output
      // std::cout << "Size of type = " << type_size << "\n";
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      // second Operand must be integer expression else C-error
      EOObject mult{"times"};
      mult.nested.push_back(eo_opd2);
      mult.nested.push_back(value);
      EOObject read_op{"read-as-ptr"};
      read_op.nested.push_back(eo_opd1);
      EOObject binary_op{operation};
      binary_op.nested.push_back(read_op);
      binary_op.nested.push_back(mult);
      return binary_op;
    }
  } else if (op_code == clang::BinaryOperatorKind::BO_MulAssign) {
    operation = "times";
  } else if (op_code == clang::BinaryOperatorKind::BO_DivAssign) {
    operation = "div";
  } else if (op_code == clang::BinaryOperatorKind::BO_RemAssign) {
    operation = "mod";
  } else if (op_code == clang::BinaryOperatorKind::BO_AndAssign) {
    operation = "bit-and";
  } else if (op_code == clang::BinaryOperatorKind::BO_XorAssign) {
    operation = "bit-xor";
  } else if (op_code == clang::BinaryOperatorKind::BO_OrAssign) {
    operation = "bit-or";
  } else if (op_code == clang::BinaryOperatorKind::BO_ShlAssign) {
    operation = "shift-left";
  } else if (op_code == clang::BinaryOperatorKind::BO_ShrAssign) {
    operation = "shift-right";
  }

  EOObject binary_op{operation};
  EOObject eo_object{"read"};
  if (opd1 != nullptr) {
    eo_object.nested.push_back(eo_opd1);
    if (!qual_type1->isRecordType()) {
      eo_object.name += "-as-" + GetTypeName(qual_type1);
    } else {
      eo_object.nested.emplace_back(
          to_string(transpiler.record_manager_
                        .GetById(qual_type1->getAsRecordDecl()->getID())
                        ->size),
          EOObjectType::EO_LITERAL);
    }
  }
  binary_op.nested.emplace_back(eo_object);
  binary_op.nested.push_back(eo_opd2);
  //   binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
  return binary_op;
}

EOObject GetBinaryStmtEOObject(const BinaryOperator *p_operator) {
  std::string operation;
  if (p_operator == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  auto op_code = p_operator->getOpcode();
  if (op_code == clang::BinaryOperatorKind::BO_Assign) {
    return GetAssignmentOperatorEOObject(p_operator);
  }
  auto *opd1 = p_operator->getLHS();
  auto *opd2 = p_operator->getRHS();
  auto eo_opd1 = GetStmtEOObject(opd1);
  auto eo_opd2 = GetStmtEOObject(opd2);
  auto qual_type1 = opd1->getType();
  auto qual_type2 = opd2->getType();
  if (op_code == clang::BinaryOperatorKind::BO_Add) {
    operation = "plus";
    // is 1st pointer or array?
    const clang::Type *type1 = qual_type1.getTypePtrOrNull();
    if (type1->isArrayType() || type1->isPointerType()) {
      // set size of pointer shift
      const uint64_t type_size = GetTypeSize(qual_type1);
      // TEST type size output
      // std::cout << "Size of type = " << type_size << "\n";
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      // second Operand must be integer expression else C-error
      EOObject mult{"times"};
      mult.nested.push_back(eo_opd2);
      mult.nested.push_back(value);
      EOObject binary_op{operation};
      binary_op.nested.push_back(eo_opd1);
      binary_op.nested.push_back(mult);
      return binary_op;
    }
  } else if (op_code == clang::BinaryOperatorKind::BO_Sub) {
    operation = "minus";
    // is 1st pointer or array?
    const clang::Type *type1 = qual_type1.getTypePtrOrNull();
    const clang::Type *type2 = qual_type2.getTypePtrOrNull();
    if (type1->isArrayType() || type1->isPointerType()) {
      // set size of pointer shift
      const uint64_t type_size = GetTypeSize(qual_type1);
      // TEST type size output
      // std::cout << "Size of type = " << type_size << "\n";
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      // Second operand maybe pointer too
      if (type2->isArrayType() || type2->isPointerType()) {
        // uint64_t type_size2 = GetTypeSize(qual_type2);
        EOObject substr{"minus"};
        substr.nested.push_back(eo_opd1);
        substr.nested.push_back(eo_opd2);
        EOObject div{"div"};
        div.nested.push_back(substr);
        div.nested.push_back(value);
        return div;
      }
      // Or second Operand must be integer expression else C-error
      EOObject mult{"times"};
      mult.nested.push_back(eo_opd2);
      mult.nested.push_back(value);
      EOObject binary_op{operation};
      binary_op.nested.push_back(eo_opd1);
      binary_op.nested.push_back(mult);
      return binary_op;
    }
  } else if (op_code == clang::BinaryOperatorKind::BO_Mul) {
    operation = "times";
  } else if (op_code == clang::BinaryOperatorKind::BO_Div) {
    operation = "div";
  } else if (op_code == clang::BinaryOperatorKind::BO_Rem) {
    operation = "mod";
  } else if (op_code == clang::BinaryOperatorKind::BO_And) {
    operation = "bit-and";
  } else if (op_code == clang::BinaryOperatorKind::BO_Or) {
    operation = "bit-or";
  } else if (op_code == clang::BinaryOperatorKind::BO_Xor) {
    operation = "bit-xor";
  } else if (op_code == clang::BinaryOperatorKind::BO_LAnd) {
    operation = "and";
  } else if (op_code == clang::BinaryOperatorKind::BO_LOr) {
    operation = "or";
  } else if (op_code == clang::BinaryOperatorKind::BO_Shl) {
    operation = "shift-left";
  } else if (op_code == clang::BinaryOperatorKind::BO_Shr) {
    operation = "shift-right";
  } else if (op_code == clang::BinaryOperatorKind::BO_EQ) {
    operation = "eq";
  } else if (op_code == clang::BinaryOperatorKind::BO_NE) {
    operation = "neq";
  } else if (op_code == clang::BinaryOperatorKind::BO_LT) {
    operation = "lt";
  } else if (op_code == clang::BinaryOperatorKind::BO_LE) {
    operation = "lte";
  } else if (op_code == clang::BinaryOperatorKind::BO_GT) {
    operation = "gt";
  } else if (op_code == clang::BinaryOperatorKind::BO_GE) {
    operation = "gte";
  } else if (op_code == clang::BinaryOperatorKind::BO_Comma) {
    operation = "seq";
  } else {
    operation = "undefined";
    llvm::errs() << "Warning: Unknown operator " << p_operator->getOpcodeStr()
                 << "\n";
  }
  EOObject binary_op{operation};
  binary_op.nested.push_back(eo_opd1);
  binary_op.nested.push_back(eo_opd2);
  //   binary_op.nested.push_back(GetStmtEOObject(p_operator->getLHS()));
  //   binary_op.nested.push_back(GetStmtEOObject(p_operator->getRHS()));
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
      clang::UnaryOperatorKind::UO_PostInc) {  // UNARY_OPERATION(PostInc,
                                               // "++")
    const std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    const QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      const QualType arg_type = p_operator->getType();
      const uint64_t type_size = GetTypeSize(arg_type);
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_PostDec) {  // UNARY_OPERATION(PostDec,
                                               // "--")
    const std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"post-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    const QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      const QualType arg_type = p_operator->getType();
      const uint64_t type_size = GetTypeSize(arg_type);
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
    // [C99 6.5.3.1] Prefix increment and decrement
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_PreInc) {  // UNARY_OPERATION(PreInc, "++")
    const std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-inc-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    const QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      const QualType arg_type = p_operator->getType();
      const uint64_t type_size = GetTypeSize(arg_type);
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_PreDec) {  // UNARY_OPERATION(PreDec, "--")
    const std::string postfix = GetTypeName(p_operator->getType());
    EOObject variable{"pre-dec-" + postfix};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    const QualType result_type = p_operator->getType();
    if (result_type->isPointerType()) {
      const QualType arg_type = p_operator->getType();
      const uint64_t type_size = GetTypeSize(arg_type);
      const EOObject value{std::to_string(type_size), EOObjectType::EO_LITERAL};
      variable.nested.push_back(value);
    }
    return variable;
    // [C99 6.5.3.2] Address and indirection
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_AddrOf) {  // UNARY_OPERATION(AddrOf, "&")
    EOObject variable{"addr-of"};
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_Deref) {  // UNARY_OPERATION(Deref, "*")
    EOObject variable{"address"};
    const EOObject ram{"global-ram"};
    variable.nested.push_back(ram);
    variable.nested.push_back(GetStmtEOObject(p_operator->getSubExpr()));
    return variable;
    // [C99 6.5.3.3] Unary arithmetic
  }
  if (op_code ==
      clang::UnaryOperatorKind::UO_Plus) {  // UNARY_OPERATION(Plus, "+")
    operation = "pos";
  } else if (op_code ==
             clang::UnaryOperatorKind::UO_Minus) {  // UNARY_OPERATION(Minus,
                                                    // "-")
    operation = "neg";
  } else if (op_code ==
             clang::UnaryOperatorKind::UO_Not) {  // UNARY_OPERATION(Not, "~")
    operation = "bit-not";
  } else if (op_code ==
             clang::UnaryOperatorKind::UO_LNot) {  // UNARY_OPERATION(LNot,
                                                   // "!")
    operation = "not";
    // "__real expr"/"__imag expr" Extension.
  } else if (op_code ==
             clang::UnaryOperatorKind::UO_Real) {  // UNARY_OPERATION(Real,
                                                   // "__real")
    operation = "real";
  } else if (op_code ==
             clang::UnaryOperatorKind::UO_Imag) {  // UNARY_OPERATION(Imag,
                                                   // "__imag")
    operation = "imag";
    // __extension__ marker.
  } else if (op_code == clang::UnaryOperatorKind::
                            UO_Extension) {  // UNARY_OPERATION(Extension,
    // "__extension__")
    operation = "extension";
    // [C++ Coroutines] co_await operator
  } else if (op_code == clang::UnaryOperatorKind::
                            UO_Coawait) {  // UNARY_OPERATION(Coawait,
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

EOObject GetUnaryExprOrTypeTraitExprEOObject(
    const clang::UnaryExprOrTypeTraitExpr *p_expr) {
  if (p_expr == nullptr) {
    return EOObject{EOObjectType::EO_PLUG};
  }
  if (p_expr->isArgumentType()) {
    // Argument isTtype
    const QualType qual_type = p_expr->getTypeOfArgument();
    auto type_size = GetSizeOfType(qual_type);
    const std::string str_val{std::to_string(type_size)};
    return EOObject{str_val, EOObjectType::EO_LITERAL};
  }
  // Argument is Expr
  const auto *p_size_expr = p_expr->getArgumentExpr();
  const QualType expr_type = p_size_expr->getType();
  //   auto expr_type_size = GetTypeSize(expr_type);
  auto expr_type_size = GetSizeOfType(expr_type);
  const std::string str_val{std::to_string(expr_type_size)};
  return EOObject{str_val, EOObjectType::EO_LITERAL};
}

EOObject GetAssignmentOperatorEOObject(const BinaryOperator *p_operator) {
  EOObject binary_op{"write"};
  EOObject constData{"write"};
  Expr *left = dyn_cast<Expr>(p_operator->getLHS());
  if (left != nullptr) {
    const TypeSimpl typeInfo =
        transpiler.type_manger_.Add(left->getType().getTypePtr());
    //    QualType qual_type = left->getType();
    EOObject eoRight = GetStmtEOObject(p_operator->getRHS());
    if (typeInfo.name == "ptr" && eoRight.nested.empty()) {
      const TypeSimpl item_type =
          transpiler.type_manger_.GetById(typeInfo.subTypeId);
      if (item_type.name != "undefinedtype") {
        uint64_t type_size = 0;
        if (item_type.name == "int8") {
          constData.name += "-as-string";
          type_size = eoRight.name.length() - 1;
        } else {
          if (item_type.typeStyle != ComplexType::RECORD &&
              item_type.typeStyle != ComplexType::ARRAY) {
            constData.name += "-as-" + item_type.name;
          }
          type_size = item_type.GetSizeOfType();
        }
        {
          EOObject address{"address"};
          address.nested.emplace_back("global-ram");
          address.nested.emplace_back(
              to_string(transpiler.glob_.GetFreeSpacePointer()),
              EOObjectType::EO_LITERAL);
          transpiler.glob_.ShiftFreeSpacePointer(type_size);
          constData.nested.push_back(address);
          constData.nested.push_back(eoRight);
          eoRight = EOObject{"addr-of"};
          eoRight.nested.push_back(address);
        }
      }
    }
    if (typeInfo.typeStyle != ComplexType::RECORD &&
        !(typeInfo.typeStyle == ComplexType::ARRAY &&
          typeInfo.name != "string") &&
        typeInfo.name != "undefinedtype") {
      binary_op.name += "-as-" + typeInfo.name;
    }
    binary_op.nested.emplace_back(GetStmtEOObject(left));
    binary_op.nested.push_back(eoRight);
    if (constData.nested.empty()) {
      return binary_op;
    }
    EOObject res{EOObjectType::EO_EMPTY};
    res.nested.push_back(constData);
    res.nested.push_back(binary_op);
    return res;
  }
  return EOObject{EOObjectType::EO_PLUG};
}

EOObject GetEODeclRefExpr(const DeclRefExpr *op) {
  if (op == nullptr) {
    return EOObject{EOObjectType::EO_EMPTY};
  }
  try {
    const auto *val = op->getFoundDecl();
    auto decl_kind = val->getKind();
    if (decl_kind == clang::Decl::EnumConstant) {
      // TEST Out
      // std::cout << "it is EnumConstant\n";
      const auto *id = dyn_cast<EnumConstantDecl>(val);
      const auto &var = transpiler.enum_manager_.GetConstantById(id);
      return EOObject{std::to_string(var->value), EOObjectType::EO_LITERAL};
    }
    if (decl_kind == clang::Decl::Function) {
      // std::cout << "it is Decl::Function\n";
      const auto *id = dyn_cast<FunctionDecl>(val);
      std::string function_name =
          transpiler.func_manager_.GetEOFunctionName(id);
      // std::cout << "function_name = " << function_name << "\n";
      auto func_index = transpiler.func_manager_.GetMapIndex(&function_name);
      return EOObject{std::to_string(func_index), EOObjectType::EO_LITERAL};
    }
    const auto *id = dyn_cast<VarDecl>(val);
    const auto &var = transpiler.glob_.GetVarById(id);
    const clang::QualType qual_type = id->getType();
    const clang::Type *type = qual_type.getTypePtrOrNull();
    //    TypeSimpl typeInfo =
    //        transpiler.type_manger_.Add(id->getType().getTypePtrOrNull());
    if (type == nullptr) {
      return EOObject{EOObjectType::EO_PLUG};
    }
    //    if (type->isPointerType()) {
    //      EOObject globAddr{"address"};
    //      globAddr.nested.emplace_back("global-ram");
    //      globAddr.nested.emplace_back(var.alias);
    //      return globAddr;
    //    }
    if (type->isArrayType()) {
      EOObject array_as_ptr{"addr-of"};
      array_as_ptr.nested.emplace_back(var.alias);
      return array_as_ptr;
    }
    if (type->isFunctionPointerType()) {
      // TEST
      // std::cout << "It is Function Pointer Type\n";
      return EOObject{var.alias};
    }
    if (type->isFunctionType()) {
      // TEST
      // std::cout << "It is Function Type\n";
      return EOObject{EOObjectType::EO_PLUG};
      //       return EOObject{var.alias};
    }
    EOObject other_object{var.alias};
    return other_object;
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
  const QualType qual_type = left->getType();
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
    const string postfix = GetPostfix(ret_value->getType());
    if (!postfix.empty()) {
      ret.name += "-as-" + postfix;
    }
    const EOObject address{"return"};
    ret.nested.push_back(address);
    ret.nested.push_back(GetStmtEOObject(ret_value));
    result.nested.push_back(ret);
  }
  const EOObject label{"goto-return-label.forward TRUE",
                       EOObjectType::EO_LITERAL};
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

EOObject GetConditionalStmtEOObject(const clang::ConditionalOperator *p_stmt) {
  EOObject if_else_stmt{"if-else"};
  if_else_stmt.nested.push_back(GetStmtEOObject(p_stmt->getCond()));
  if_else_stmt.nested.push_back(GetStmtEOObject(p_stmt->getTrueExpr()));
  if_else_stmt.nested.push_back(GetStmtEOObject(p_stmt->getFalseExpr()));
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
  const string label_name = "while-loop-label";
  return_label.arguments.emplace_back(label_name);
  const EOObject continue_obj{label_name + ".backward", "continue"};
  const EOObject break_obj{label_name + ".forward TRUE", "break"};
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
  const EOObject break_obj{"do-while-loop-label-1.forward TRUE", "break"};

  EOObject seq_object_1{"seq", "@"};

  EOObject goto_object_2{"goto"};
  EOObject return_label_2{EOObjectType::EO_ABSTRACT};
  return_label_2.arguments.emplace_back("do-while-loop-label-2");
  const EOObject continue_obj{"do-while-loop-label-2.forward TRUE", "continue"};
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
  if (type_ptr == nullptr) {
    // !!std::cout << "Incorrect Type Pointer\n";
    return 0;
  }
  const TypeInfo type_info = context->getTypeInfo(type_ptr);
  const uint64_t type_size = type_info.Width;

  if (type_ptr->isPointerType()) {
    const clang::Type *pointer_type = type_ptr->getPointeeType().getTypePtr();
    const TypeInfo pointer_type_info = context->getTypeInfo(pointer_type);
    const uint64_t pointer_type_size = pointer_type_info.Width;
    return pointer_type_size / byte_size;
  }
  return type_size / byte_size;
}

uint64_t GetSizeOfType(QualType qual_type) {
  const clang::Type *type_ptr = qual_type.getTypePtr();
  const TypeInfo type_info = context->getTypeInfo(type_ptr);
  const uint64_t type_size = type_info.Width;

  if (type_ptr->isPointerType()) {
    return 8;  // Size of any pointer == 8 byte
  }
  return type_size / byte_size;
}

std::string GetPostfix(QualType qual_type) {
  const clang::Type *type_ptr = qual_type.getTypePtr();
  const TypeInfo type_info = context->getTypeInfo(type_ptr);
  const uint64_t type_size = type_info.Width;
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

  if (type_ptr->isIntegerType()) {
    str += "int" + std::to_string(type_size);
    return str;
  }
  return "";
}

std::string GetTypeName(QualType qual_type) {
  const clang::Type *type_ptr = qual_type.getTypePtr();
  const TypeInfo type_info = context->getTypeInfo(type_ptr);
  const uint64_t type_size = type_info.Width;
  std::string str;

  if (type_ptr->isBooleanType()) {
    str += "bool";
    return str;
  }

  if (type_ptr->isPointerType()) {
    const auto *const ptr_type = dyn_cast<clang::PointerType>(type_ptr);
    if (ptr_type != nullptr && ptr_type->getPointeeType()->isCharType()) {
      str += "string";
    } else {
      str += "ptr";
    }
    return str;
  }

  if (type_ptr->isFloatingType()) {
    str += "float" + std::to_string(type_size);
    return str;
  }

  if (!type_ptr->isSignedIntegerType()) {
    str += "u";
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
    // TEST
    // std::cout << "not visited: " << child.postfix << "\n";
    not_visited.push(std::move(child));
  }
  while (!not_visited.empty()) {
    const EOObject cur = not_visited.front();
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
    // TEST
    // std::cout << "erase: " << known_obj << "\n";
    unknown.erase(known_obj);
  }

  return unknown;
}
