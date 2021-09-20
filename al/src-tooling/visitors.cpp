#include "visitors.h"
/*
bool Visitors::VisitRecordDecl(RecordDecl *RD) {
    !!!!RD->dump();
    DeclContext *declContext = RD;
    std::cout << "RD context: " << declContext->getDeclKindName() << "\n";
    std::cout << "Own declaration name: " << RD->getNameAsString() << "\n";
    std::cout << "Qualified declaration name: " << RD->getQualifiedNameAsString() << "\n";

    FullSourceLoc FullLocation = Context->getFullLoc(RD->getBeginLoc());
    if (FullLocation.isValid()) {
        std::cout << "Found declaration at "
                    << FullLocation.getSpellingLineNumber() << ":"
                    << FullLocation.getSpellingColumnNumber() << "\n";
    }
    
    if(RD->hasObjectMember()) {
        std::cout << "Has members\n";
    }
    else {
        std::cout << "Doesn't have members\n";
    }
    return true;
}
*/
bool Visitors::VisitVarDecl(VarDecl *VD) {
    //VD->dump();
    getVarDeclParameters(VD);
//     std::cout << "Variable declaration\n";
//     auto storageClass = VD->getStorageClass();
//     if(storageClass == SC_None) {
//         std::cout << "Storage class: NONE\n";
//     } else {
//         std::cout << "Storage class: " << VD->getStorageClassSpecifierString(storageClass) << "\n";
//     }
    // Проверка на наличие инициализирующей части
//     if(VD->hasInit()) {
//         std::cout << "VarDecl has init part\n";
//         !!!const Expr* expr = VD->getAnyInitializer();
//         Expr* expr = VD->getInit();
//     } else {
//         std::cout << "VarDecl does not have init part\n";
//     }
    // Проверка на наличие инициализирующего выражения
//     if(VD->getAnyInitializer() != nullptr) {
//         std::cout << "VarDecl has AnyInitializer\n";
//     } else {
//         std::cout << "VarDecl does not have AnyInitializer\n";
//     }
    // Проверка на наличие вычисляемого значени
//         if(VD->evaluateValue() != nullptr) {
//             std::cout << "VarDecl has evaluateValue\n";
//         } else {
//             std::cout << "VarDecl does not have evaluateValue\n";
//         }
    
    
    ////const char *buf = VD->getStorageClassSpecifierString(storageClass);
    ////std::cout << "Storage class: " << VD->getStorageClassSpecifierString(storageClass)) << "\n";
    ////std::cout << "Storage class: " << buf << "\n";
    
//         DeclContext *declContext = VD;
//         std::cout << "VD context: " << declContext->getDeclKindName() << "\n";
//         std::cout << "Own declaration name: " << VD->getNameAsString() << "\n";
//         std::cout << "Qualified declaration name: " << VD->getQualifiedNameAsString() << "\n";
// 
//         FullSourceLoc FullLocation = Context->getFullLoc(VD->getBeginLoc());
//         if (FullLocation.isValid()) {
//             std::cout << "Found declaration at "
//                         << FullLocation.getSpellingLineNumber() << ":"
//                         << FullLocation.getSpellingColumnNumber() << "\n";
//         }
//      
//         if(VD->hasObjectMember()) {
//             std::cout << "Has members\n";
//         }
//         else {
//             std::cout << "Doesn't have members\n";
//         }
    return true;
}

// bool Visitors::VisitExpr(Expr *EXPR) {
//     EXPR->dump();
//     std::cout << "Expression\n";
//     
//     if(EXPR->isLValue()) {
//         std::cout << "  LValue Expression\n";
//     }
//     else if(EXPR->isRValue()) {
//         std::cout << "  RValue Expression\n";
//     }
// 
//     return true;
// }
 
// bool Visitors::VisitFullExpr(FullExpr *FEXPR) {
//     FEXPR->dump();
//     std::cout << "Full Expression\n";
//     
//     if(FEXPR->isLValue()) {
//         std::cout << "  LValue Expression\n";
//     }
//     else if(FEXPR->isRValue()) {
//         std::cout << "  RValue Expression\n";
//     }
// 
//     return true;
// }

// bool Visitors::VisitDeclRefExpr(DeclRefExpr *DREXPR) {
//     DREXPR->dump();
//     std::cout << "DeclRefExpr\n";
//     
//     if(DREXPR->isLValue()) {
//         std::cout << "  LValue Expression\n";
//     }
//     else if(DREXPR->isRValue()) {
//         std::cout << "  RValue Expression\n";
//     }
// 
//     return true;
// }

// bool Visitors::VisitBinaryOperator(BinaryOperator *BOP) {
//     BOP->dump();
//     std::cout << "BinaryOperator\n";
//     
//     BinaryOperatorKind opc = BOP->getOpcode();
//     std::cout << "  opc = " << BOP->getOpcodeStr().str() << "\n";
//     return true;
// }
