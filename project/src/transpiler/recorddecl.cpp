#include "stmt.h"
#include "recorddecl.h"
#include "vardecl.h"

void getRecordDeclSubObjects(const RecordDecl* RD) {
    if (!RD->hasLinkage())return; //todo: test it!
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
}

std::vector<RecordGen*> getAllRecordDeclSubObjects(const RecordDecl* RD) {
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

    std::vector < RecordGen * > RGs;
    RGs.push_back(RG);

    for (auto it = RD->decls_begin(); it != RD->decls_end(); it++) {
        if ((*it)->getKind() == Decl::Record) {
            std::vector<RecordGen*> subRGs = getAllRecordDeclSubObjects((const RecordDecl*) *it);
            for (auto subRG: subRGs)
                RGs.push_back(subRG);
        }
    }
    return RGs;
}

void setSubFields(RecordGen* RG, const RecordDecl* RD) {
    RG->count = 0;
    for (clang::RecordDecl::field_iterator it = RD->field_begin(); it != RD->field_end(); it++) {
        std::string strType = "";
        //TODO fix string below
        //getTypeName(*it, strType);
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