#include "index.h"

Index::Index(const Index &other)
    : _ids(other._ids),
      _id_by_name(other._id_by_name),
      _id_by_group(other._id_by_group),
      _id_by_rating(other._id_by_rating) {}

Index &Index::operator=(const Index &other) {
  if (this != &other) {
    _ids = other._ids;
    _id_by_name = other._id_by_name;
    _id_by_group = other._id_by_group;
    _id_by_rating = other._id_by_rating;
  }
  return *this;
}

Index &Index::operator=(Index &&other) {
  if (this != &other) {
    _ids = std::move(other._ids);
    _id_by_name = std::move(other._id_by_name);
    _id_by_group = std::move(other._id_by_group);
    _id_by_rating = std::move(other._id_by_rating);
  }
  return *this;
}

bool Index::Insert(size_t id, const Student &s) {
  if (!_ids.insert(id).second) return false;
  _id_by_name[s.name].insert(id);
  _id_by_group[s.group].insert(id);
  _id_by_rating[s.rating].insert(id);
  return true;
}

bool Index::Remove(size_t id, const Student &s) {
  if (!HasId(id)) return false;
  _ids.erase(id);
  {
    auto it = _id_by_name.find(s.name);
    if (it->second.size() == 1)
      _id_by_name.erase(it);
    else
      it->second.erase(id);
  }
  {
    auto it = _id_by_group.find(s.group);
    if (it->second.size() == 1)
      _id_by_group.erase(it);
    else
      it->second.erase(id);
  }
  {
    auto it = _id_by_rating.find(s.rating);
    if (it->second.size() == 1)
      _id_by_rating.erase(it);
    else
      it->second.erase(id);
  }
  return true;
}

void Index::Clear() {
  _ids.clear();
  _id_by_group.clear();
  _id_by_name.clear();
  _id_by_rating.clear();
}

bool Index::HasId(size_t id) const { return _ids.count(id) > 0; }

template <class T>
std::set<size_t> CollectL(const std::map<T, std::set<size_t>> &container,
                          const T &value) {
  std::set<size_t> result;
  auto first_greater = container.upper_bound(value);
  if (first_greater != container.begin() &&
      std::prev(first_greater)->first == value)
    --first_greater;
  for (auto it = container.begin(); it != first_greater; ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}

bool IsEqual(double a, double b, double eps = 1e-10) {
  return std::abs(a - b) < eps;
}

template <>
std::set<size_t> CollectL(const std::map<double, std::set<size_t>> &container,
                          const double &value) {
  std::set<size_t> result;
  auto first_greater = container.upper_bound(value);
  if (first_greater != container.begin() &&
      IsEqual(std::prev(first_greater)->first, value))
    --first_greater;
  for (auto it = container.begin(); it != first_greater; ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}
std::set<size_t> Index::FindByConditionL(const std::string &name) const {
  return CollectL(_id_by_name, name);
}
std::set<size_t> Index::FindByConditionL(int group) const {
  return CollectL(_id_by_group, group);
}
std::set<size_t> Index::FindByConditionL(double rating) const {
  return CollectL(_id_by_rating, rating);
}

template <class T>
std::set<size_t> CollectLE(const std::map<T, std::set<size_t>> &container,
                           const T &value) {
  std::set<size_t> result;
  auto first_greater = container.upper_bound(value);
  for (auto it = container.begin(); it != first_greater; ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}
std::set<size_t> Index::FindByConditionLE(const std::string &name) const {
  return CollectLE(_id_by_name, name);
}
std::set<size_t> Index::FindByConditionLE(int group) const {
  return CollectLE(_id_by_group, group);
}
std::set<size_t> Index::FindByConditionLE(double rating) const {
  return CollectLE(_id_by_rating, rating);
}

template <class T>
std::set<size_t> CollectG(const std::map<T, std::set<size_t>> &container,
                          const T &value) {
  std::set<size_t> result;
  auto first_greater_or_equal = container.lower_bound(value);
  if (first_greater_or_equal != container.end() &&
      std::next(first_greater_or_equal)->first == value)
    ++first_greater_or_equal;
  for (auto it = first_greater_or_equal; it != container.end(); ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}
template <>
std::set<size_t> CollectG(const std::map<double, std::set<size_t>> &container,
                          const double &value) {
  std::set<size_t> result;
  auto first_greater_or_equal = container.lower_bound(value);
  if (first_greater_or_equal != container.end() &&
      IsEqual(std::next(first_greater_or_equal)->first, value))
    ++first_greater_or_equal;
  for (auto it = first_greater_or_equal; it != container.end(); ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}

std::set<size_t> Index::FindByConditionG(const std::string &name) const {
  return CollectG(_id_by_name, name);
}
std::set<size_t> Index::FindByConditionG(int group) const {
  return CollectG(_id_by_group, group);
}
std::set<size_t> Index::FindByConditionG(double rating) const {
  return CollectG(_id_by_rating, rating);
}

template <class T>
std::set<size_t> CollectGE(const std::map<T, std::set<size_t>> &container,
                           const T &value) {
  std::set<size_t> result;
  auto first_greater_or_equal = container.lower_bound(value);
  for (auto it = first_greater_or_equal; it != container.end(); ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}
std::set<size_t> Index::FindByConditionGE(const std::string &name) const {
  return CollectGE(_id_by_name, name);
}
std::set<size_t> Index::FindByConditionGE(int group) const {
  return CollectGE(_id_by_group, group);
}
std::set<size_t> Index::FindByConditionGE(double rating) const {
  return CollectGE(_id_by_rating, rating);
}

std::set<size_t> Index::FindByConditionE(const std::string &name) const {
  if (!name.empty() && name.back() == '*') {
    auto from = name;
    from.pop_back();
    auto to = from;
    to.back() += 1;
    return FindByConditionIn(from, to);
  }
  return _id_by_name.count(name) > 0 ? _id_by_name.at(name)
                                     : std::set<size_t>();
}
std::set<size_t> Index::FindByConditionE(int group) const {
  return _id_by_group.count(group) > 0 ? _id_by_group.at(group)
                                       : std::set<size_t>();
}
std::set<size_t> Index::FindByConditionE(double rating) const {
  return _id_by_rating.count(rating) > 0 ? _id_by_rating.at(rating)
                                         : std::set<size_t>();
}
std::set<size_t> Index::FindByConditionNE(const std::string &name) const {
  if (!name.empty() && name.back() == '*') {
    auto from = name;
    from.pop_back();
    auto to = from;
    to.back() += 1;
    return FindByConditionNIn(from, to);
  }
  std::set<size_t> result = _ids;
  if (_id_by_name.count(name) > 0)
    result.erase(_id_by_name.at(name).begin(), _id_by_name.at(name).end());
  return result;
}
std::set<size_t> Index::FindByConditionNE(int group) const {
  std::set<size_t> result = _ids;
  if (_id_by_group.count(group) > 0)
    result.erase(_id_by_group.at(group).begin(), _id_by_group.at(group).end());
  return result;
}
std::set<size_t> Index::FindByConditionNE(double rating) const {
  std::set<size_t> result = _ids;
  if (_id_by_rating.count(rating) > 0)
    result.erase(_id_by_rating.at(rating).begin(),
                 _id_by_rating.at(rating).end());
  return result;
}
// Get set of ids, with names in given range
std::set<size_t> Index::FindByConditionIn(const std::string &from,
                                          const std::string &to) const {
  std::set<size_t> result;
  auto _from = _id_by_name.lower_bound(from);
  auto _to = _id_by_name.upper_bound(to);
  for (auto it = _from; it != _to; ++it) {
    const auto &set = it->second;
    result.insert(set.begin(), set.end());
  }
  return result;
}

std::set<size_t> Index::FindByConditionNIn(const std::string &from,
                                           const std::string &to) const {
  std::set<size_t> result = _ids;
  auto _from = _id_by_name.lower_bound(from);
  auto _to = _id_by_name.upper_bound(to);
  for (auto it = _from; it != _to; ++it) {
    const auto &set = it->second;
    result.erase(set.begin(), set.end());
  }
  return result;
}

const std::set<size_t> &Index::GetIds() const { return _ids; }

const std::map<std::string, std::set<size_t>> &Index::GetNameMap() const {
  return _id_by_name;
}
const std::map<int, std::set<size_t>> &Index::GetGroupMap() const {
  return _id_by_group;
}
const std::map<double, std::set<size_t>> &Index::GetRatingMap() const {
  return _id_by_rating;
}