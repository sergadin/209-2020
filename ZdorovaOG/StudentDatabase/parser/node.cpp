#include "node.h"

#include <algorithm>
#include <iterator>

std::set<size_t> GetIntersection(const std::set<size_t> &x,
                                 const std::set<size_t> &y) {
  std::set<size_t> result;
  std::set_intersection(x.begin(), x.end(), y.begin(), y.end(),
                        std::inserter(result, result.begin()));
  return result;
}

std::set<size_t> GetUnion(const std::set<size_t> &x,
                          const std::set<size_t> &y) {
  std::set<size_t> result;
  std::set_union(x.begin(), x.end(), y.begin(), y.end(),
                 std::inserter(result, result.begin()));
  return result;
}

std::set<size_t> EmptyNode::Process(const Index &) const { return {}; }
std::set<size_t> GroupSelectionNode::Process(const Index &ind) const {
  switch (_cmp) {
    case Comparison::LESS:
      return ind.FindByConditionL(_group);
    case Comparison::LESS_EQUAL:
      return ind.FindByConditionLE(_group);
    case Comparison::GREATER:
      return ind.FindByConditionG(_group);
    case Comparison::GREATER_EQUAL:
      return ind.FindByConditionGE(_group);
    case Comparison::EQUAL:
      return ind.FindByConditionE(_group);
    case Comparison::NOT_EQUAL:
      return ind.FindByConditionNE(_group);
    default:
      throw std::logic_error("Unknown comparison operation");
  }
}
std::set<size_t> NameSelectionNode::Process(const Index &ind) const {
  switch (_cmp) {
    case Comparison::LESS:
      return ind.FindByConditionL(_name);
    case Comparison::LESS_EQUAL:
      return ind.FindByConditionLE(_name);
    case Comparison::GREATER:
      return ind.FindByConditionG(_name);
    case Comparison::GREATER_EQUAL:
      return ind.FindByConditionGE(_name);
    case Comparison::EQUAL:
      return ind.FindByConditionE(_name);
    case Comparison::NOT_EQUAL:
      return ind.FindByConditionNE(_name);
    default:
      throw std::logic_error("Unknown comparison operation");
  }
}
std::set<size_t> RatingSelectionNode::Process(const Index &ind) const {
  switch (_cmp) {
    case Comparison::LESS:
      return ind.FindByConditionL(_rating);
    case Comparison::LESS_EQUAL:
      return ind.FindByConditionLE(_rating);
    case Comparison::GREATER:
      return ind.FindByConditionG(_rating);
    case Comparison::GREATER_EQUAL:
      return ind.FindByConditionGE(_rating);
    case Comparison::EQUAL:
      return ind.FindByConditionE(_rating);
    case Comparison::NOT_EQUAL:
      return ind.FindByConditionNE(_rating);
    default:
      throw std::logic_error("Unknown comparison operation");
  }
}
std::set<size_t> LogicalOperationNode::Process(const Index &ind) const {
  if (_op == LogicalOperation::AND) {
    return GetIntersection(_lhs->Process(ind), _rhs->Process(ind));
  }
  if (_op == LogicalOperation::OR) {
    return GetUnion(_lhs->Process(ind), _rhs->Process(ind));
  }
  return {};
}

std::string GetCompNotation(const Comparison &cmp) {
  if (cmp == Comparison::LESS)
    return "<";
  else if (cmp == Comparison::LESS_EQUAL)
    return "<=";
  else if (cmp == Comparison::GREATER)
    return ">";
  else if (cmp == Comparison::GREATER_EQUAL)
    return ">=";
  else if (cmp == Comparison::EQUAL)
    return "==";
  else if (cmp == Comparison::NOT_EQUAL)
    return "!=";
  else
    return "<unknown sign>";
}

std::ostream &EmptyNode::Print(std::ostream &out) const { return out; }
std::ostream &GroupSelectionNode::Print(std::ostream &out) const {
  out << "group" << GetCompNotation(_cmp) << _group;
  return out;
}
std::ostream &RatingSelectionNode::Print(std::ostream &out) const {
  out << "rating" << GetCompNotation(_cmp) << _rating;
  return out;
}
std::ostream &NameSelectionNode::Print(std::ostream &out) const {
  out << "name" << GetCompNotation(_cmp) << '\"' << _name << '\"';
  return out;
}
std::ostream &LogicalOperationNode::Print(std::ostream &out) const {
  out << '(';
  _lhs->Print(out);
  if (_op == LogicalOperation::AND) {
    out << " & ";
  }
  if (_op == LogicalOperation::OR) {
    out << " | ";
  }
  _rhs->Print(out);
  out << ')';
  return out;
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
  return node.Print(os);
}