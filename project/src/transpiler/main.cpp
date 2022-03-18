/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"
#include "util.h"
#include "unit_transpiler.h"
#include "eo_object.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("c2eo options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");

const char **transform_argv(const char *const *argv);

UnitTranspiler transpiler;
//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {

    if (argc < 3) {
        llvm::errs() << "Incorrect command line format. Necessary: c2eo <C-file-name> <EO-file-name>\n";
        return -1;
    }


    int parser_argc = 3;
    const char **parser_argv = transform_argv(argv);
    const char* inputFileName = argv[1];
    std::string filename = argv[2];


    transpiler.SetPackageName(filename.substr(0, filename.size()-3));



    auto ExpectedParser
            = CommonOptionsParser::create(parser_argc, parser_argv, MyToolCategory, llvm::cl::Optional);

    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());


////    Matchers matchers;
////    return Tool.run(matchers.getFrontEndActionFactory());
    LoopAnalyzer loopAnalyzer;
    MatchFinder finder;
    addMatchers(finder);
//     Finder.addMatcher(LoopMatcher, &loopAnalyzer);
//
   //Disable unpretty error messages from CLang
   Tool.setPrintErrorMessage(false);
   auto result = Tool.run(newFrontendActionFactory(&finder).get());

   /*
   if (result == 1)
   {
     // Error ocured
     llvm::errs() << "An error occurred in CLang" << "\n";
     return 0;
   }*/

   // тестовый вывод
   //std::cout << transpiler;
   std::ofstream out(filename);
   out << transpiler;


   // Test out for EOObject correctness testing
   //std::cout << createSeq();
}

const char **transform_argv(const char *const *argv) {
    const char** parser_argv = new const char*[3];
    parser_argv[0] = argv[0];
    parser_argv[1] = argv[1];
    parser_argv[2] = "--";
    return parser_argv;
}