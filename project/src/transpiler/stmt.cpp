#include "stmt.h"
#include "generator.h"

//#define STM_DEB
UnaryStmtGen* getUnaryOpertorStatement(const UnaryOperator* pOperator);

BinaryStmtGen* getBinaryStatement(const BinaryOperator* pOperator);

UnaryStmtGen* getCastGen(const ImplicitCastExpr* pExpr);

UnaryStmtGen* getEmptyUnaryGen();

StmtGen* getStmtGen(const Stmt* i);

UnaryStmtGen* getEmptyUnaryGen(const Expr* pExpr);

UnaryStmtGen* getDeclName(const DeclRefExpr* pExpr);

StmtGen* getCompoundStmtOutputGenerator(const Stmt* pExpr);

UnaryStmtGen* getIntegerLiteralGen(const IntegerLiteral* pLiteral);

UnaryStmtGen* getASPIntIntegerLiteralGen(const APInt pNum, bool isSignedInt);

UnaryStmtGen* getFloatingLiteralGen(const FloatingLiteral* pLiteral);

StmtGen* getIfStmtGenerator(const IfStmt* pStmt);

StmtGen* getWhileStmtGenerator(const WhileStmt* pStmt);

StmtGen* getDoWhileStmtGenerator(const DoStmt* pStmt);

ASTContext* context;

//-------------------------------------------------------------------------------------------------
// Определение и тестовый вывод основных параметров составного оператора
void getCompoundStmtParameters(const CompoundStmt* CS, ASTContext* context) {
    std::string strShift = "  ";
    //  for(int i = 0; i < shift; i++) {strShift += "  ";}
    bool isBodyEmpty = CS->body_empty();
    if (isBodyEmpty) {
        llvm::outs() << "  Body is Empty\n";
    } else {
        llvm::outs() << "  Body isn't Empty\n";
    }
    unsigned bodySize = CS->size();
    llvm::outs() << "  Body size = " << bodySize << " \n";

#ifdef STM_DEB
    //getCompoundStmtGenerator(CS, 0);

    for(CompoundStmt::const_body_iterator i = CS->body_begin(); i != CS->body_end(); i++) {
        char* stmtName = (char*)(i->getStmtClassName());
        Stmt::StmtClass stmtClass = i->getStmtClass();
        llvm::outs() << "      It is " << stmtName << " stmtClass = " <<  stmtClass << "\n";
        const clang::BinaryOperator* op = (BinaryOperator*)i;
        llvm::outs() << "      operator: " << op->getOpcodeStr() << " isEval " << op->isEvaluatable(*context) <<"\n";
        for(BinaryOperator::const_child_iterator i = op->child_begin(); i != op->child_end(); i++)
        {
            llvm::outs()  << "          stmt class   " <<i->getStmtClassName() << "\n";
            if (std::string(i->getStmtClassName()).compare("ImplicitCastExpr")==0)
            {
                const ImplicitCastExpr* exp = (ImplicitCastExpr*)i;
                llvm::outs()  << "             stmt class   " <<exp->child_begin()->getStmtClassName() << "\n";
                const DeclRefExpr* var = (DeclRefExpr*)(*(exp->child_begin()));
                llvm::outs()  << "                var ref ID   " <<var->getFoundDecl()<< "\n";

            }
        }
        //auto isAssigmentOperator = ;
    }
#endif
}

StmtGen* getASTStmtGen(const Stmt* i, ASTContext* context){
    ::context = context;
    return getStmtGen(i);
}

CompoundStmtGen* getCompoundStmtGenerator(const CompoundStmt* CS, ASTContext* context, bool isDecorator) {
    ::context = context;
    CompoundStmtGen* compoundStmt = new CompoundStmtGen;
    compoundStmt->is_decorator = isDecorator;

    for (CompoundStmt::const_body_iterator i = CS->body_begin(); i != CS->body_end(); i++) {
        // Костыльное решение для тестового выводо
        // char* stmtName = (char*)(i->getStmtClassName());
        // if (strcmp(stmtName , "ImplicitCastExpr") == 0)
        Stmt::StmtClass stmtClass = (*i)->getStmtClass();
        if (stmtClass == Stmt::ImplicitCastExprClass) // Нужно разобраться с именами перчислимых типов
        {
            StmtGen* stmtGen = getCompoundStmtOutputGenerator(*i);
            compoundStmt->Add(stmtGen);
            continue;
        }
        StmtGen* stmtGen = getStmtGen(*i);
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
StmtGen* getCompoundStmtOutputGenerator(const Stmt* pExpr) {
    MultiLineStmtGen* compoundStmt = new MultiLineStmtGen;
    if (pExpr->children().empty())
        return compoundStmt;
    //compoundStmt->value = "";
    // Вывод переменной
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen->value = R"(printf "%s\n" )";
    unaryStmtGen->postfix += ".as-string";
    // TODO: fix MemberExpr
    DeclRefExpr* declExpr = (DeclRefExpr * )(*pExpr->child_begin());
    while (declExpr->getStmtClass() == Stmt::MemberExprClass) {
        std::string field_name = ".f_";
        field_name += ((MemberExpr*)declExpr)->getMemberDecl()->getNameAsString();
        unaryStmtGen->postfix = field_name + unaryStmtGen->postfix;
        declExpr = (DeclRefExpr * )(*declExpr->child_begin());
    }
    unaryStmtGen->nestedStmt = getDeclName(declExpr);
    compoundStmt->Add(unaryStmtGen);
    return compoundStmt;
}

StmtGen* getStmtGen(const Stmt* i) {
    //TODO подумать над утечкой памяти
    StmtGen* stmtGen = getEmptyUnaryGen();
    //!!char* stmtName = (char*)(i->getStmtClassName());
    //if (strcmp(stmtName ,"BinaryOperator") == 0)
    Stmt::StmtClass stmtClass = i->getStmtClass();
    if (stmtClass == Stmt::BinaryOperatorClass) {
        const BinaryOperator* op = (BinaryOperator * )(i);
        if (op->isIntegerConstantExpr(*context)) {
            Optional <llvm::APSInt> val = op->getIntegerConstantExpr(*context);
            stmtGen = getASPIntIntegerLiteralGen(val.getValue(), true);
        } else {
            BinaryStmtGen* binaryStmtGen = getBinaryStatement(op);
            stmtGen = binaryStmtGen;
        }
    }
        //else if (strcmp(stmtName , "ParenExpr") == 0)
    else if (stmtClass == Stmt::ParenExprClass) {
        const ParenExpr* op = (ParenExpr*) i;
        UnaryStmtGen* unaryStmtGen = getEmptyUnaryGen(op);
        stmtGen = unaryStmtGen;
    }
        //else if (strcmp(stmtName , "IntegerLiteral") == 0)
    else if (stmtClass == Stmt::IntegerLiteralClass) {
        const IntegerLiteral* op = (IntegerLiteral*) i;
        UnaryStmtGen* unaryStmtGen = getIntegerLiteralGen(op);
        stmtGen = unaryStmtGen;
    } else if (stmtClass == Stmt::FloatingLiteralClass) {
        const FloatingLiteral* op = (FloatingLiteral*) i;
        UnaryStmtGen* unaryStmtGen = getFloatingLiteralGen(op);
        stmtGen = unaryStmtGen;
    }
        //else if (strcmp(stmtName , "ImplicitCastExpr") == 0)
    else if (stmtClass == Stmt::ImplicitCastExprClass) {
        const ImplicitCastExpr* op = (ImplicitCastExpr*) i;
        UnaryStmtGen* unaryStmtGen = getCastGen(op);
        stmtGen = unaryStmtGen;
    }
        //else if(strcmp(stmtName , "DeclRefExpr") == 0)
    else if (stmtClass == Stmt::DeclRefExprClass) {
        const DeclRefExpr* op = (DeclRefExpr*) i;
        UnaryStmtGen* unaryStmtGen = getDeclName(op);
        stmtGen = unaryStmtGen;
    }
        //else if (strcmp(stmtName ,"UnaryOperator") == 0)
    else if (stmtClass == Stmt::UnaryOperatorClass) {
        const UnaryOperator* op = (UnaryOperator*) i;
        UnaryStmtGen* unaryStmtGen = getUnaryOpertorStatement(op);
        stmtGen = unaryStmtGen;
    }
        //else if(strcmp(stmtName , "CompoundStmt") == 0)
    else if (stmtClass == Stmt::CompoundStmtClass) {
        const CompoundStmt* cs = (CompoundStmt*) i;
        CompoundStmtGen* compoundStmtGen = getCompoundStmtGenerator(cs, context, false);
        stmtGen = compoundStmtGen;
    } else if (stmtClass == Stmt::IfStmtClass) {
        const IfStmt* cs = (IfStmt*) i;
        stmtGen = getIfStmtGenerator(cs);
    } else if (stmtClass == Stmt::WhileStmtClass) {
        const WhileStmt* cs = (WhileStmt*) i;
        stmtGen = getWhileStmtGenerator(cs);
    }
    if (stmtClass == Stmt::DoStmtClass) {
        const DoStmt* cs = (DoStmt*) i;
        stmtGen = getDoWhileStmtGenerator(cs);
    }
    if (stmtClass == Stmt::MemberExprClass) {
        // TODO: fix MemberExpr
        UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
        unaryStmtGen->postfix = "";
        DeclRefExpr* declExpr = (DeclRefExpr * )i;
        while (declExpr->getStmtClass() == Stmt::MemberExprClass) {
            std::string field_name = ".f_";
            field_name += ((MemberExpr*)declExpr)->getMemberDecl()->getNameAsString();
            unaryStmtGen->postfix = field_name + unaryStmtGen->postfix;
            declExpr = (DeclRefExpr * )(*declExpr->child_begin());
        }
        unaryStmtGen->nestedStmt = getDeclName(declExpr);
        stmtGen = unaryStmtGen;
    }
    return stmtGen;
}

StmtGen* getDoWhileStmtGenerator(const DoStmt* pStmt) {
    auto* gen = new DoWhileStmtGen;
    auto cond = getStmtGen(pStmt->getCond());
    UnaryStmtGen* asBoolGen = new UnaryStmtGen;
    asBoolGen->value = "as-bool ";
    asBoolGen->nestedStmt = cond;
    gen->Add(asBoolGen);
    auto* objStmtGen = new ObjectStmtGen;
    auto body = getStmtGen(pStmt->getBody());
    auto* bodyCmp = llvm::dyn_cast<CompoundStmtGen>(body);
    if (!bodyCmp) {
        bodyCmp = new CompoundStmtGen;
        bodyCmp->Add(body);
    }
    bodyCmp->is_decorator = true;
    objStmtGen->body = bodyCmp;
    gen->Add(objStmtGen);
    return gen;
}

StmtGen* getWhileStmtGenerator(const WhileStmt* pStmt) {
    WhileStmtGen* gen = new WhileStmtGen;
    auto cond = getStmtGen(pStmt->getCond());
    UnaryStmtGen* asBoolGen = new UnaryStmtGen;
    asBoolGen->value = "as-bool ";
    asBoolGen->nestedStmt = cond;
    gen->Add(asBoolGen);
    ObjectStmtGen* objStmtGen = new ObjectStmtGen;
    auto body = getStmtGen(pStmt->getBody());
    CompoundStmtGen* bodyCmp = llvm::dyn_cast<CompoundStmtGen>(body);
    if (!bodyCmp) {
        bodyCmp = new CompoundStmtGen;
        bodyCmp->Add(body);
    }
    bodyCmp->is_decorator = true;
    objStmtGen->body = bodyCmp;
    gen->Add(objStmtGen);
    return gen;
}

StmtGen* getIfStmtGenerator(const IfStmt* pStmt) {
    IfStmtGen* gen = new IfStmtGen;
    auto cond = getStmtGen(pStmt->getCond());
    gen->Add(cond);
    auto then_c = getStmtGen(pStmt->getThen());
    gen->Add(then_c);
    if (pStmt->hasElseStorage()) {
        auto else_c = getStmtGen(pStmt->getElse());
        gen->Add(else_c);
    } else {
        CompoundStmtGen* empt = new CompoundStmtGen;
        gen->Add(empt);
    }
    return gen;
}

UnaryStmtGen* getFloatingLiteralGen(const FloatingLiteral* pLiteral) {
    LiteralStmtGen* literalStmtGen = new LiteralStmtGen;
    auto floatValue = pLiteral->getValue().convertToDouble();
    literalStmtGen->value = "c_float64 " + std::to_string(floatValue);
    literalStmtGen->nestedStmt = nullptr;
    return literalStmtGen;
}

UnaryStmtGen* getIntegerLiteralGen(const IntegerLiteral* pLiteral) {

    return getASPIntIntegerLiteralGen(pLiteral->getValue(), pLiteral->getType()->isSignedIntegerType());
}

UnaryStmtGen* getASPIntIntegerLiteralGen(const APInt pNum, bool isSignedInt) {
    LiteralStmtGen* literalStmtGen = new LiteralStmtGen;
    literalStmtGen->value = "c_int32 " + pNum.toString(10, isSignedInt);
    literalStmtGen->nestedStmt = nullptr;
    return literalStmtGen;
}

// Метод для получения имени переменной.
UnaryStmtGen* getDeclName(const DeclRefExpr* pExpr) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    //pExpr->printPretty();
    /*if (pExpr->getStmtClass() == Stmt::MemberExprClass) {
        unaryStmtGen->value = ".f_" + ((MemberExpr*)pExpr)->getMemberDecl()->getNameAsString();
        unaryStmtGen->nestedStmt = getDeclName((DeclRefExpr * )(*pExpr->child_begin()));
    } else {*/
    unaryStmtGen->value = AbstractGen::identifiers[reinterpret_cast<uint64_t>(pExpr->getFoundDecl())];
    unaryStmtGen->nestedStmt = nullptr;
    //}
    return unaryStmtGen;
}

UnaryStmtGen* getCastGen(const ImplicitCastExpr* pExpr) {
    return getEmptyUnaryGen(pExpr);
}

UnaryStmtGen* getEmptyUnaryGen() {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen->value = "";
    unaryStmtGen->nestedStmt = nullptr;
    return unaryStmtGen;
}

UnaryStmtGen* getEmptyUnaryGen(const Expr* pExpr) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    unaryStmtGen->value = "";
    unaryStmtGen->nestedStmt = getStmtGen(*pExpr->child_begin());
    return unaryStmtGen;

}

UnaryStmtGen* getUnaryOpertorStatement(const UnaryOperator* pOperator) {
    UnaryStmtGen* unaryStmtGen = new UnaryStmtGen;
    std::string opName = pOperator->getOpcodeStr(pOperator->getOpcode()).str();
    if (opName == "-")
    {
        unaryStmtGen->value = "neg ";
    }
    unaryStmtGen->nestedStmt = getStmtGen(*pOperator->child_begin());
    return unaryStmtGen;
}

BinaryStmtGen* getBinaryStatement(const BinaryOperator* pOperator) {
    BinaryStmtGen* binaryStmtGen = new BinaryStmtGen;
    std::string opName = pOperator->getOpcodeStr().str();
    if (opName.compare("=") == 0)
    {
        binaryStmtGen->value = "write";
    }
    else if (opName.compare("+") == 0)
    {
        binaryStmtGen->value = "add";
    }
    else if (opName.compare("-") == 0)
    {
        binaryStmtGen->value = "sub";
    }
    else if (opName.compare("*") == 0)
    {
        binaryStmtGen->value = "mul";
    }
    else if (opName.compare("/") == 0)
    {
        binaryStmtGen->value = "div ";
    }
    else if (opName.compare("%") == 0)
    {
        binaryStmtGen->value = "mod";
    }
    else if (opName.compare("==") == 0)
    {
        binaryStmtGen->value = "eq";
    }
    else if (opName.compare("!=") == 0)
    {
        binaryStmtGen->value = "neq";
    }
    else if (opName == "<")
    {
        binaryStmtGen->value = "less";
    }
    else if (opName.compare("<=") == 0)
    {
        binaryStmtGen->value = "leq";
    }
    else if (opName.compare(">") == 0)
    {
        binaryStmtGen->value = "greater";
    }
    else if (opName.compare(">=") == 0)
    {
        binaryStmtGen->value = "geq";
    }
    else
    {
        binaryStmtGen->value = "";
    }
    binaryStmtGen->left = getStmtGen(*pOperator->child_begin());
    binaryStmtGen->right = getStmtGen(*(++pOperator->child_begin()));


    return binaryStmtGen;

}

