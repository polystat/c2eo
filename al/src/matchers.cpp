#include "matchers.h"

//==============================================================================

//------------------------------------------------------------------------------
// Сопоставитель с циклом из исходного примера
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
// Сопоставитель с определением целочисленной переменной
void addIntVarDeclMatcher(MatchFinder &finder) {
    static IntVarDeclAnalyzer intVarDeclAnalyzer;
    static DeclarationMatcher intVarDeclMatcher =
        varDecl(hasType(isInteger())).bind("intVarDecl");

    finder.addMatcher(intVarDeclMatcher, &intVarDeclAnalyzer);
}

//------------------------------------------------------------------------------
// Сопоставитель с определением глобальной целочисленной переменной
void addIntVarDeclGlobalMemoryMatcher(MatchFinder &finder) {
    static IntVarDeclGlobalMemoryAnalyzer intVarDeclGlobalMemoryAnalyzer;
    static DeclarationMatcher intVarDeclGlobalMemoryMatcher =
        varDecl(
            hasType(isInteger())
        ).bind("intVarGlobalMemoryDecl");

    finder.addMatcher(intVarDeclGlobalMemoryMatcher, &intVarDeclGlobalMemoryAnalyzer);
}

//------------------------------------------------------------------------------
// Сопоставитель с определением глобальной переменной базового типа
void addDeclBaseVarGlobalMemoryMatcher(MatchFinder &finder) {
    static DeclBaseVarGlobalMemoryAnalyzer declBaseVarGlobalMemoryAnalyzer;
    static DeclarationMatcher declBaseVarGlobalMemoryMatcher =
        varDecl().bind("declBaseVarGlobalMemory");

    finder.addMatcher(declBaseVarGlobalMemoryMatcher, &declBaseVarGlobalMemoryAnalyzer);
}

//==============================================================================
void addMatchers(MatchFinder &finder) {
    addDeclBaseVarGlobalMemoryMatcher(finder);
//    addIntVarDeclMatcher(finder);
//    addIntVarDeclGlobalMemoryMatcher(finder);
//    addLoopMatcher(finder);
}
