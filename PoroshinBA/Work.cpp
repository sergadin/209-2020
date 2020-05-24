#include "Headbase.h"

void DataBase::RawEditionSelectName(std::string str) {
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

void DataBase::SelectGroup(int g) {
  for (auto it = group_data.begin(); it != group_data.end();) {
    if (it->first != g)
      it = group_data.erase(it);
    else
      it++;
  }
}

void DataBase::SelectRating(double r) {
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

/*void DataBase::Process(std::istream &is, std::ostream &os) {  //Work in progress...
  std::string query;
  is >> query;
  for (auto &c : query) c = std::tolower(c);
  if (query == "select") {
    is >> query;
    for (auto &d : query) d = std::tolower(d);

    if (query == "name") {
      
    } else if (query == "groups") {

    } else if (query == "rating")

  }
}*/

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