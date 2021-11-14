#ifndef __TYPE__
#define __TYPE__

#include <string>

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"

using namespace clang;

std::string getTypeName(const clang::ValueDecl* VD);

#endif // __TYPE__
