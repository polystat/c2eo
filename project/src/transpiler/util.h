#ifndef C2EO_SRC_TRANSPILER_UTIL_H_
#define C2EO_SRC_TRANSPILER_UTIL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string StrJoin(std::vector<std::string> vec);

__attribute__((unused)) void Str2File(const std::string &str, const std::string &file_name);

__attribute__((unused)) void File2Vector(const std::string &file_name, std::vector<std::string> &text);

__attribute__((unused)) void Text2File(std::vector<std::string> &text, const std::string &file_name);

#endif // C2EO_SRC_TRANSPILER_UTIL_H_
