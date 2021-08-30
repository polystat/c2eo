#include "matchers.h"

//==============================================================================


//------------------------------------------------------------------------------
void addIntVarDeclMatcher(MatchFinder &finder) {
    static IntVarDeclAnalyzer intVarDeclAnalyzer;
    static DeclarationMatcher intVarDeclMatcher =
        varDecl(hasType(isInteger())).bind("intVarDecl");

    finder.addMatcher(intVarDeclMatcher, &intVarDeclAnalyzer);
}

//------------------------------------------------------------------------------
void addLoopMatcher(MatchFinder &finder) {
    static LoopAnalyzer loopAnalyzer;
    static StatementMatcher loopMatcher =
    forStmt(hasLoopInit(declStmt(
                hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))))
                                  .bind("initVarName")))),
            hasIncrement(unaryOperator(
                hasOperatorName("++"),
                hasUnaryOperand(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("incVarName")))))),
            hasCondition(binaryOperator(
                hasOperatorName("<"),
                hasLHS(ignoringParenImpCasts(declRefExpr(
                    to(varDecl(hasType(isInteger())).bind("condVarName"))))),
                hasRHS(expr(hasType(isInteger())))))).bind("forLoop");

    finder.addMatcher(loopMatcher, &loopAnalyzer);
}

//==============================================================================
void addMatchers(MatchFinder &finder) {
    addIntVarDeclMatcher(finder);
    addLoopMatcher(finder);
}
