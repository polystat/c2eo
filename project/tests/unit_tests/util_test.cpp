//
// Created by yarrya on 22.08.22.
//
#include <gtest/gtest.h>

#include "util.h"

TEST(Util, StrJoinEmpty1) {
  std::vector<std::string> vector;
  EXPECT_EQ(StrJoin(vector), "");
}

TEST(Util, StrJoinEmpty2) { EXPECT_EQ(StrJoin({}), ""); }

TEST(Util, StrJoinOneElement) {
  std::string st = "hello";
  EXPECT_EQ(StrJoin({st}), st);
}

TEST(Util, StrJoin) {
  std::vector<std::string> vector;
  std::string st = "hello-";
  for (int i = 0; i < 6; i++) {
    vector.push_back(st);
    if (i != 0) {
      st += " " + st;
    }
  }
  EXPECT_EQ(StrJoin(vector), st);
}

TEST(Util, Str2FileEmpty) {
  std::string file_name = "test01.txt";
  Str2File("", file_name);
  std::ifstream file;
  file.open(file_name);
  EXPECT_EQ(file.is_open(), true);
}

TEST(Util, Str2FileHello) {
  std::string file_name = "test02.txt";
  std::string text = "hello";
  Str2File(text, file_name);
  std::ifstream file;
  file.open(file_name);
  EXPECT_EQ(file.is_open(), true);
  std::string out;
  std::string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      if (!out.empty())
        out += "\n";
      out += line;
    }
  }
}

TEST(Util, Text2FileEmpty) {
  std::string file_name = "test03.txt";
  std::vector<std::string> vector;
  Text2File(vector, file_name);
  std::ifstream file;
  file.open(file_name);
  EXPECT_EQ(file.is_open(), true);
}

TEST(Util, Text2FileHello) {
  std::string file_name = "test04.txt";
  std::vector<std::string> vector;
  vector.push_back("hello");
  Text2File(vector, file_name);
  std::ifstream file;
  file.open(file_name);
  EXPECT_EQ(file.is_open(), true);
  std::string out;
  std::string line;
  if (file.is_open()) {
    while (getline(file, line)) {
      if (!out.empty())
        out += "\n";
      out += line;
    }
  }
}