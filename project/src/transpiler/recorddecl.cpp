#ifndef RECORD_DECL_INFO
#define RECORD_DECL_INFO
#endif

#include "stmt.h"
#include "generator.h"
#include "recorddecl.h"

void getRecordDeclSubObjects(const RecordDecl* RD) {
    RecordGen* RG = new RecordGen;
#ifdef RECORD_DECL_INFO
    llvm::outs() << "\x1B[33m";
#endif

    if (RD->isUnion()){
#ifdef RECORD_DECL_INFO
        llvm::outs() << "Union \n";
#endif
        RG->type = "union";
    }
    if (RD->isStruct()) {
#ifdef RECORD_DECL_INFO
        llvm::outs() << "Struct \n";
#endif
        RG->type = "struct";
    }

    RG->name = "anonymous";
    if (RD->hasNameForLinkage())
        RG->name = RD->getNameAsString();
#ifdef RECORD_DECL_INFO
    llvm::outs() << "  name - " << RG->name << "\n";
#endif

    if (RD->field_empty())
#ifdef RECORD_DECL_INFO
        llvm::outs() << "  is Empty \n";
#endif

    for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
        TypeInfo typeInfo = it->getASTContext().getTypeInfo(it->getType());
        uint64_t typeSize = typeInfo.Width;
        unsigned fieldAlign = typeInfo.Align;
        const clang::QualType qt = it->getType();
        VarGen* VG = new VarGen;
        if (!it->isUnnamedBitfield())
            VG->name = it->getNameAsString();
        else
            VG->name = "field"+std::to_string(RG->fields.size());
        VG->type = "c_" + qt.getAsString() + std::to_string(typeInfo.Width);
        VG->value = "0";
        RG->fields.push_back(VG);

#ifdef RECORD_DECL_INFO
        llvm::outs() << "    field\n";
        llvm::outs() << "      name - " << it->getNameAsString() << "\n";
        llvm::outs() << "      index - " << it->getFieldIndex() << "\n";
        llvm::outs() << "      is unnamed bit field - " << it->isUnnamedBitfield() << "\n";
        //llvm::outs() << "      isAnonymousStructOrUnion - " << it->isAnonymousStructOrUnion() << "\n";
        llvm::outs() << "      field kind name: " << it->getDeclKindName() << "\n";
        //llvm::outs() << "      field id: " << reinterpret_cast<uint64_t>(it) << "\n";
        llvm::outs() << "      size - " << typeInfo.Width << "\n";
        llvm::outs() << "      align - " << typeInfo.Align << "\n";
        llvm::outs() << "      type - " << qt.getAsString() << "\n";
#endif
    }
    RG->staticSpaceGenPtr->Add(RG);
#ifdef RECORD_DECL_INFO
    llvm::outs() << "\033[0m";
#endif
}