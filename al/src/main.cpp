/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("c2eo options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

/*
 * Главная функция, обеспечивающая начальный запуск и обход AST
 */ 

int main(int argc, const char **argv) {
    if (argc < 2) {
        llvm::errs() << "Incorrect command line format. Necessary: recvisitor <C-file-name>\n";
        return -1;
    }

    auto ExpectedParser 
        = CommonOptionsParser::create(argc, argv, MyToolCategory, llvm::cl::Optional);
    
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());


////    Matchers matchers;
////    return Tool.run(matchers.getFrontEndActionFactory());
     LoopAnalyzer loopAnalyzer;
     MatchFinder finder;
     addMatchers(finder);
//     Finder.addMatcher(LoopMatcher, &loopAnalyzer);
// 
     return Tool.run(newFrontendActionFactory(&finder).get());
}
