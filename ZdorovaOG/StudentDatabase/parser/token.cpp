#include "token.h"

#include <stdexcept>

using namespace std;

bool isEqual(std::istream &is, const std::string &s) {
  for (char c : s) {
    char ic = is.peek();
    if (std::tolower(ic) != std::tolower(c)) return false;
    is.ignore();
  }
  return true;
}

vector<Token> Tokenize(istream &is) {
  vector<Token> tokens;
  while (is) {
    auto token = ReadToken(is);
    if (token.type == TokenType::VOID) continue;
    if (token.type == TokenType::END)
      break;
    else
      tokens.push_back(std::move(token));
  }

  return tokens;
}

Token ReadToken(istream &is) {
  char c;
  while (is >> c) {
    if (isdigit(c)) {
      bool is_float = false;
      string number(1, c);
      while (isdigit(is.peek())) {
        number += is.get();
      }
      if (is.peek() == '.') {
        is_float = true;
        number += is.get();
        while (isdigit(is.peek())) {
          number += is.get();
        }
      }
      return {number, is_float ? TokenType::FLOAT : TokenType::INTEGER};
    } else if (c == '"') {
      string name;
      getline(is, name, '"');
      return {name, TokenType::STRING};
    } else if (std::isalpha(c)) {
      switch (std::tolower(c)) {
        case 'i':
          if (isEqual(is, "d"))
            return {"id", TokenType::COLUMN};
          else {
            if (isEqual(is, "nfo"))
              return {"info", TokenType::COLUMN};
            else
              throw logic_error("Unknown token. Expected: info");
          }
          break;
        case 'n':
          if (isEqual(is, "ame"))
            return {"name", TokenType::COLUMN};
          else
            throw logic_error("Unknown token. Expected: name");
          break;
        case 'g':
          if (isEqual(is, "roup"))
            return {"group", TokenType::COLUMN};
          else
            throw logic_error("Unknown token. Expected: group");
          break;
        case 'r':
          if (isEqual(is, "ating"))
            return {"rating", TokenType::COLUMN};
          else
            throw logic_error("Unknown token. Expected: rating");
          break;
        case 'a':
          if (isEqual(is, "nd"))
            return {"AND", TokenType::LOGICAL_OP};
          else
            throw logic_error("Unknown token. Expected: and");
          break;
        case 'o':
          if (isEqual(is, "r"))
            return {"OR", TokenType::LOGICAL_OP};
          else
            throw logic_error("Unknown token. Expected: or");
          break;
        case 's':
          if (isEqual(is, "ortby"))
            return {"", TokenType::SORT_OP};
          else
            throw logic_error("Unknown token. Expected: sortby");
          break;
        case 'e':
          if (isEqual(is, "nd"))
            return {"", TokenType::END};
          else
            throw logic_error("Unknown token. Expected: end");
        default:
          throw logic_error("Unknown token");
      }
    } else if (c == '(') {
      return {"(", TokenType::PAREN_LEFT};
    } else if (c == ')') {
      return {")", TokenType::PAREN_RIGHT};
    } else if (c == '<') {
      if (is.peek() == '=') {
        is.get();
        return {"<=", TokenType::COMPARE_OP};
      } else {
        return {"<", TokenType::COMPARE_OP};
      }
    } else if (c == '>') {
      if (is.peek() == '=') {
        is.get();
        return {">=", TokenType::COMPARE_OP};
      } else {
        return {">", TokenType::COMPARE_OP};
      }
    } else if (c == '=') {
      if (is.get() == '=') {
        return {"==", TokenType::COMPARE_OP};
      } else {
        throw logic_error("Unknown token. Expected: ==");
      }
    } else if (c == '!') {
      if (is.get() == '=') {
        return {"!=", TokenType::COMPARE_OP};
      } else {
        throw logic_error("Unknown token. Expected: !=");
      }
    }
  }
  return {"", TokenType::VOID};
}