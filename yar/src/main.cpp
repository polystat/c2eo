/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"
#include "generator.h"
#include "util.h"


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("c2eo options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");


//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {

    GlobalSpaceGen globGen;
    GlobalVarGen::globalSpaceGenPtr = &globGen;
    std::string globObj;
    std::string globInit;

    ApplicationGen appGen;
    std::string appCode;

    if (argc < 2) {
        llvm::errs() << "Incorrect command line format. Necessary: recvisitor <C-file-name>\n";
        return -1;
    }

    std::string path = "";
    if (argc > 3 && strcmp("-d", argv[argc - 2]) == 0) {
        path = argv[argc - 1];
        llvm::errs() << path;
        argc -= 2;
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
    auto result = Tool.run(newFrontendActionFactory(&finder).get());

//         CodeGenerator::getCodeToConsole();

//         CodeGenerator::getCodeToFile("test.eo");
//         llvm::outs() << "code printed to file " << "test.eo" << "\n";

    globGen.Generate(globObj);
#ifdef _DEBUG
    std::string globDb;
    globGen.ShowValue(globDb);
#endif
    globGen.GenValue(globInit);
    llvm::outs() << "\n===================================\n";
    llvm::outs() << globObj;
    str2file(globObj, "glob.global");
    llvm::outs() << globInit;
    str2file(globInit, "glob.seq");
#ifdef _DEBUG
    llvm::outs() << globDb;
    str2file(globDb, "glob.debug");
#endif

    // Тестовое формирование глобального объекта с инициализацией
    std::vector<std::string> text;
    createGlobal(text);
    text2file(text, path + "global.eo");

    llvm::outs() << "\n===================================\n";
    appGen.Generate(appCode);
    llvm::outs() << appCode;

/*#ifdef _DEBUG
    Debug debug;
    debug.PrintAll(appCode, &globGen.globalObjects);
#endif*/
    str2file(appCode, path + "app.eo");

    return 0;
    return result;
}
