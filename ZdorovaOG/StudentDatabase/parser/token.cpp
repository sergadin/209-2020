#include "token.h"

#include <stdexcept>

using namespace std;

bool isEqual(std::istream &is, const std::string &s) {
  for (char c : s)
    if (std::tolower(is.get()) != std::tolower(c))
      return false;
  return true;
}

vector<Token> Tokenize(istream &is) {
  vector<Token> tokens;

  char c;
  while (is >> c) {
    if (isdigit(c)) {
      string number(1, c);
      while (isdigit(is.peek())) {
        number += is.get();
      }
      if (is.peek() == '.') {
        number += is.get();
        while (isdigit(is.peek())) {
          number += is.get();
        }
      }
      tokens.push_back({number, TokenType::NUMBER});
    } else if (c == '"') {
      string name;
      getline(is, name, '"');
      tokens.push_back({name, TokenType::NAME});
    } else if (std::isalpha(c)) {
      switch (std::tolower(c)) {
      case 'n':
        if (isEqual(is, "ame"))
          tokens.push_back({"name", TokenType::COLUMN});
        else
          throw logic_error("Unknown token");
        break;
      case 'g':
        if (isEqual(is, "roup"))
          tokens.push_back({"group", TokenType::COLUMN});
        else
          throw logic_error("Unknown token");
        break;
      case 'r':
        if (isEqual(is, "ating"))
          tokens.push_back({"rating", TokenType::COLUMN});
        else
          throw logic_error("Unknown token");
        break;
      case 'a':
        if (isEqual(is, "nd"))
          tokens.push_back({"AND", TokenType::LOGICAL_OP});
        else
          throw logic_error("Unknown token");
        break;
      case 'o':
        if (isEqual(is, "r"))
          tokens.push_back({"OR", TokenType::LOGICAL_OP});
        else
          throw logic_error("Unknown token");
        break;
      case 's':
        if (isEqual(is, "ortby"))
          tokens.push_back({"", TokenType::SORT_OP});
        else
          throw logic_error("Unknown token");
        break;
      case 'e':
        if (isEqual(is, "nd"))
          return tokens;
        else
          throw logic_error("Unknown token");
      default:
        throw logic_error("Unknown token");
      }
    } else if (c == '(') {
      tokens.push_back({"(", TokenType::PAREN_LEFT});
    } else if (c == ')') {
      tokens.push_back({")", TokenType::PAREN_RIGHT});
    } else if (c == '<') {
      if (is.peek() == '=') {
        is.get();
        tokens.push_back({"<=", TokenType::COMPARE_OP});
      } else {
        tokens.push_back({"<", TokenType::COMPARE_OP});
      }
    } else if (c == '>') {
      if (is.peek() == '=') {
        is.get();
        tokens.push_back({">=", TokenType::COMPARE_OP});
      } else {
        tokens.push_back({">", TokenType::COMPARE_OP});
      }
    } else if (c == '=') {
      if (is.get() == '=') {
        tokens.push_back({"==", TokenType::COMPARE_OP});
      } else {
        throw logic_error("Unknown token");
      }
    } else if (c == '!') {
      if (is.get() == '=') {
        tokens.push_back({"!=", TokenType::COMPARE_OP});
      } else {
        throw logic_error("Unknown token");
      }
    }
  }

  return tokens;
}