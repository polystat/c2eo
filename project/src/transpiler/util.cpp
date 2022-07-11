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

#include "src/transpiler/util.h"

__attribute__((unused)) void Str2File(const std::string &str,
                                      const std::string &file_name) {
  std::ofstream out;
  out.open(file_name);
  if (out.is_open()) {
    out << str;
  }
}

__attribute__((unused)) void File2Vector(const std::string &file_name,
                                         std::vector<std::string> &text) {
  std::ifstream in;
  in.open(file_name);
  std::string line;
  if (in.is_open()) {
    while (getline(in, line)) {
      text.push_back(line);
    }
  }
}

__attribute__((unused)) void Text2File(std::vector<std::string> &text,
                                       const std::string &file_name) {
  std::ofstream out;
  out.open(file_name);
  if (out.is_open()) {
    for (const auto &line : text) {
      out << line << "\n";
    }
  }
}

std::string StrJoin(std::vector<std::string> vec) {
  if (vec.empty()) {
    return "";
  }
  if (vec.size() == 1) {
    return vec[0];
  }
  std::string res{vec[0]};
  for (int i = 1; i < vec.size(); ++i) {
    res += " " + vec[i];
  }
  return res;
}
