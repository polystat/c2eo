#include "type.h"

std::string getTypeName(const clang::ValueDecl* VD) {
    TypeInfo typeInfo = VD->getASTContext().getTypeInfo(VD->getType());
    uint64_t typeSize = typeInfo.Width;
    unsigned fieldAlign = typeInfo.Align;
    const clang::QualType qualType = VD->getType();
    auto typePtr = qualType.getTypePtr();

    if (typePtr->isBooleanType())
        return "c_bool";
    if (typePtr->isRealFloatingType())
        return "c_float" + std::to_string(typeInfo.Width);

    std::string strType = "c_";
    if (!typePtr->isSignedIntegerType())
        strType += "u";
    if (typePtr->isCharType())
        return strType + "char";
    if (typePtr->isIntegerType())
        return strType + ("int" + std::to_string(typeInfo.Width));


    if (typePtr->isUnionType())
        strType = "un_";
    if (typePtr->isStructureType())
        strType = "st_";
    if (typePtr->isUnionType() || typePtr->isStructureType()){
        RecordDecl* RD = typePtr->getAsRecordDecl();
        if (RD->hasNameForLinkage())
            strType += RD->getNameAsString();
        else
            strType += std::to_string(reinterpret_cast<uint64_t>(RD));
        return strType;
    }

    return "memptr";
}