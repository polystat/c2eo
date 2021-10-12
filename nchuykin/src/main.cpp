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


void generateSpace(SpaceGen &globGen,  const char *objFilename,
                   const char* initFilename);

//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {

    SpaceGen globGen, staticLocalGen, staticGlobalGen;
    VarGen::globalSpaceGenPtr = &globGen;
    VarGen::globalStaticSpaceGenPtr = &staticGlobalGen;
    VarGen::localStaticSpaceGenPtr = &staticLocalGen;


    std::string staticLocalObj;
    std::string staticLocalInit;
    std::string staticGlobalObj;
    std::string staticGlobalInit;

    ApplicationGen appGen;
    std::string appCode;

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
    auto result = Tool.run(newFrontendActionFactory(&finder).get());

//         CodeGenerator::getCodeToConsole();

//         CodeGenerator::getCodeToFile("test.eo");
//         llvm::outs() << "code printed to file " << "test.eo" << "\n";

    std::string objFilename ="glob.global";
    std::string  initFilename ="glob.seq";

    generateSpace(globGen, "glob.global", "glob.seq");
    generateSpace(staticLocalGen,  "staticLocal.global", "staticLocal.seq");
    generateSpace(staticGlobalGen, "staticGlobal.global", "staticGlobal.seq");

    // Тестовое формирование глобального объекта с инициализацией
    std::vector<std::string> text;
    createGlobal(text);
    text2file(text, "global.eo");

    llvm::outs() << "\n===================================\n";
    appGen.Generate(appCode);
    llvm::outs() << appCode;
    str2file(appCode, "app.eo");

    return result;
}

void generateSpace(SpaceGen &globGen, const char* objFilename,
                   const char* initFilename) {
    std::string obj;
    std::string init;
    globGen.Generate(obj);
    globGen.GenValue(init);
    outs() << "\n===================================\n";
    outs() << obj;
    str2file(obj, objFilename);
    outs() << init;
    str2file(init, initFilename);
}
