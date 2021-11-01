#include "stmt.h"
//#define STM_DEB
UnaryStmtGen *getUnaryOpertorStatement(const UnaryOperator *pOperator,  int shift);
BinaryStmtGen *getBinaryStatement(const BinaryOperator *pOperator, int shift);

UnaryStmtGen *getCastGen(const ImplicitCastExpr *pExpr,  int shift);
UnaryStmtGen *getEmptyUnaryGen(const Expr *pExpr,  int shift);

UnaryStmtGen *getDeclName(const DeclRefExpr *pExpr);

StmtGen *getStmtGen(ConstStmtIterator i, int shift);

CompoundStmtGen *getCompoundStmtOutputGenerator(Expr *pExpr, int shift);

UnaryStmtGen *getIntegerLiteralGen(const IntegerLiteral *pLiteral, int shift);
UnaryStmtGen *getASPIntIntegerLiteralGen(const APInt pNum, bool isSignedInt, int shift);

ASTContext* context;

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод основных параметров составного оператора
void getCompoundStmtParameters(const CompoundStmt* CS,ASTContext* context ,int shift) {
    std::string strShift = "";
    for(int i = 0; i < shift; i++) {strShift += "  ";}
    bool isBodyEmpty = CS->body_empty();
    if(isBodyEmpty) {
        llvm::outs() << "  Body is Empty\n";
    } else {
        llvm::outs() << "  Body isn't Empty\n";
    }
    unsigned bodySize = CS->size();
    llvm::outs() << "  Body size = " << bodySize << " \n";

#ifdef STM_DEB
    //getCompoundStmtGenerator(CS, 0);

    for(CompoundStmt::const_body_iterator i = CS->body_begin(); i != CS->body_end(); i++) {
        char* stmtName = (char*)((*i)->getStmtClassName());
        Stmt::StmtClass stmtClass = (*i)->getStmtClass();
        llvm::outs() << "      It is " << stmtName << " stmtClass = " <<  stmtClass << "\n";
        const clang::BinaryOperator* op = (BinaryOperator*)(*i);
        llvm::outs() << "      operator: " << op->getOpcodeStr() << " isEval " << op->isEvaluatable(*context) <<"\n";
        for(BinaryOperator::const_child_iterator i = op->child_begin(); i != op->child_end(); i++)
        {
            llvm::outs()  << "          stmt class   " <<(*i)->getStmtClassName() << "\n";
            if (std::string((*i)->getStmtClassName()).compare("ImplicitCastExpr")==0)
            {
                const ImplicitCastExpr* exp = (ImplicitCastExpr*)(*i);
                llvm::outs()  << "             stmt class   " <<exp->child_begin()->getStmtClassName() << "\n";
                const DeclRefExpr* var = (DeclRefExpr*)(*(exp->child_begin()));
                llvm::outs()  << "                var ref ID   " <<var->getFoundDecl()<< "\n";

            }
        }
        //auto isAssigmentOperator = ;
    }
#endif
}

CompoundStmtGen* getCompoundStmtGenerator(const CompoundStmt *CS,ASTContext* context, int shift, bool isDecorator) {
    ::context = context;
    CompoundStmtGen* compoundStmt = new CompoundStmtGen;
    compoundStmt->value = "seq";
    if (isDecorator)
        compoundStmt->value += " > @";
    compoundStmt->shift = shift;
    for(CompoundStmt::const_body_iterator i = CS->body_begin(); i != CS->body_end(); i++) {
        // Костыльное решение для тестового выводо
        // char* stmtName = (char*)((*i)->getStmtClassName());
        // if (strcmp(stmtName , "ImplicitCastExpr") == 0)
        Stmt::StmtClass stmtClass = (*i)->getStmtClass();
        if(stmtClass == Stmt::ImplicitCastExprClass) // Нужно разобраться с именами перчислимых типов
        {
            CompoundStmtGen *stmtGen = getCompoundStmtOutputGenerator((ImplicitCastExpr*)(*i),shift + 1);
            compoundStmt->Add(stmtGen);
            continue;
        }
        StmtGen *stmtGen = getStmtGen(i, shift + 1);
        if (stmtGen != nullptr)
            compoundStmt->Add(stmtGen);
    }
//     CompoundStmtGen * trueStmt = new CompoundStmtGen;
//     trueStmt->value = "  TRUE";
//     trueStmt->shift = shift;
//     compoundStmt->Add(trueStmt);
    return compoundStmt;
}
//Временное решение для вывода
CompoundStmtGen *getCompoundStmtOutputGenerator(Expr *pExpr, int shift) {
    CompoundStmtGen* compoundStmt = new CompoundStmtGen;
    compoundStmt->value = "seq";
    compoundStmt->shift = shift;
    // Вывод переменной
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen-> shift = shift + 1;
    unaryStmtGen->value = "stdout ";
    unaryStmtGen->postfix = ".as-string";
    DeclRefExpr *declExpr = (DeclRefExpr *)(*(pExpr->child_begin()));
    unaryStmtGen->nestedStmt = getDeclName(declExpr);
    compoundStmt->Add(unaryStmtGen);
    // Вывод перевода строки
    unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen-> shift = shift + 1;
    unaryStmtGen-> value = R"(stdout "\n")";
    unaryStmtGen-> nestedStmt = nullptr;
    compoundStmt->Add(unaryStmtGen);
    return compoundStmt;
}

StmtGen *getStmtGen(ConstStmtIterator i, int shift) {
    StmtGen* stmtGen = nullptr;
    //!!char* stmtName = (char*)((*i)->getStmtClassName());
    //if (strcmp(stmtName ,"BinaryOperator") == 0)
    Stmt::StmtClass stmtClass = (*i)->getStmtClass();
    if(stmtClass == Stmt::BinaryOperatorClass)
    {
        const BinaryOperator* op = (BinaryOperator*)(*i);
        if(op->isIntegerConstantExpr(*context))
        {
            Optional<llvm::APSInt> val = op->getIntegerConstantExpr(*context);
            stmtGen = getASPIntIntegerLiteralGen(val.getValue(), true,shift);
        }
        else {
            BinaryStmtGen *binaryStmtGen = getBinaryStatement(op, shift);
            binaryStmtGen->shift = shift;
            stmtGen = binaryStmtGen;
        }
    }
    //else if (strcmp(stmtName , "ParenExpr") == 0)
    else if(stmtClass == Stmt::ParenExprClass)
   {
        const ParenExpr* op = (ParenExpr*)(*i);
        UnaryStmtGen* unaryStmtGen = getEmptyUnaryGen(op, shift);
        unaryStmtGen->shift  = shift;
        stmtGen = unaryStmtGen;
    }
    //else if (strcmp(stmtName , "IntegerLiteral") == 0)
    else if(stmtClass == Stmt::IntegerLiteralClass)
    {
        const IntegerLiteral* op = (IntegerLiteral*)(*i);
        UnaryStmtGen* unaryStmtGen = getIntegerLiteralGen(op, shift);
        unaryStmtGen->shift  = shift;
        stmtGen = unaryStmtGen;
    }
    //else if (strcmp(stmtName , "ImplicitCastExpr") == 0)
    else if(stmtClass == Stmt::ImplicitCastExprClass)
    {
        const ImplicitCastExpr* op = (ImplicitCastExpr*)(*i);
        UnaryStmtGen* unaryStmtGen = getCastGen(op, shift);
        unaryStmtGen->shift  = shift;
        stmtGen = unaryStmtGen;
    }
    //else if(strcmp(stmtName , "DeclRefExpr") == 0)
    else if(stmtClass == Stmt::DeclRefExprClass)
    {
        const DeclRefExpr* op = (DeclRefExpr*)(*i);
        UnaryStmtGen* unaryStmtGen = getDeclName(op);
        unaryStmtGen->shift  = shift;
        stmtGen = unaryStmtGen;
    }
    //else if (strcmp(stmtName ,"UnaryOperator") == 0)
    else if(stmtClass == Stmt::UnaryOperatorClass)
    {
        const UnaryOperator* op = (UnaryOperator*)(*i);
        UnaryStmtGen* unaryStmtGen = getUnaryOpertorStatement(op, shift);
        unaryStmtGen->shift  = shift;
        stmtGen = unaryStmtGen;
    }
    //else if(strcmp(stmtName , "CompoundStmt") == 0)
    else if(stmtClass == Stmt::CompoundStmtClass)
    {
        const CompoundStmt* cs = (CompoundStmt*)(*i);
        CompoundStmtGen* compoundStmtGen = getCompoundStmtGenerator(cs,context,shift,false);
        stmtGen = compoundStmtGen;
    }
    return stmtGen;
}

UnaryStmtGen *getIntegerLiteralGen(const IntegerLiteral *pLiteral, int shift) {

    return  getASPIntIntegerLiteralGen(pLiteral->getValue(),pLiteral->getType()->isSignedIntegerType(), shift);
}

UnaryStmtGen *getASPIntIntegerLiteralGen(const APInt pNum, bool isSignedInt, int shift) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen->value = pNum.toString(10,isSignedInt);
    unaryStmtGen-> nestedStmt = nullptr;
    return  unaryStmtGen;
}

// Метод для получения имени переменной.
UnaryStmtGen *getDeclName(const DeclRefExpr *pExpr) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    //pExpr->printPretty();
    unaryStmtGen->value = AbstractGen::identifiers[reinterpret_cast<uint64_t>(pExpr->getFoundDecl())];
    unaryStmtGen-> nestedStmt = nullptr;
    return  unaryStmtGen;
}

UnaryStmtGen *getCastGen(const ImplicitCastExpr *pExpr, int shift) {
    return getEmptyUnaryGen(pExpr,shift + 1);
}

UnaryStmtGen *getEmptyUnaryGen(const Expr *pExpr, int shift) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen->value = "";
    unaryStmtGen-> nestedStmt =  getStmtGen(pExpr->child_begin(), shift + 1);
    return  unaryStmtGen;

}
UnaryStmtGen *getUnaryOpertorStatement(const UnaryOperator *pOperator,  int shift) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    std::string opName = pOperator->getOpcodeStr(pOperator->getOpcode()).str();
    if (opName.compare("-") == 0)
    {
        unaryStmtGen->postfix = ".neg";
    }
    unaryStmtGen-> nestedStmt =  getStmtGen(pOperator->child_begin(), shift + 1);
    return  unaryStmtGen;
}
BinaryStmtGen *getBinaryStatement(const BinaryOperator *pOperator, int shift) {
    BinaryStmtGen* binaryStmtGen = new BinaryStmtGen;
    std::string opName = pOperator->getOpcodeStr().str();
    if (opName.compare("=") == 0)
    {
        binaryStmtGen->value = ".write ";
    }
    else if (opName.compare("+") == 0)
    {
        binaryStmtGen->value = ".add ";
    }
    else if (opName.compare("-") == 0)
    {
        binaryStmtGen->value = ".sub ";
    }
    else if (opName.compare("*") == 0)
    {
        binaryStmtGen->value = ".mul ";
    }
    else if (opName.compare("/") == 0)
    {
        binaryStmtGen->value = ".div ";
    }
    else if (opName.compare("%") == 0)
    {
        binaryStmtGen->value = ".mod ";
    }
    else if (opName.compare("==") == 0)
    {
        binaryStmtGen->value = ".eq ";
    }
    else if (opName.compare("!=") == 0)
    {
        binaryStmtGen->value = ".neq ";
    }
    else if (opName.compare("<") == 0)
    {
        binaryStmtGen->value = ".less ";
    }
    else if (opName.compare("<=") == 0)
    {
        binaryStmtGen->value = ".leq ";
    }
    else if (opName.compare(">") == 0)
    {
        binaryStmtGen->value = ".greater ";
    }
    else if (opName.compare(">=") == 0)
    {
        binaryStmtGen->value = ".geq ";
    }
    else
    {
        binaryStmtGen->value = "";
    }
    binaryStmtGen -> left = getStmtGen(pOperator->child_begin(), shift + 1);
    binaryStmtGen -> right = getStmtGen(++pOperator->child_begin(), shift + 1);


    return binaryStmtGen;

}

