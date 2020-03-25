#pragma once

#include <sstream>
#include <vector>

enum class TokenType {
  NUMBER,
  NAME,
  COLUMN,
  LOGICAL_OP,
  COMPARE_OP,
  SORT_OP,
  PAREN_LEFT,
  PAREN_RIGHT
};

struct Token {
  const std::string value;
  const TokenType type;
};

/**
 * @brief      Считывает из потока ввода и разбивает на токены
 *
 * @param      is    Поток ввода
 *
 * @return     Список токенов
 */
std::vector<Token> Tokenize(std::istream &is);