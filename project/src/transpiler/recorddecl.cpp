#include "stmt.h"
#include "generator.h"
#include "recorddecl.h"

void getRecordDeclSubObjects(const RecordDecl* RD) {
    llvm::outs() << "\x1B[33m";

    if (RD->isUnion())
        llvm::outs() << "Union \n";
    if (RD->isStruct())
        llvm::outs() << "Struct \n";

    std::string recordName = "anonymous";
    if (RD->hasNameForLinkage())
        recordName = RD->getNameAsString();
    llvm::outs() << "  name - " << recordName << "\n";

    if (RD->field_empty())
        llvm::outs() << "  is Empty \n";

    for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
        llvm::outs() << "    field - " << it->getNameAsString() << "\n";
        llvm::outs() << "      index - " << it->getFieldIndex() << "\n";
        llvm::outs() << "      is unnamed bit field - " << it->isUnnamedBitfield() << "\n";
        //llvm::outs() << "      isAnonymousStructOrUnion - " << it->isAnonymousStructOrUnion() << "\n";
        llvm::outs() << "      field kind name: " << it->getDeclKindName() << "\n";
        //llvm::outs() << "      field id: " << reinterpret_cast<uint64_t>(it) << "\n";
        TypeInfo typeInfo = it->getASTContext().getTypeInfo(it->getType());
        uint64_t typeSize = typeInfo.Width;
        unsigned fieldAlign = typeInfo.Align;
        llvm::outs() << "      size - " << typeInfo.Width << "\n";
        llvm::outs() << "      align - " << typeInfo.Align << "\n";
        QualType qualType = it->getType();
        const IdentifierInfo *typeId = qualType.getBaseTypeIdentifier();
        if (typeId != nullptr) {
            llvm::outs() << "      Type is " << typeId->getName() << "\n";
        } else {
            llvm::outs() << "      The problem to identify type name\n";
        }
    }


    llvm::outs() << "\033[0m";
}