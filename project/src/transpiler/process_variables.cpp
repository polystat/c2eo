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

#include "src/transpiler/process_variables.h"

#include <vector>

#include "src/transpiler/vardecl.h"

using clang::CaseStmt;
using clang::CompoundStmt;
using clang::Decl;
using clang::DeclStmt;
using clang::DefaultStmt;
using clang::DoStmt;
using clang::ForStmt;
using clang::IfStmt;
using clang::Stmt;
using clang::SwitchStmt;
using clang::VarDecl;
using clang::WhileStmt;
using llvm::dyn_cast;
using std::vector;

extern UnitTranspiler transpiler;

void ProcessDeclStmt(size_t shift, vector<Variable> &all_local,
                     DeclStmt *decl_stmt);

void ProcessForStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                  ForStmt *for_stmt);
void ProcessWhileStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                    WhileStmt *while_stmt);
void ProcessCaseStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                   CaseStmt *case_stmt);
void ProcessIfStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                 IfStmt *if_stmt);
void ProcessDefaultStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                      DefaultStmt *default_stmt);
void ProcessSwitchStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                     SwitchStmt *switch_stmt);
void ProcessDoStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                 DoStmt *do_stmt);
void ProcessStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                               Stmt *stmt) {
  Stmt::StmtClass stmt_class = stmt->getStmtClass();
  if (stmt_class == Stmt::DeclStmtClass) {
    auto *decl_stmt = dyn_cast<DeclStmt>(stmt);
    ProcessDeclStmt(shift, all_local, decl_stmt);
  } else if (stmt_class == Stmt::ForStmtClass) {
    auto *for_stmt = dyn_cast<ForStmt>(stmt);
    ProcessForStmtLocalVariables(all_local, shift, for_stmt);
  } else if (stmt_class == Stmt::CompoundStmtClass) {
    auto *compound_stmt = dyn_cast<CompoundStmt>(stmt);
    ProcessFunctionLocalVariables(compound_stmt, all_local, shift);
  } else if (stmt_class == Stmt::WhileStmtClass) {
    auto *while_stmt = dyn_cast<WhileStmt>(stmt);
    ProcessWhileStmtLocalVariables(all_local, shift, while_stmt);
  } else if (stmt_class == Stmt::SwitchStmtClass) {
    auto *switch_stmt = dyn_cast<SwitchStmt>(stmt);
    ProcessSwitchStmtLocalVariables(all_local, shift, switch_stmt);
  } else if (stmt_class == Stmt::DoStmtClass) {
    auto *do_stmt = dyn_cast<DoStmt>(stmt);
    ProcessDoStmtLocalVariables(all_local, shift, do_stmt);
  } else if (stmt_class == Stmt::CaseStmtClass) {
    auto *case_stmt = dyn_cast<CaseStmt>(stmt);
    ProcessCaseStmtLocalVariables(all_local, shift, case_stmt);
  } else if (stmt_class == Stmt::DefaultStmtClass) {
    auto *default_stmt = dyn_cast<DefaultStmt>(stmt);
    ProcessDefaultStmtLocalVariables(all_local, shift, default_stmt);
  } else if (stmt_class == Stmt::IfStmtClass) {
    auto *if_stmt = dyn_cast<IfStmt>(stmt);
    ProcessIfStmtLocalVariables(all_local, shift, if_stmt);
  }
}

void ProcessFunctionLocalVariables(const clang::CompoundStmt *CS,
                                   std::vector<Variable> &all_local,
                                   size_t shift) {
  if (CS == nullptr) {
    return;
  }
  for (auto *stmt : CS->body()) {
    ProcessStmtLocalVariables(all_local, shift, stmt);
  }
}
void ProcessDoStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                 DoStmt *do_stmt) {
  if (do_stmt == nullptr) {
    return;
  }
  if (do_stmt->getBody() != nullptr &&
      do_stmt->getBody()->getStmtClass() == Stmt::CompoundStmtClass) {
    auto *compound_stmt = dyn_cast<CompoundStmt>(do_stmt->getBody());
    ProcessFunctionLocalVariables(compound_stmt, all_local, shift);
  }
}
void ProcessSwitchStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                     SwitchStmt *switch_stmt) {
  if (switch_stmt == nullptr) {
    return;
  }
  if (switch_stmt->getBody() != nullptr &&
      switch_stmt->getBody()->getStmtClass() == Stmt::CompoundStmtClass) {
    auto *compound_stmt = dyn_cast<CompoundStmt>(switch_stmt->getBody());
    ProcessFunctionLocalVariables(compound_stmt, all_local, shift);
  }
}
void ProcessDefaultStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                      DefaultStmt *default_stmt) {
  if (default_stmt == nullptr) {
    return;
  }
  if (default_stmt->getSubStmt() != nullptr &&
      default_stmt->getSubStmt()->getStmtClass() == Stmt::CompoundStmtClass) {
    auto *compound_stmt = dyn_cast<CompoundStmt>(default_stmt->getSubStmt());
    ProcessFunctionLocalVariables(compound_stmt, all_local, shift);
  } else if (default_stmt->getSubStmt() != nullptr &&
             default_stmt->getSubStmt()->getStmtClass() ==
                 Stmt::DeclStmtClass) {
    auto *decl_stmt = dyn_cast<DeclStmt>(default_stmt->getSubStmt());
    ProcessDeclStmt(shift, all_local, decl_stmt);
  }
}
void ProcessIfStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                 IfStmt *if_stmt) {
  if (if_stmt == nullptr) {
    return;
  }
  if (if_stmt->getThen() != nullptr) {
    ProcessStmtLocalVariables(all_local, shift, if_stmt->getThen());
  }
  if (if_stmt->getElse() != nullptr) {
    ProcessStmtLocalVariables(all_local, shift, if_stmt->getElse());
  }
}
void ProcessCaseStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                   CaseStmt *case_stmt) {
  if (case_stmt == nullptr || case_stmt->getSubStmt() == nullptr) {
    return;
  }
  ProcessStmtLocalVariables(all_local, shift, case_stmt->getSubStmt());
}
void ProcessWhileStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                    WhileStmt *while_stmt) {
  if (while_stmt == nullptr || while_stmt->getBody() == nullptr) {
    return;
  }
  ProcessStmtLocalVariables(all_local, shift, while_stmt->getBody());
}
void ProcessForStmtLocalVariables(vector<Variable> &all_local, size_t shift,
                                  ForStmt *for_stmt) {
  if (for_stmt == nullptr) {
    return;
  }
  if (for_stmt->getInit() != nullptr &&
      for_stmt->getInit()->getStmtClass() == Stmt::DeclStmtClass) {
    auto *decl_stmt = dyn_cast<DeclStmt>(for_stmt->getInit());
    ProcessDeclStmt(shift, all_local, decl_stmt);
  }
  if (for_stmt->getBody() != nullptr) {
    ProcessStmtLocalVariables(all_local, shift, for_stmt->getBody());
  }
}

void ProcessCompoundStatementLocalVariables(const clang::CompoundStmt *CS,
                                            vector<Variable> &all_local) {
  if (CS == nullptr) {
    return;
  }
  for (auto *stmt : CS->body()) {
    Stmt::StmtClass stmt_class = stmt->getStmtClass();
    if (stmt_class == Stmt::DeclStmtClass) {
      auto *decl_stmt = dyn_cast<DeclStmt>(stmt);
      if (decl_stmt == nullptr) {
        continue;
      }
      for (auto *decl : decl_stmt->decls()) {
        Decl::Kind decl_kind = decl->getKind();
        if (decl_kind == Decl::Var) {
          auto *var_decl = dyn_cast<VarDecl>(decl);
          if (var_decl != nullptr) {
            all_local.push_back(transpiler.glob_.GetVarById(var_decl));
          }
        }
      }
    } else if (stmt_class == Stmt::ForStmtClass) {
      auto *for_stmt = dyn_cast<ForStmt>(stmt);
      if (for_stmt == nullptr) {
        continue;
      }
      if (for_stmt->getInit() != nullptr &&
          for_stmt->getInit()->getStmtClass() == Stmt::DeclStmtClass) {
        auto *decl_stmt = dyn_cast<DeclStmt>(for_stmt->getInit());
        if (decl_stmt == nullptr) {
          continue;
        }
        for (auto *decl : decl_stmt->decls()) {
          Decl::Kind decl_kind = decl->getKind();
          if (decl_kind == Decl::Var) {
            auto *var_decl = dyn_cast<VarDecl>(decl);
            if (var_decl != nullptr) {
              all_local.push_back(transpiler.glob_.GetVarById(var_decl));
            }
          }
        }
      }
    }
  }
}

void ProcessDeclStmt(size_t shift, vector<Variable> &all_local,
                     DeclStmt *decl_stmt) {
  if (decl_stmt == nullptr) {
    return;
  }
  for (auto *decl : decl_stmt->decls()) {
    Decl::Kind decl_kind = decl->getKind();
    if (decl_kind == Decl::Var) {
      auto *var_decl = dyn_cast<VarDecl>(decl);
      if (var_decl != nullptr) {
        all_local.push_back(ProcessVariable(var_decl, "local-start", shift));
      }
    }
  }
}
