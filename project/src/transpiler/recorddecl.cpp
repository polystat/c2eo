#include "recorddecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"

std::vector<RecordType> ProcessRecordType(const clang::RecordDecl* RD, bool is_local) {
  extern UnitTranspiler transpiler;
  std::vector<RecordType> types;
  RecordType* existed = transpiler.record_manager.getById(RD);
  if (existed)
    return {};

  std::string name;
  if (RD->isUnion())
    name = "un-";
  if (RD->isStruct())
    name = "st-";
  if (RD->hasNameForLinkage())
    name += RD->getNameAsString();
  else
    name += std::to_string(reinterpret_cast<uint64_t>(RD));

  size_t size = 0;

  std::map<std::string, size_t> fields;
  size_t shift = 0;

  for (auto it = RD->decls_begin(); it != RD->decls_end(); it++) {
    if (it->getKind() == clang::Decl::Record) {
      auto new_types = ProcessRecordType(llvm::dyn_cast<clang::RecordDecl>(*it), is_local);
      types.insert(types.end(), new_types.begin(), new_types.end());
    } else if(it->getKind() == clang::Decl::Field) {
      auto field = llvm::dyn_cast<clang::FieldDecl>(*it);

      std::string fieldName;
      if (!field->isUnnamedBitfield())
        fieldName = /* "f-" + */ field->getNameAsString();
      else
        fieldName = "field" + std::to_string(fields.size());
      fields[fieldName] = shift;


      clang::QualType qualType = field->getType();
      clang::TypeInfo typeInfo = field->getASTContext().getTypeInfo(qualType);
      if (RD->isStruct()) {
        shift += typeInfo.Width / 8;
        size = shift;
      } else
        size = std::max(size, typeInfo.Width / 8);
    }
  }
  types.push_back(transpiler.record_manager.Add(RD, name, size, fields, is_local));
  return types;
}
