#include "Headbase.h"

void DataBase::SelectName(std::string str) {
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      std::string name = stud.name;
      int a = str.length();
      if (str != name.substr(a))
        student_it = Remove(stud);
      else
        student_it++;
    }
  }
}

void DataBase::SelectGroupMore(int g) {
  for (auto it = group_data.begin(); it != group_data.end();) {
    if (it->first < g)
      it = group_data.erase(it);
    else
      it++;
  }
}

void DataBase::SelectGroupLess(int g) {
  for (auto it = group_data.begin(); it != group_data.end();) {
    if (it->first > g)
      it = group_data.erase(it);
    else
      it++;
  }
}

void DataBase::SelectRatingMore(double r) {
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      if (stud.rating < r)
        student_it = Remove(stud);
      else
        student_it++;
    }
  }
}

void DataBase::SelectRatingLess(double r) {
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while (student_it != students.end()) {
      auto stud = *student_it;
      if (stud.rating > r)
        student_it = Remove(stud);
      else
        student_it++;
    }
  }
}

void DataBase::Load(const std::string &filename) {
  std::ifstream input;
  input.open(filename);
  Student Bogdan;
  std::string buff;
  while(input.peek() >= 0) {
    std::getline(input, Bogdan.name, ',');
    if(!input) break;
    std::getline(input, buff, ',');
    Bogdan.group = std::stoi(buff);
    std::getline(input, buff);
    Bogdan.rating = std::stod(buff);
    Insert(Bogdan);
  }
  input.close();
}

void DataBase::Save(const std::string &filename) {
  std::ofstream file;
  file.open(filename);
  PrintAll(file);
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

void DataBase::SortName(std::ostream &os) {
  std::vector<Student> Names;
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Names.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Names.begin(), Names.end(), compareNames);
  for (auto iter = Names.begin(); iter != Names.end(); iter++){
    os << *iter << '\n';
  }
}

void DataBase::SortGroup(std::ostream &os) {
  std::vector<Student> Groups;
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Groups.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Groups.begin(), Groups.end(), compareGroups);
  for (auto iter = Groups.begin(); iter != Groups.end(); iter++){
    os << *iter << '\n';
  }
}

void DataBase::SortRating(std::ostream &os) {
  std::vector<Student> Ratings;
  for (auto it = group_data.begin(); it != group_data.end(); it++) {
    auto &group = group_data[it->first];
    auto &students = group.students_by_rating;
    auto student_it = students.begin();
    while(student_it != students.end()) {
      Ratings.push_back(*student_it);
      student_it++;
    }
  }
  std::sort(Ratings.begin(), Ratings.end(), compareRatings);
  for (auto iter = Ratings.begin(); iter != Ratings.end(); iter++){
    os << *iter << '\n';
  }
}

void DataBase::Process(std::istream &is, std::ostream &os) {  //New unchecked
  std::string query;
  is >> query;
  for (auto &c : query) c = std::tolower(c);
  if (query == "select") {
    is >> query;
    for (auto &d : query) d = std::tolower(d);

    if (query == "name") {
      is >> query;
      SelectName(query); 
    } else if (query == "group") {
      is >> query;
      for (auto &c : query) c = std::tolower(c);
      if (query == "between") {
        is >> query;
        int l = stoi(query);
        is >> query;
        int m = stoi(query);
        SelectGroupMore(l);
        SelectGroupLess(m);
      } else if (query == "more") {
        is >> query;
        double r = stoi(query);
        SelectGroupMore(r);
      } else if (query == "less") {
        is >> query;
        double r = stoi(query);
        SelectGroupLess(r);
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
        SelectRatingMore(l);
        SelectRatingLess(m);
      } else if (query == "more") {
        is >> query;
        double r = stod(query);
        SelectRatingMore(r);
      } else if (query == "less") {
        is >> query;
        double r = stod(query);
        SelectRatingLess(r);
      } 
      else {os << "Incorrect query" << '\n';}
    }
    else {os << "Incorrect query" << '\n';}
  }

  else if (query == "sort") {
    is >> query;
    for (auto &d : query) d = std::tolower(d);

    if (query == "name") {
      SortName(os);
    } else if (query == "group") {
      SortGroup(os);
    } else if (query == "rating") {
      SortRating(os);
    } else {os << "Incorrect query" << '\n';}
  }

  else if (query == "save") {
    is >> query;
    Save(query);
  }

  else if (query == "load") {
    is >> query;
    Load(query);
  }

  else if (query == "print") {
    is >> query;
    if (query == "all") {
      PrintAll(os);
    } else if (query == "name") {
      PrintName(os);
    } else if (query == "group") {
      PrintName(os);
    } else if (query == "rating") {
      PrintName(os);
    } 
    else {os << "Incorrect query" << '\n';}
  }

  else if (query == "insert") {  //Добавить проверки
    Student Bogdan;
    is >> query;
    Bogdan.name = query;
    is >> query;
    Bogdan.group = stoi(query);
    is >> query;
    Bogdan.rating = stod(query);
    Insert(Bogdan);
  }

  else if (query == "remove") {  //Добавить проверки
    Student Bogdan;
    is >> query;
    Bogdan.name = query;
    is >> query;
    Bogdan.group = stoi(query);
    is >> query;
    Bogdan.rating = stod(query);
    Remove(Bogdan);
  }
  else {os << "Incorrect query" << '\n';}
}

int main() {
  DataBase db;
  db.Insert({"A", 1, 0.51});
  db.Insert({"B", 2, 0.25});
  db.Insert({"C", 3, 0.65});
  db.Insert({"D", 1, 0.57});
  db.Insert({"D", 2, 0.125});
  db.Insert({"F", 3, 0.95});
  db.Insert({"G", 1, 0.75});
  db.Insert({"H", 2, 0.55});
  db.Insert({"K", 3, 0.15});
  std::cout << "============================\n";
  //db.Remove({"B", 2, 0.25});
  //db.Remove({"D", 1, 0.57});
  //db.Remove({"F", 3, 0.95});
  //db.Remove({"H", 2, 0.55});
  //db.SelectGroup(1);
  //db.SelectRating(0.4);
  //db.RawEditionSelectName("T");
  db.PrintAll(std::cout);
  std::cout << "============================\n";
  db.Save("Data.csv");
  DataBase nb;
  nb.Load("database.csv");
  //nb.PrintAll(std::cout);
  nb.Save("test.csv");
  std::ofstream fout;
  fout.open("test.csv");
  nb.SortRating(fout);
  fout.close();
  return 0;
}