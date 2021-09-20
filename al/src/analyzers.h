#ifndef __ANALYZERS__
#define __ANALYZERS__

#include "vardecl.h"

class IntVarDeclAnalyzer : public MatchFinder::MatchCallback {
public: 
    static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
        return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();
    }
    virtual void run(const MatchFinder::MatchResult &Result);
};

class IntVarDeclGlobalMemoryAnalyzer : public MatchFinder::MatchCallback {
public: 
    static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
        return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();
    }
    virtual void run(const MatchFinder::MatchResult &Result);
};

class LoopAnalyzer : public MatchFinder::MatchCallback {
public: 
    static bool areSameVariable(const ValueDecl *First, const ValueDecl *Second) {
        return First && Second && First->getCanonicalDecl() == Second->getCanonicalDecl();
    }
    virtual void run(const MatchFinder::MatchResult &Result);
};

#endif // __ANALYZERS__

