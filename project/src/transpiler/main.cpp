/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-2022 c2eo team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <csignal>
#include <cstdlib>

#include "src/transpiler/eo_object.h"
#include "src/transpiler/matchers.h"
#include "src/transpiler/unit_transpiler.h"
#include "src/transpiler/util.h"

static const int parser_arg_count = 4;

using clang::tooling::ClangTool;
using clang::tooling::CommonOptionsParser;
using clang::tooling::newFrontendActionFactory;

static llvm::cl::OptionCategory MyToolCategory("c2eo options");

const char **TransformArgv(const char *const *argv);

std::string package_name;
std::string filename;

UnitTranspiler transpiler;

void SegfaultSigaction(int /*unused*/, siginfo_t *si, void * /*unused*/) {
  llvm::errs() << "exception: segfault at address " << si->si_addr
               << " while tool run\n";
  std::ofstream out(filename);
  out << "+alias c2eo.coperators.printf\n\n";
  out << "+package c2eo.src." << package_name << "\n\n";
  out << "[args...] > global\n";
  out << "  printf \"exception: segfault at address " << si->si_addr
      << " while tool run\" > @\n";
  out.close();
  exit(-1);
}

void SigAbrtSigaction(int /*unused*/, siginfo_t *si, void * /*unused*/) {
  llvm::errs() << "exception: SIGABRT with code " << si->si_code
               << " while tool run\n";
  std::ofstream out(filename);
  out << "+alias c2eo.coperators.printf\n\n";
  out << "+package c2eo.src." << package_name << "\n\n";
  out << "[args...] > global\n";
  out << "  printf \"exception: SIGABRT at address " << si->si_addr
      << " while tool run\" > @\n";
  out.close();
  exit(-1);
}

int main(int argc, const char **argv) {
  struct sigaction sa {};
  memset(&sa, 0, sizeof(struct sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = SegfaultSigaction;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, nullptr);

  struct sigaction sab {};
  memset(&sab, 0, sizeof(struct sigaction));
  sigemptyset(&sab.sa_mask);
  sab.sa_sigaction = SigAbrtSigaction;
  sab.sa_flags = SA_SIGINFO;
  sigaction(SIGABRT, &sab, nullptr);

  if (argc < 3) {
    llvm::errs() << "exception: incorrect command line format. Necessary: c2eo "
                    "<C-file-name_> <EO-file-name_> [--meta]\n";
    return -1;
  }

  std::string new_in_file_name{std::string(argv[1]) + ".i"};
  std::string ppc_command{"clang -E " + std::string(argv[1]) +
                          std::string(" > ") + new_in_file_name};
  const char *ppc = ppc_command.c_str();
  system(ppc);

  argv[1] = new_in_file_name.c_str();
  const char **parser_argv = TransformArgv(argv);
  filename = argv[2];

  if (argc == 4) {
    if (std::string("--meta") != argv[3]) {
      llvm::errs()
          << "exception: incorrect command line format. Necessary: c2eo "
             "<C-file-name_> <EO-file-name_> [--meta]\n";
      return -1;
    }
    transpiler.GenerateMeta();
  }

  package_name = filename.substr(0, filename.size() - 3);
  if (package_name.rfind('/') != std::string::npos) {
    package_name = package_name.substr(package_name.rfind('/') + 1);
  }
  transpiler.SetPackageName(package_name);

  std::string path_name;
  auto pos = filename.rfind('/');
  if (pos != std::string::npos) {
    path_name = filename.substr(0, pos + 1);
  }
  transpiler.SetPathName(path_name);

  int parser_argc = parser_arg_count;
  auto expected_parser = CommonOptionsParser::create(
      parser_argc, parser_argv, MyToolCategory, llvm::cl::Optional);

  delete[] parser_argv;

  if (!expected_parser) {
    // Fail gracefully for unsupported options.
    llvm::errs() << expected_parser.takeError();
    return 1;
  }

  CommonOptionsParser &options_parser = expected_parser.get();
  ClangTool tool(options_parser.getCompilations(),
                 options_parser.getSourcePathList());

  clang::ast_matchers::MatchFinder finder;
  AddMatchers(finder);
  tool.setPrintErrorMessage(false);

  auto result = tool.run(newFrontendActionFactory(&finder).get());
  if (result != 0) {
    std::cerr << "An error in clang occurred\n";
  }
  std::ofstream out(filename);
  out << transpiler;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
const char **TransformArgv(const char *const *argv) {
  const char **parser_argv = new const char *[parser_arg_count];
  parser_argv[0] = argv[0];
  parser_argv[1] = argv[1];
  parser_argv[2] = "--";
  parser_argv[3] = "-Wno-unused-value";
  return parser_argv;
}
#pragma clang diagnostic pop
