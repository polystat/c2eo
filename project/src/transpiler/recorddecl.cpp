#include "recorddecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"

RecordType ProcessRecordType(const clang::RecordDecl* RD) {
  extern UnitTranspiler transpiler;
//  std::cout<<"RD:"<<RD<<'\n';
//  RD->dump();

  std::string name;
  if (RD->isUnion())
    name = "un_";
  if (RD->isStruct())
    name = "st_";
  if (RD->hasNameForLinkage())
    name += RD->getNameAsString();
  else
    name += std::to_string(reinterpret_cast<uint64_t>(RD));

  size_t size = 0;

  std::map<std::string, size_t> fields;
  size_t shift = 0;
  for (auto it = RD->field_begin(); it != RD->field_end(); it++) {
    std::string fieldName;
    if (!it->isUnnamedBitfield())
      fieldName = /* "f_" + */ it->getNameAsString();
    else
      fieldName = "field" + std::to_string(fields.size());
    fields[fieldName] = shift;

    clang::QualType qualType = it->getType();
    clang::TypeInfo typeInfo = it->getASTContext().getTypeInfo(qualType);
    if (RD->isStruct()) {
      shift += typeInfo.Width / 8;
      size = shift;
    } else
      size = std::max(size, typeInfo.Width / 8);
  }
  return transpiler.record_manager.Add(RD, name, size, fields);
}
