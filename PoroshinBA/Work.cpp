#include "Headbase.h"

void DataBase::SelectName(std::string str, int user_id) {
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      //std::string name = stud.name;
      std::size_t found = stud.name.find(str);
      if (found == std::string::npos)
      //size_t a = str.length();
      //if (str != name.substr(0, a))
        student_it = Remove(stud, user_id);
      else
        student_it++;
    }
  }
}

void DataBase::SelectGroupMore(int g, int user_id) {
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end();) {
    if (it->first <= g)
      it = buffer[user_id].erase(it);
    else
      it++;
  }
}

void DataBase::SelectGroupLess(int g, int user_id) {
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end();) {
    if (it->first >= g)
      it = buffer[user_id].erase(it);
    else
      it++;
  }
}

void DataBase::SelectRatingMore(double r, int user_id) {
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      if (stud.rating < r)
        student_it = Remove(stud, user_id);
      else
        student_it++;
    }
  }
}

void DataBase::SelectRatingLess(double r, int user_id) {
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      if (stud.rating > r)
        student_it = Remove(stud, user_id);
      else
        student_it++;
    }
  }
}

void DataBase::Load(const std::string &filename, int user_id) {
  Clear(user_id);
  std::ifstream input;
  input.open(filename);
  Student Bogdan;
  std::string buff;
  while(input.peek() >= 0) {
    std::getline(input, Bogdan.name, ',');
    if(!input) break;
    std::getline(input, buff, ',');
    Bogdan.group = std::stoi(buff);
    std::getline(input, buff, ',');
    Bogdan.rating = std::stod(buff);
    std::getline(input, Bogdan.info);
    Insert(Bogdan, user_id);
  }
  group_data_copied[user_id] = buffer[user_id];
  input.close();
}

void DataBase::Save(const std::string &filename, int user_id) {
  std::ofstream file;
  file.open(filename);
  PrintAll(file, user_id, 1, 1, 1, 1);
  file.close();
}

bool compareNames(Student A, Student B) {
  return (A.name < B.name);
}
bool compareGroups(Student A, Student B) {
  return (A.group < B.group);
}
bool compareRatings(Student A, Student B) {
  return (A.rating < B.rating);
}

void DataBase::SortName(std::ostream &os, int user_id, bool n, bool g, bool r, bool i) {
  std::vector<Student> Names;
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Names.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Names.begin(), Names.end(), compareNames);
  for (auto iter = Names.begin(); iter != Names.end(); iter++){
    if (n == true) os << (*iter).name << ',';
    if (g == true) os << (*iter).group << ',';
    if (r == true) os << (*iter).rating << ',';
    if (i == true) os << (*iter).info;
    os << '\n';
  }
}

void DataBase::SortGroup(std::ostream &os, int user_id, bool n, bool g, bool r, bool i) {
  std::vector<Student> Groups;
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Groups.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Groups.begin(), Groups.end(), compareGroups);
  for (auto iter = Groups.begin(); iter != Groups.end(); iter++){
    if (n == true) os << (*iter).name << ',';
    if (g == true) os << (*iter).group << ',';
    if (r == true) os << (*iter).rating << ',';
    if (i == true) os << (*iter).info;
    os << '\n';
  }
}

void DataBase::SortRating(std::ostream &os, int user_id, bool n, bool g, bool r, bool i) {
  std::vector<Student> Ratings;
  for (auto it = buffer[user_id].begin(); it != buffer[user_id].end(); it++) {
    auto &group = buffer[user_id][it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Ratings.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Ratings.begin(), Ratings.end(), compareRatings);
  for (auto iter = Ratings.begin(); iter != Ratings.end(); iter++){
    if (n == true) os << (*iter).name << ',';
    if (g == true) os << (*iter).group << ',';
    if (r == true) os << (*iter).rating << ',';
    if (i == true) os << (*iter).info;
    os << '\n';
  }
}

int DataBase::Process(std::istream &is, std::ostream &os, int user_id) {
  std::string query;
  is >> query;
  for (auto &c : query) c = std::tolower(c);
  if (query == "select") {
    is >> query;
    for (auto &d : query) d = std::tolower(d);

    if (query == "name") {
      is >> query;
      SelectName(query, user_id); 
    } else if (query == "group") {
      is >> query;
      for (auto &c : query) c = std::tolower(c);
      if (query == "between") {
        is >> query;
        int l = stoi(query);
        is >> query;
        int m = stoi(query);
        SelectGroupMore(l, user_id);
        SelectGroupLess(m, user_id);
      } else if (query == "more") {
        is >> query;
        double r = stoi(query);
        SelectGroupMore(r, user_id);
      } else if (query == "less") {
        is >> query;
        double r = stoi(query);
        SelectGroupLess(r, user_id);
      } 
      else {os << "Incorrect query" << '\n';}

    } else if (query == "rating") {
      is >> query;
      for (auto &c : query) c = std::tolower(c);
      if (query == "between") {
        is >> query;
        double l = stod(query);
        is >> query;
        double m = stod(query);
        SelectRatingMore(l, user_id);
        SelectRatingLess(m, user_id);
      } else if (query == "more") {
        is >> query;
        double r = stod(query);
        SelectRatingMore(r, user_id);
      } else if (query == "less") {
        is >> query;
        double r = stod(query);
        SelectRatingLess(r, user_id);
      } 
      else {os << "Incorrect query" << '\n';}
    }
    else {os << "Incorrect query" << '\n';}
  }

  else if (query == "sort") {
    is >> query;
    for (auto &d : query) d = std::tolower(d);

    if (query == "name") {
      is >> query;
      int choise = stoi(query);
      bool r = choise % 10;
      choise = choise / 10;
      bool g = choise % 10;
      choise = choise / 10;
      bool n = choise % 10;
      if (n == false && g == false && r == false) {
        os << "Choose parameters for print" << '\n';
        return 0;
      }
      SortName(os, user_id, n, g, r, 1);
    } else if (query == "group") {
      is >> query;
      int choise = stoi(query);
      bool r = choise % 10;
      choise = choise / 10;
      bool g = choise % 10;
      choise = choise / 10;
      bool n = choise % 10;
      if (n == false && g == false && r == false) {
        os << "Choose parameters for print" << '\n';
        return 0;
      }
      SortGroup(os, user_id, n, g, r, 1);
    } else if (query == "rating") {
      is >> query;
      int choise = stoi(query);
      bool r = choise % 10;
      choise = choise / 10;
      bool g = choise % 10;
      choise = choise / 10;
      bool n = choise % 10;
      if (n == false && g == false && r == false) {
        os << "Choose parameters for print" << '\n';
        return 0;
      }
      SortRating(os, user_id, n, g, r, 1);
    } else {os << "Incorrect query" << '\n';}
  }

  else if (query == "save") {
    is >> query;
    Save(query, user_id);
  }

  else if (query == "load") {
    is >> query;
    Load(query, user_id);
  }

  else if (query == "print") {
    is >> query;
    int choise = stoi(query);
    bool r = choise % 10;
    choise = choise / 10;
    bool g = choise % 10;
    choise = choise / 10;
    bool n = choise % 10;
    if (n == false && g == false && r == false) {
      os << "Choose parameters for print" << '\n';
      return 0;
    }
    PrintAll(os, user_id, n, g, r, 1);
  }

  else if (query == "insert") {
    Student Bogdan;
    is >> query;
    Bogdan.name = query;
    is >> query;
    Bogdan.group = stoi(query);
    is >> query;
    Bogdan.rating = stod(query);
    Insert(Bogdan, user_id);
  }

  else if (query == "remove") {
    Student Bogdan;
    is >> query;
    Bogdan.name = query;
    is >> query;
    Bogdan.group = stoi(query);
    is >> query;
    Bogdan.rating = stod(query);
    Remove(Bogdan, user_id);
  }

  else if (query == "renew") {
    Renew_Buffer(user_id);
  }

  else if (query == "disconnect") {
    DeleteUser(user_id);
    return 1;
  }

  else if (query == "exit") {
    std::cout << "Server closed" << '\n';
    return -1;
  }
  else {os << "Incorrect query" << '\n';}
  return 0;
}

std::ostream &operator<<(std::ostream &os, const Student &s) {
  os << s.name << ',' << s.group << ',' << s.rating << ',' << s.info;
  return os;
}