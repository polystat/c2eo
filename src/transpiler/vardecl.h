/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_VARDECL_H_
#define SRC_TRANSPILER_VARDECL_H_

#include <string>

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "src/transpiler/memory_manager.h"

const int byte_size = 8;
const int double_size = 64;
const int float_size = 32;

Variable ProcessVariable(const clang::VarDecl *VD,
                         const std::string &local_name = "", size_t shift = 0);

#endif  // SRC_TRANSPILER_VARDECL_H_
