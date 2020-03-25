#pragma once

#include <memory>
#include <ostream>
#include <set>
#include <string>

#include "../database/index.h"

enum class Comparison {
  Less,
  LessOrEqual,
  Greater,
  GreaterOrEqual,
  Equal,
  NotEqual
};

enum class LogicalOperation { Or, And };

/**
 * @brief      Базовый узел дерева запросов
 */
class Node {
public:
  /**
   * @brief      Выводит в поток содержимое узла
   *
   * @param      out   Поток вывода
   *
   * @return     Поток вывода
   */
  virtual std::ostream &Print(std::ostream &out) const = 0;

  /**
   * @brief      Производит операцию в узле
   *
   * @param[in]  ind   Индекс баззы данных
   *
   * @return     Список ИД студентов
   */
  virtual std::set<size_t> Process(const Index &ind) const = 0;
  virtual ~Node() {}
};

/**
 * @brief      Пустой узел
 */
class EmptyNode : public Node {
public:
  EmptyNode() {}
  std::ostream &Print(std::ostream &out) const override;
  std::set<size_t> Process(const Index &ind) const override;
};

/**
 * @brief      Узел выбора группы
 */
class GroupSelectionNode : public Node {
public:
  GroupSelectionNode(const Comparison &cmp, int group)
      : _cmp(cmp), _group(group) {}
  std::ostream &Print(std::ostream &out) const override;
  std::set<size_t> Process(const Index &ind) const override;

private:
  const Comparison _cmp;
  const int _group;
};

/**
 * @brief      Узел выбора рейтинга
 */
class RatingSelectionNode : public Node {
public:
  RatingSelectionNode(const Comparison &cmp, double rating)
      : _cmp(cmp), _rating(rating) {}
  std::ostream &Print(std::ostream &out) const override;
  std::set<size_t> Process(const Index &ind) const override;

private:
  const Comparison _cmp;
  const double _rating;
};

/**
 * @brief      Узел выбора имени
 */
class NameSelectionNode : public Node {
public:
  NameSelectionNode(const Comparison &cmp, const std::string &name)
      : _cmp(cmp), _name(name) {}
  std::ostream &Print(std::ostream &out) const override;
  std::set<size_t> Process(const Index &ind) const override;

private:
  const Comparison _cmp;
  const std::string _name;
};

/**
 * @brief      Узел логической операции
 */
class LogicalOperationNode : public Node {
public:
  LogicalOperationNode(const LogicalOperation &op,
                       const std::shared_ptr<Node> lhs,
                       const std::shared_ptr<Node> rhs)
      : _op(op), _lhs(lhs), _rhs(rhs) {}
  std::ostream &Print(std::ostream &out) const override;
  std::set<size_t> Process(const Index &ind) const override;

private:
  const LogicalOperation _op;
  const std::shared_ptr<Node> _lhs;
  const std::shared_ptr<Node> _rhs;
};

/**
 * @brief      Оператор вывода узла в поток
 *
 * @param      os    Поток вывода
 * @param[in]  node  Узел
 *
 * @return     Поток вывода
 */
std::ostream &operator<<(std::ostream &os, const Node &node);