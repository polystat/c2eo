/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_TRANSPILE_HELPER_H_
#define SRC_TRANSPILER_TRANSPILE_HELPER_H_

// Declares clang::SyntaxOnlyAction.
#include <cstdint>
#include <set>
#include <string>

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/APSInt.h"
#include "llvm/Support/CommandLine.h"
#include "src/transpiler/eo_object.h"

int64_t GetInt64Value(const llvm::APSInt &value);

std::string GetTypeName(clang::QualType qual_type);

std::string GetPostfix(clang::QualType qual_type);

EOObject GetFunctionBody(const clang::FunctionDecl *FD);

EOObject GetCompoundStmt(const clang::CompoundStmt *CS, bool is_decorator);

EOObject GetStmtEOObject(const clang::Stmt *p_stmt);

EOObject ReplaceEmpty(const EOObject &eoObject, const EOObject &alias);

std::set<std::string> FindAllExternalObjects(const EOObject &obj);

#endif  // SRC_TRANSPILER_TRANSPILE_HELPER_H_
