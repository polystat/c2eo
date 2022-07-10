#include "enumdecl.h"

#include <algorithm>

#include "transpile_helper.h"
#include "unit_transpiler.h"
#include "vardecl.h"

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
  //
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
