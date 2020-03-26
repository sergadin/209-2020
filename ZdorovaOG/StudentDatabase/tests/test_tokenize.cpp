#include "../parser/token.h"
#include "test_runner.h"

#include <iostream>
#include <map>
#include <sstream>

void TestRequestEmpty() {
  std::stringstream inp;
  auto out = Tokenize(inp);
  ASSERT(out.empty());
}

void TestRequestSimple() {
  std::map<std::string, std::string> data = {
      {"group", "4"}, {"rating", "4.15"}, {"name", "\"Ivan\""}};
  for (const auto &[field, value] : data)
    for (const auto &op : {"==", "!=", ">", ">=", "<", "<="}) {
      std::stringstream inp;
      inp << field << op << value;
      auto out = Tokenize(inp);
      ASSERT(out.size() == 3);
      ASSERT_EQUAL(out[0].value, field);
      ASSERT_EQUAL(out[1].value, op);
      ASSERT_EQUAL(out[2].value,
                   field == "name" ? value.substr(1, value.size() - 2) : value);
      ASSERT(out[0].type == TokenType::COLUMN);
      ASSERT(out[1].type == TokenType::COMPARE_OP);
      ASSERT(out[2].type ==
             (field == "name" ? TokenType::NAME : TokenType::NUMBER));
    }
}

void TestRequestBad() {
  std::vector<std::string> data = {"groups", "groupes", "names", "rate",
                                   "year"};
  for (const auto &item : data) {
    bool unsuccess = false;
    try {
      std::stringstream inp;
      inp << item;
      auto out = Tokenize(inp);
    } catch (...) {
      unsuccess = true;
    }
    ASSERT(unsuccess);
  }
}

void TestRequestCaseIndepend() {
  std::vector<std::string> data = {"group", "rating", "name", "and", "or"};
  for (const auto &item : data) {
    std::stringstream inp1;
    inp1 << item;
    std::stringstream inp2;
    for (char c : item)
      inp2 << char(std::toupper(c));
    auto out1 = Tokenize(inp1);
    auto out2 = Tokenize(inp2);
    ASSERT_EQUAL(out1[0].value, out2[0].value);
    ASSERT(out1[0].type == out2[0].type);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRequestEmpty);
  RUN_TEST(tr, TestRequestSimple);
  RUN_TEST(tr, TestRequestCaseIndepend);
  RUN_TEST(tr, TestRequestBad);
  return 0;
}