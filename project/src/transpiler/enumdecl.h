//
// Created by Андрей on 09.07.2022.
//

#ifndef C2EO_ENUMDECL_H
#define C2EO_ENUMDECL_H

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "enum_manager.h"

EnumType ProcessEnumDecl(const clang::EnumDecl *ED);

#endif //C2EO_ENUMDECL_H
