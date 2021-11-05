#include "stmt.h"
#include "generator.h"
#include "recorddecl.h"

void getRecordDeclSubObjects(const RecordDecl* RD) {
    if (RD->isUnion())
        llvm::outs() << "Union \n";
    if (RD->isStruct())
        llvm::outs() << "Struct \n";
}