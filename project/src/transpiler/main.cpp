#include "matchers.h"
#include "util.h"
#include "unit_transpiler.h"
#include "eo_object.h"
#include <csignal>
#include <cstdlib>

using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace std;

static llvm::cl::OptionCategory MyToolCategory("c2eo options");

const char **TransformArgv(const char *const *argv);

std::string package_name;
std::string filename;

UnitTranspiler transpiler;

void SegfaultSigaction(int, siginfo_t *si, void *) {
  llvm::errs() << "exception: segfault at address " << si->si_addr << " while tool run\n";
  ofstream out(filename);
  out << "+package c2eo.src." << package_name << "\n\n";
  out << "+alias c2eo.stdio.printf\n\n";
  out << "[args...] > global\n";
  out << "  printf \"Segfault exception at address " << si->si_addr << " while tool run\" > @\n";
  out.close();
  exit(0);
}


int main(int argc, const char **argv) {

  if (argc < 3) {
    llvm::errs() << "exception: incorrect command line format. Necessary: c2eo <C-file-name_> <EO-file-name_>\n";
    return -1;
  }

  std::string new_in_file_name{std::string(argv[1]) + ".i"};
  std::string ppc_command{"clang -E " + std::string(argv[1]) + std::string(" > ") + new_in_file_name};
  const char *ppc = ppc_command.c_str();
  system(ppc);

  int parser_argc = 6;
  argv[1] = new_in_file_name.c_str();
  const char **parser_argv = TransformArgv(argv);
  filename = argv[2];

  package_name = filename.substr(0, filename.size() - 3);
  if (package_name.rfind('/') != std::string::npos)
    package_name = package_name.substr(package_name.rfind('/') + 1);
  transpiler.SetPackageName(package_name);

  std::string path_name;
  auto pos = filename.rfind('/');
  if (pos != std::string::npos)
    path_name = filename.substr(0, pos + 1);
  UnitTranspiler::SetPathName(path_name);

  auto expected_parser
      = CommonOptionsParser::create(parser_argc, parser_argv, MyToolCategory, llvm::cl::Optional);

  if (!expected_parser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << expected_parser.takeError();
    return 1;
  }

  CommonOptionsParser &options_parser = expected_parser.get();
  ClangTool tool(options_parser.getCompilations(),
                 options_parser.getSourcePathList());

  MatchFinder finder;
  AddMatchers(finder);
  tool.setPrintErrorMessage(false);

  struct sigaction sa{};
  memset(&sa, 0, sizeof(struct sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = SegfaultSigaction;
  sa.sa_flags = SA_SIGINFO;

  sigaction(SIGSEGV, &sa, nullptr);
  auto result = tool.run(newFrontendActionFactory(&finder).get());
  if (result) {
    cerr << "An error in clang occurred\n";
  }
  std::ofstream out(filename);
  out << transpiler;
}

const char **TransformArgv(const char *const *argv) {
  const char **parser_argv = new const char *[6];
  parser_argv[0] = argv[0];
  parser_argv[1] = argv[1];
  parser_argv[2] = "--";
  parser_argv[3] = "-I/usr/include/linux";
  // parser_argv[4] = "-I/usr/include/c++/11/tr1";
  // parser_argv[5] = "-I/usr/include/c++/11";
  parser_argv[4] = "-I/usr/include/c++/11.2.0/tr1";
  parser_argv[5] = "-I/usr/include/c++/11.2.0";
  return parser_argv;
}
