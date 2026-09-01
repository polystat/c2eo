/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_ENUMDECL_H_
#define SRC_TRANSPILER_ENUMDECL_H_

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "src/transpiler/enum_manager.h"

EnumType ProcessEnumDecl(const clang::EnumDecl *ED);

#endif  // SRC_TRANSPILER_ENUMDECL_H_
