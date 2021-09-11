#pragma once

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
#include <set>

struct Student {
  std::string name;
  int group;
  double rating;
  std::string info;
  bool operator==(const Student &other) {
    return std::tie(name, group, rating, info) ==
           std::tie(other.name, other.group, other.rating, other.info);
  }
  bool operator!=(const Student &other) { return !(*this == other); }
};

std::ostream &operator<<(std::ostream &os, const Student &s);

struct GroupInfo {
  std::list<Student> students_by_rating;
  using student_it = std::list<Student>::iterator;
  std::map<std::string, std::vector<student_it>> it_by_name;
};

class DataBase {
 private:
  std::set<int> Users = {0};  //new

  //std::unordered_map<int, GroupInfo> group_data;
  std::map<int, std::unordered_map<int, GroupInfo> > group_data_copied;  //new
  std::map<int, std::unordered_map<int, GroupInfo> > buffer;  //new

 public:
  DataBase() = default;
  ~DataBase() = default;
  
  void Clear(int user_id) {  //new
    for (auto it = group_data_copied[user_id].begin(); it != group_data_copied[user_id].end();) {
      it = group_data_copied[user_id].erase(it);
    }
  }

  int RegisterUser() {  //new
    int last_id = *Users.rbegin();
    last_id = last_id + 1;
    buffer[last_id] = group_data_copied[last_id] = group_data_copied[0];
    Users.insert(last_id);
    return last_id;
  }

  void Renew_Buffer(int user_id) {
    buffer[user_id] = group_data_copied[user_id];
  }

  void DeleteUser(int user_id) {  //new
    group_data_copied.erase(user_id);
    buffer.erase(user_id);
    Users.erase(user_id);
  }

  void Insert(const Student &s, int user_id) {
    auto &group = buffer[user_id][s.group];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end() && student_it->rating < s.rating)
      ++student_it;
    student_it = students.insert(student_it, s);
    group.it_by_name[s.name].push_back(student_it);
  }

  GroupInfo::student_it Remove(const Student &s, int user_id) {
    auto &group = buffer[user_id][s.group];
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

  void PrintAll(std::ostream &os, int user_id, bool n, bool g, bool r, bool i) {  //VERY DANGEROUS
    for (const auto &[group_id, group] : buffer[user_id])
      for (auto it : group.students_by_rating) {
          if (n == true) os << (it).name << ',';
          if (g == true) os << (it).group << ',';
          if (r == true) os << (it).rating << ',';
          if (i == true) os << (it).info;
          os << '\n';
        }
  }

  /*void PrintName(std::ostream &os, int user_id) {
    for (const auto &[group_id, group] : buffer[user_id])
      for (const auto &[name, it_vector] : group.it_by_name)
        for (auto it : it_vector) os << (*it).name << '\n';
  }
  void PrintGroup(std::ostream &os, int user_id) {
    for (const auto &[group_id, group] : buffer[user_id])
      for (const auto &[name, it_vector] : group.it_by_name)
        for (auto it : it_vector) os << (*it).group << '\n';
  }
  void PrintRating(std::ostream &os, int user_id) {
    for (const auto &[group_id, group] : buffer[user_id])
      for (const auto &[name, it_vector] : group.it_by_name)
        for (auto it : it_vector) os << (*it).rating << '\n';
  }*/


  void SelectName(std::string str, int user_id);
  void SelectGroupMore(int g, int user_id);
  void SelectGroupLess(int g, int user_id);
  void SelectRatingMore(double r, int user_id);
  void SelectRatingLess(double r, int user_id);

  void Load(const std::string &filename, int user_id);
  void Save(const std::string &filename, int user_id);

  void SortName(std::ostream &os, int user_id, bool n, bool g, bool r, bool i);
  void SortGroup(std::ostream &os, int user_id, bool n, bool g, bool r, bool i);
  void SortRating(std::ostream &os, int user_id, bool n, bool g, bool r, bool i);

  int Process(std::istream &is, std::ostream &os, int user_id);
};