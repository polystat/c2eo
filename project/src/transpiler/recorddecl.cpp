#include "stmt.h"
#include "recorddecl.h"
#include "vardecl.h"

RecordGen* getRecordDeclSubObjects(const RecordDecl* RD) {
    RecordGen* RG = new RecordGen;
    if (RD->isUnion()) {
        RG->type = "union";
        RG->name = "un_";
    }
    if (RD->isStruct()) {
        RG->type = "struct";
        RG->name = "st_";
    }
    if (RD->hasNameForLinkage())
        RG->name += RD->getNameAsString();
    else
        RG->name += std::to_string(reinterpret_cast<uint64_t>(RD));
    setSubFields(RG, RD);

    RG->globalSpaceGenPtr->Add(RG);
    return RG;
}

void setSubFields(RecordGen* RG, const RecordDecl* RD) {
//    for (auto it = RD->decls_begin(); it != RD->decls_end(); it++) {
//        if ((*it)->getKind() == Decl::Record){
//            (*it)->dump();
//            llvm::outs() << "😄\n\n\n";
//        }
//    }


    RG->count = 0;
    for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
//        (*it)->dump();
//        llvm::outs() << (*it)->getKind() << '\n';
//        if ((*it)->getKind() == Decl::Record)
//            llvm::outs() << "hm...))\n";
        std::string strType = "";
        getTypeName(*it, strType);
        RecordGen* VG = new RecordGen;
        if (!it->isUnnamedBitfield())
            VG->name = "f_" + it->getNameAsString();
        else
            VG->name = "field" + std::to_string(RG->fields.size());
        //VG->name += "_" + std::to_string(it->getFieldIndex());
        VG->type = strType;
        const clang::Type* typePtr = it->getType().getTypePtr();
        if (typePtr->isRecordType())
            setSubFields(VG, typePtr->getAsRecordDecl());
        else
            VG->count = 1;
        RG->count += VG->count;
        RG->fields.push_back(VG);
    }
}