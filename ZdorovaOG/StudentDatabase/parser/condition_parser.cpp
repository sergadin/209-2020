#include "condition_parser.h"

#include <map>
#include <stdexcept>
#include <vector>

#include "token.h"
using namespace std;

template <class It>
shared_ptr<Node> ParseComparison(It &current, It end) {
  if (current == end) {
    throw logic_error("1. Expected column name: name, group or rating");
  }

  Token &column = *current;
  if (column.type != TokenType::COLUMN) {
    throw logic_error("2. Expected column name: name, group or rating");
  }
  ++current;

  if (current == end) {
    throw logic_error("1. Expected comparison operation");
  }

  Token &op = *current;
  if (op.type != TokenType::COMPARE_OP) {
    throw logic_error("2. Expected comparison operation");
  }
  ++current;

  if (current == end) {
    throw logic_error("Expected right value of comparison");
  }

  Comparison cmp;
  if (op.value == "<") {
    cmp = Comparison::LESS;
  } else if (op.value == "<=") {
    cmp = Comparison::LESS_EQUAL;
  } else if (op.value == ">") {
    cmp = Comparison::GREATER;
  } else if (op.value == ">=") {
    cmp = Comparison::GREATER_EQUAL;
  } else if (op.value == "==") {
    cmp = Comparison::EQUAL;
  } else if (op.value == "!=") {
    cmp = Comparison::NOT_EQUAL;
  } else {
    throw logic_error("Unknown comparison token: " + op.value);
  }

  const string &value = current->value;
  ++current;

  if (column.value == "group") {
    return make_shared<GroupSelectionNode>(cmp, std::stoi(value));
  } else if (column.value == "rating") {
    return make_shared<RatingSelectionNode>(cmp, std::stod(value));
  } else {
    return make_shared<NameSelectionNode>(cmp, value);
  }
}

template <class It>
shared_ptr<Node> ParseExpression(It &current, It end, unsigned precedence) {
  if (current == end) {
    return shared_ptr<Node>();
  }

  shared_ptr<Node> left;

  if (current->type == TokenType::PAREN_LEFT) {
    ++current;  // consume '('
    left = ParseExpression(current, end, 0u);
    if (current == end || current->type != TokenType::PAREN_RIGHT) {
      throw logic_error("Missing right paren");
    }
    ++current;  // consume ')'
  } else {
    left = ParseComparison(current, end);
  }

  const map<LogicalOperation, unsigned> precedences = {
      {LogicalOperation::OR, 1}, {LogicalOperation::AND, 2}};

  while (current != end && current->type != TokenType::PAREN_RIGHT) {
    if (current->type != TokenType::LOGICAL_OP) {
      throw logic_error("Expected logic operation");
    }

    const auto logical_operation =
        current->value == "AND" ? LogicalOperation::AND : LogicalOperation::OR;
    const auto current_precedence = precedences.at(logical_operation);
    if (current_precedence <= precedence) {
      break;
    }

    ++current;  // consume op

    left = make_shared<LogicalOperationNode>(
        logical_operation, left,
        ParseExpression(current, end, current_precedence));
  }

  return left;
}

std::shared_ptr<Node> ParseExpression(istream &is) {
  auto tokens = Tokenize(is);  // vector<Token>
  auto current = tokens.begin();
  auto top_node = ParseExpression(current, tokens.end(), 0u);
  if (!top_node) {
    top_node = make_shared<EmptyNode>();
  }

  if (current != tokens.end()) {
    throw logic_error("1. Unexpected tokens after condition");
  }

  return top_node;
}

std::pair<std::vector<std::string>, std::string> ParseFieldList(
    std::istream &is) {
  std::vector<std::string> fields;
  std::string sortby = "id";
  auto tokens = Tokenize(is);
  auto current = tokens.begin();
  for (; current != tokens.end(); ++current) {
    if (current->type == TokenType::SORT_OP) break;
    if (current->type == TokenType::COLUMN)
      fields.push_back(current->value);
    else
      throw logic_error("3. Expected column name: name, group, rating or info");
  }
  if (current != tokens.end() && ++current != tokens.end()) {
    if (current->type == TokenType::COLUMN)
      sortby = current->value;
    else
      throw logic_error("4. Expected column name: name, group or rating");
    ++current;
  }
  if (current != tokens.end())
    throw logic_error("2. Unexpected tokens after condition");

  return {fields, sortby};
}

std::string ParseString(std::istream &is) {
  auto token = ReadToken(is);
  if (token.type == TokenType::STRING)
    return token.value;
  else
    throw logic_error("Expected string");
}

int ParseInteger(std::istream &is) {
  auto token = ReadToken(is);
  if (token.type == TokenType::INTEGER)
    return std::stoi(token.value);
  else
    throw logic_error("Expected integer number");
}

double ParseFloat(std::istream &is) {
  auto token = ReadToken(is);
  if (token.type == TokenType::FLOAT || token.type == TokenType::INTEGER)
    return std::stod(token.value);
  else
    throw logic_error("Expected real number");
}