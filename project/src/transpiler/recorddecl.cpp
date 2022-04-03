#include "recorddecl.h"
#include "unit_transpiler.h"
#include "transpile_helper.h"

RecordType ProcessRecordType(const clang::RecordDecl* RD){
  extern UnitTranspiler transpiler;
  RecordType* rt = transpiler.record_manager.getById(RD);
  if(rt) return *rt;
  size_t size = 0;
  std::string type = "c_";
  std::string name;
  if (RD->isUnion()) {
    type += "union";
    name = "un_";
  }
  if (RD->isStruct()) {
    type += "struct";
    name = "st_";
  }
  if (RD->hasNameForLinkage())
    name += RD->getNameAsString();
  else
    name += std::to_string(reinterpret_cast<uint64_t>(RD));
  std::vector<RecordType> fields;
  for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
    RecordType field;
    clang::QualType qualType = it->getType();
    field.type = "c_" + GetTypeName(it->getType());
    if (!it->isUnnamedBitfield())
      field.name = "f_" + it->getNameAsString();
    else
      field.name = "field" + std::to_string(fields.size());
    field.size = 0;
    const clang::Type* typePtr = it->getType().getTypePtr();
    if (typePtr->isRecordType()) {
      field.size = ProcessRecordType(typePtr->getAsRecordDecl()).size;
    } else {
      clang::TypeInfo typeInfo = it->getASTContext().getTypeInfo(qualType);
      field.size = typeInfo.Width / 8;
    }
    size += field.size;
  }
  return transpiler.record_manager.Add(RD, size, type, name, fields);
}