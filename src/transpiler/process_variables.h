/*
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 c2eo team
 * SPDX-License-Identifier: MIT
 */

#ifndef SRC_TRANSPILER_PROCESS_VARIABLES_H_
#define SRC_TRANSPILER_PROCESS_VARIABLES_H_
#include <vector>

#include "src/transpiler/memory_manager.h"
#include "src/transpiler/unit_transpiler.h"

void ProcessFunctionLocalVariables(const clang::CompoundStmt* CS,
                                   std::vector<Variable>& all_local,
                                   size_t shift, bool process_only_static);

void ProcessCompoundStatementLocalVariables(const clang::CompoundStmt* CS,
                                            std::vector<Variable>& all_local);
#endif  // SRC_TRANSPILER_PROCESS_VARIABLES_H_
