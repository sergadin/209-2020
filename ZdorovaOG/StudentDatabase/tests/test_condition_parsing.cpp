#include "../parser/condition_parser.h"
#include "test_runner.h"

#include <iostream>
#include <map>
#include <sstream>

void TestSelectRequestSimple() {
  std::map<std::string, std::string> data = {
      {"group", "4"}, {"rating", "4.15"}, {"name", "\"Ivan\""}};
  for (const auto &[field, value] : data)
    for (const auto &op : {"==", "!=", ">", ">=", "<", "<="}) {
      std::stringstream inp;
      std::stringstream exp;
      inp << field << op << value;
      exp << field << op << value;
      auto ptr = ParseSelectCondition(inp);
      ASSERT(ptr != nullptr);
      std::stringstream out;
      out << *ptr;
      ASSERT_EQUAL(out.str(), exp.str());
    }
}

void TestSelectRequestAnd() {
  std::map<std::string, std::string> data = {
      {"group", "4"}, {"rating", "4.15"}, {"name", "\"Ivan\""}};
  for (const auto &[field1, value1] : data)
    for (const auto &[field2, value2] : data)
      for (const auto &op1 : {"==", "!=", ">", ">=", "<", "<="})
        for (const auto &op2 : {"==", "!=", ">", ">=", "<", "<="}) {
          std::stringstream inp;
          std::stringstream exp;
          inp << field1 << op1 << value1 << " AND " << field2 << op2 << value2;
          exp << '(' << field1 << op1 << value1 << " & " << field2 << op2
              << value2 << ')';
          auto ptr = ParseSelectCondition(inp);
          ASSERT(ptr != nullptr);
          std::stringstream out;
          out << *ptr;
          ASSERT_EQUAL(out.str(), exp.str());
        }
}

void TestSelectRequestOr() {
  std::map<std::string, std::string> data = {
      {"group", "4"}, {"rating", "4.15"}, {"name", "\"Ivan\""}};
  for (const auto &[field1, value1] : data)
    for (const auto &[field2, value2] : data)
      for (const auto &op1 : {"==", "!=", ">", ">=", "<", "<="})
        for (const auto &op2 : {"==", "!=", ">", ">=", "<", "<="}) {
          std::stringstream inp;
          std::stringstream exp;
          inp << field1 << op1 << value1 << " OR " << field2 << op2 << value2;
          exp << '(' << field1 << op1 << value1 << " | " << field2 << op2
              << value2 << ')';
          auto ptr = ParseSelectCondition(inp);
          ASSERT(ptr != nullptr);
          std::stringstream out;
          out << *ptr;
          ASSERT_EQUAL(out.str(), exp.str());
        }
}

void TestSelectRequestComplex() {
  std::map<std::string, std::string> data = {
      {"group", "4"}, {"rating", "4.15"}, {"name", "\"Ivan\""}};
  // a AND b OR c
  for (const auto &[field1, value1] : data)
    for (const auto &[field2, value2] : data)
      for (const auto &[field3, value3] : data)
        for (const auto &op1 : {"==", "!=", ">", ">=", "<", "<="})
          for (const auto &op2 : {"==", "!=", ">", ">=", "<", "<="})
            for (const auto &op3 : {"==", "!=", ">", ">=", "<", "<="}) {
              std::stringstream inp;
              std::stringstream exp;
              inp << field1 << op1 << value1 << " AND " << field2 << op2
                  << value2 << " OR " << field3 << op3 << value3;
              exp << "((" << field1 << op1 << value1 << " & " << field2 << op2
                  << value2 << ')' << " | " << field3 << op3 << value3 << ')';
              auto ptr = ParseSelectCondition(inp);
              ASSERT(ptr != nullptr);
              std::stringstream out;
              out << *ptr;
              ASSERT_EQUAL(out.str(), exp.str());
            }
  // a OR b AND c
  for (const auto &[field1, value1] : data)
    for (const auto &[field2, value2] : data)
      for (const auto &[field3, value3] : data)
        for (const auto &op1 : {"==", "!=", ">", ">=", "<", "<="})
          for (const auto &op2 : {"==", "!=", ">", ">=", "<", "<="})
            for (const auto &op3 : {"==", "!=", ">", ">=", "<", "<="}) {
              std::stringstream inp;
              std::stringstream exp;
              inp << field1 << op1 << value1 << " OR " << field2 << op2
                  << value2 << " AND " << field3 << op3 << value3;
              exp << "(" << field1 << op1 << value1 << " | " << '(' << field2
                  << op2 << value2 << " & " << field3 << op3 << value3 << "))";
              auto ptr = ParseSelectCondition(inp);
              ASSERT(ptr != nullptr);
              std::stringstream out;
              out << *ptr;
              ASSERT_EQUAL(out.str(), exp.str());
            }
  // (a OR b) AND c
  for (const auto &[field1, value1] : data)
    for (const auto &[field2, value2] : data)
      for (const auto &[field3, value3] : data)
        for (const auto &op1 : {"==", "!=", ">", ">=", "<", "<="})
          for (const auto &op2 : {"==", "!=", ">", ">=", "<", "<="})
            for (const auto &op3 : {"==", "!=", ">", ">=", "<", "<="}) {
              std::stringstream inp;
              std::stringstream exp;
              inp << '(' << field1 << op1 << value1 << " OR " << field2 << op2
                  << value2 << ')' << " AND " << field3 << op3 << value3;
              exp << "((" << field1 << op1 << value1 << " | " << field2 << op2
                  << value2 << ')' << " & " << field3 << op3 << value3 << ')';
              auto ptr = ParseSelectCondition(inp);
              ASSERT(ptr != nullptr);
              std::stringstream out;
              out << *ptr;
              ASSERT_EQUAL(out.str(), exp.str());
            }
}

void TestSelectRequestBad() {
  bool start_logic = false;
  try {
    std::stringstream inp;
    inp << "AND rating < 5.5";
    auto ptr = ParseSelectCondition(inp);
  } catch (...) {
    start_logic = true;
  }
  ASSERT(start_logic);

  bool double_logic = false;
  try {
    std::stringstream inp;
    inp << "group == 4 AND AND rating < 5.5";
    auto ptr = ParseSelectCondition(inp);
  } catch (...) {
    double_logic = true;
  }
  ASSERT(double_logic);

  bool no_logic = false;
  try {
    std::stringstream inp;
    inp << "group == 4 rating < 5.5";
    auto ptr = ParseSelectCondition(inp);
  } catch (...) {
    no_logic = true;
  }
  ASSERT(no_logic);

  bool no_paren = false;
  try {
    std::stringstream inp;
    inp << "(group == 4 OR rating < 5.5";
    auto ptr = ParseSelectCondition(inp);
  } catch (...) {
    no_paren = true;
  }
  ASSERT(no_paren);

  bool logic_on_end = false;
  try {
    std::stringstream inp;
    inp << "(group == 4 OR rating < 5.5 AND";
    auto ptr = ParseSelectCondition(inp);
  } catch (...) {
    logic_on_end = true;
  }
  ASSERT(logic_on_end);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSelectRequestSimple);
  RUN_TEST(tr, TestSelectRequestAnd);
  RUN_TEST(tr, TestSelectRequestOr);
  RUN_TEST(tr, TestSelectRequestComplex);
  RUN_TEST(tr, TestSelectRequestBad);
  return 0;
}