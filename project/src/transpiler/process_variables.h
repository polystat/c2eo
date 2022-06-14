#ifndef C2EO__PROCESS_VARIABLES_H_
#define C2EO__PROCESS_VARIABLES_H_
#include "memory_manager.h"
#include "unit_transpiler.h"

void ProcessFunctionLocalVariables(const clang::CompoundStmt* CS,
                                   std::vector<Variable>& all_local,
                                   size_t shift);

void ProcessCompoundStatementLocalVariables(const clang::CompoundStmt *CS,
                                            std::vector<Variable> &all_local);
#endif  // C2EO__PROCESS_VARIABLES_H_
