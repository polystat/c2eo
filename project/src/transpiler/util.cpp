#include "util.h"

using namespace std;

__attribute__((unused)) void Str2File(const std::string &str, const std::string &file_name) {
  std::ofstream out;
  out.open(file_name);
  if (out.is_open()) {
    out << str;
  }
}

__attribute__((unused)) void File2Vector(const std::string &file_name, std::vector<std::string> &text) {
  std::ifstream in;
  in.open(file_name);
  std::string line;
  if (in.is_open()) {
    while (getline(in, line)) {
      text.push_back(line);
    }
  }
}

__attribute__((unused)) void Text2File(std::vector<std::string> &text, const std::string &file_name) {
  std::ofstream out;
  out.open(file_name);
  if (out.is_open()) {
    for (const auto &line : text) {
      out << line << "\n";
    }
  }
}

std::string StrJoin(std::vector<std::string> vec) {
  if (vec.empty())
    return "";
  if (vec.size() == 1)
    return vec[0];
  string res{vec[0]};
  for (int i = 1; i < vec.size(); ++i) {
    res += " " + vec[i];
  }
  return res;
}

