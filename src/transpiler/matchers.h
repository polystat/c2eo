/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_MATCHERS_H_
#define SRC_TRANSPILER_MATCHERS_H_

#include "src/transpiler/analyzers.h"

void AddMatchers(clang::ast_matchers::MatchFinder &finder);

#endif  // SRC_TRANSPILER_MATCHERS_H_
