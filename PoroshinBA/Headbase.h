#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

struct Student {
  std::string name;
  int group;
  double rating;
  bool operator==(const Student &other) {
    return std::tie(name, group, rating) ==
           std::tie(other.name, other.group, other.rating);
  }
  bool operator!=(const Student &other) { return !(*this == other); }
};

std::ostream &operator<<(std::ostream &os, const Student &s) {
  os << s.name << ',' << s.group << ',' << s.rating;
  return os;
}

struct GroupInfo {
  std::list<Student> students_by_rating;
  using student_it = std::list<Student>::iterator;
  std::map<std::string, std::vector<student_it>> it_by_name;
};

class DataBase {
 private:
  std::unordered_map<int, GroupInfo> group_data;

 public:
  DataBase() = default;
  ~DataBase() = default;
  void Insert(const Student &s) {
    auto &group = group_data[s.group];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end() && student_it->rating < s.rating)
      ++student_it;
    student_it = students.insert(student_it, s);
    group.it_by_name[s.name].push_back(student_it);
  }

  GroupInfo::student_it Remove(const Student &s) {
    auto &group = group_data[s.group];
    auto &students = group.students_by_rating;
    auto student_it1 = students.begin();
    while (student_it1 != students.end() && *student_it1 != s) ++student_it1;
    auto student_it2 = student_it1;
    while (student_it2 != students.end() && *student_it2 == s) ++student_it2;
    auto is_in_range = [student_it1,
                        student_it2](GroupInfo::student_it student_it) {
      for (auto it = student_it1; it != student_it2; ++it)
        if (it == student_it) return true;
      return false;
    };
    auto &same_name = group.it_by_name[s.name];
    for (size_t i = 0, j = same_name.size(); i < j; ++i) {
      if (is_in_range(same_name[i])) {
        same_name[i] = same_name.back();
        same_name.pop_back();
        --j;
      }
    }
    if (group.it_by_name[s.name].empty()) group.it_by_name.erase(s.name);
    return students.erase(student_it1, student_it2);
  }

  void PrintAll(std::ostream &os) {
    for (const auto &[group_id, group] : group_data)
      for (const auto &[name, it_vector] : group.it_by_name)
        for (auto it : it_vector) os << *it << '\n';
  }

  void RawEditionSelectName(std::string str);
  void SelectGroup(int g);
  void SelectRating(double r);

  void Load(const std::string &filename);
  void Save(const std::string &filename);

  void SortName(std::ostream &os);
  void SortGroup(std::ostream &os);
  void SortRating(std::ostream &os);

  void Process(std::istream &is, std::ostream &os);
};