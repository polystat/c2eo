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

#include "src/transpiler/enumdecl.h"

#include <algorithm>
#include <string>
#include <vector>

#include "src/transpiler/transpile_helper.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/vardecl.h"

EnumType ProcessEnumDecl(const clang::EnumDecl *ED) {
  if (ED == nullptr) {
    return {};
  }

  extern UnitTranspiler transpiler;
  //    if (transpiler.enum_manager_.GetById(ED)) {
  //        return {};
  //    }

  std::vector<EnumConstantType> constants;
  std::string enum_name = "en-" + ED->getNameAsString();
  uint64_t size = 0;

  for (auto decl = ED->decls_begin(); decl != ED->decls_end(); decl++) {
    if (decl->getKind() == clang::Decl::EnumConstant) {
      auto *const enum_const_decl =
          llvm::dyn_cast<clang::EnumConstantDecl>(*decl);
      if (enum_const_decl == nullptr) {
        return {};
      }
      clang::QualType qual_type = enum_const_decl->getType();
      clang::TypeInfo type_info =
          enum_const_decl->getASTContext().getTypeInfo(qual_type);
      size = type_info.Width / byte_size;
      std::string name = "c-" + enum_const_decl->getNameAsString();
      int64_t value = enum_const_decl->getInitVal().getExtValue();
      constants.push_back(EnumConstantType{enum_const_decl, name, value});
    }
  }
  return transpiler.enum_manager_.Add(ED, enum_name, size, constants);
}
