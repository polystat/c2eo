#include "recorddecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"
#include "vardecl.h"

std::vector<RecordType> ProcessRecordType(const clang::RecordDecl *RD, bool is_local) {
  extern UnitTranspiler transpiler;
  std::vector<RecordType> types;
  RecordType *existed = transpiler.record_manager_.GetById(RD->getID());
  if (existed != nullptr) {
    return {};
  }

  std::string name;
  if (RD->isUnion()) {
    name = "un-";
  }
  if (RD->isStruct()) {
    name = "st-";
  }
  if (RD->hasNameForLinkage()) {
    name += RD->getNameAsString();
  } else {
    name += std::to_string(reinterpret_cast<uint64_t>(RD)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  }

  size_t size = 0;

  std::map<std::string, size_t> fields;
  size_t shift = 0;

  for (auto it = RD->decls_begin(); it != RD->decls_end(); it++) {
    if (it->getKind() == clang::Decl::Record) {
      auto new_types = ProcessRecordType(llvm::dyn_cast<clang::RecordDecl>(*it), is_local);
      types.insert(types.end(), new_types.begin(), new_types.end());
    } else if (it->getKind() == clang::Decl::Field) {
      auto *field = llvm::dyn_cast<clang::FieldDecl>(*it);

      std::string field_name;
      if (!field->isUnnamedBitfield()) {
        field_name = /* "f-" + */ field->getNameAsString();
      } else {
        field_name = "field" + std::to_string(fields.size());
      }
      fields[field_name] = shift;

      clang::QualType qual_type = field->getType();
      clang::TypeInfo type_info = field->getASTContext().getTypeInfo(qual_type);
      if (RD->isStruct()) {
        shift += type_info.Width / byte_size;
        size = shift;
      } else {
        size = std::max(size, type_info.Width / byte_size);
      }
    }
  }
  types.push_back(transpiler.record_manager_.Add(RD->getID(), name, size, fields, is_local));
  return types;
}
