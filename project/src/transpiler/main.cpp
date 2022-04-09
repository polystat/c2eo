/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"
#include "util.h"
#include "unit_transpiler.h"
#include "eo_object.h"
#include <csignal>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

static llvm::cl::OptionCategory MyToolCategory("c2eo options");
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("\nMore help text...\n");

const char **transform_argv(const char *const *argv);

std::string packagename;
std::string filename;

void segfault_sigaction(int signal, siginfo_t *si, void *arg) {
  llvm::errs() << "Caught segfault at address " << si->si_addr << " while tool run\n";
  ofstream out(filename);
  out << "+package c2eo.src." << packagename << "\n\n";
  out << "+alias c2eo.stdio.printf\n\n";
  out << "[args...] > global\n";
  out << "  printf \"Segfault exception at address " << si->si_addr << " while tool run\" > @\n";
  out.close();
  exit(0);
}


UnitTranspiler transpiler;

//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {

  if (argc < 3) {
    llvm::errs() << "Incorrect command line format. Necessary: c2eo <C-file-name> <EO-file-name>\n";
    return -1;
  }


  int parser_argc = 6;
  const char **parser_argv = transform_argv(argv);
  const char *inputFileName = argv[1];
  filename = argv[2];

  packagename = filename.substr(0, filename.size() - 3);
  if (packagename.rfind('/') != std::string::npos)
    packagename = packagename.substr(packagename.rfind('/') + 1);
  transpiler.SetPackageName(packagename);


  // TODO Add path to library as parser_argv
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


  // Matchers matchers;
  // return Tool.run(matchers.getFrontEndActionFactory());
  LoopAnalyzer loopAnalyzer;
  MatchFinder finder;
  addMatchers(finder);
  // Finder.addMatcher(LoopMatcher, &loopAnalyzer);
  //
  // Disable unpretty error messages from CLang
  Tool.setPrintErrorMessage(false);

  struct sigaction sa{};
  memset(&sa, 0, sizeof(struct sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = segfault_sigaction;
  sa.sa_flags = SA_SIGINFO;

  sigaction(SIGSEGV, &sa, nullptr);
  auto result = Tool.run(newFrontendActionFactory(&finder).get());

  /*
  if (result == 1)
  {
    // Error ocured
    llvm::errs() << "An error occurred in CLang" << "\n";
    return 0;
  }*/

  // тестовый вывод
  // std::cout << transpiler;
  std::ofstream out(filename);
  out << transpiler;


  // Test out for EOObject correctness testing
  // std::cout << createSeq();
}

const char** transform_argv(const char* const *argv) {
  const char** parser_argv = new const char*[6];
  parser_argv[0] = argv[0];
  parser_argv[1] = argv[1];
  parser_argv[2] = "--";
  parser_argv[3] = "-I/usr/include/linux";
  // parser_argv[4] = "-I/usr/include/c++/10/tr1";
  // parser_argv[5] = "-I/usr/include/c++/10";
  parser_argv[4] = "-I/usr/include/c++/11/tr1";
  parser_argv[5] = "-I/usr/include/c++/11";
  return parser_argv;
}
