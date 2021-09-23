#include "matchers.h"

//==============================================================================

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

//------------------------------------------------------------------------------
void addIntVarDeclMatcher(MatchFinder &finder) {
    static IntVarDeclAnalyzer intVarDeclAnalyzer;
    static DeclarationMatcher intVarDeclMatcher =
        varDecl(hasType(isInteger())).bind("intVarDecl");

    finder.addMatcher(intVarDeclMatcher, &intVarDeclAnalyzer);
}

//------------------------------------------------------------------------------
void addIntVarDeclGlobalMemoryMatcher(MatchFinder &finder) {
    static IntVarDeclGlobalMemoryAnalyzer intVarDeclGlobalMemoryAnalyzer;
    static DeclarationMatcher intVarDeclGlobalMemoryMatcher =
        varDecl(
            hasType(isInteger())
        ).bind("intVarGlobalMemoryDecl");

    finder.addMatcher(intVarDeclGlobalMemoryMatcher, &intVarDeclGlobalMemoryAnalyzer);
}

//==============================================================================
void addMatchers(MatchFinder &finder) {
    addIntVarDeclMatcher(finder);
//    addIntVarDeclGlobalMemoryMatcher(finder);
    addLoopMatcher(finder);
}
