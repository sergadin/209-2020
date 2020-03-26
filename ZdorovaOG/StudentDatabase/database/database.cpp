#include "database.h"
#include "../parser/condition_parser.h"
#include "../tests/profile.h"
#include "util.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

void DataHolder::Insert(const Student &s) {
  size_t id = _data.size();
  _data.push_back(s);
  _index.Insert(id, s);
}

bool DataHolder::Remove(size_t id) {
  if (!_index.HasId(id))
    return false;
  return _index.Remove(id, _data[id]);
}

bool DataHolder::Load(const std::string &file_name) {
#ifdef DEBUG
  std::stringstream debug_ss;
  debug_ss << "Loading file(" << file_name << ')';
  LOG_DURATION(debug_ss.str());
#endif

  std::ifstream ifs(file_name, std::ios::binary);
  if (!ifs.is_open())
    return false;

  return Load(ifs);
}

bool DataHolder::Load(std::istream &is) {
#ifdef DEBUG
  LOG_DURATION("Parsing input");
#endif
  std::string buffer;
  is.seekg(0, std::ios::end);
  buffer.resize(is.tellg());
  is.seekg(0);
  is.read(&buffer[0], buffer.size());

  _data.reserve(_data.size() + std::count(buffer.begin(), buffer.end(), '\n'));
  std::stringstream ss(std::move(buffer));

  using namespace util;
  Student s;
  while (std::getline(ss, s.name, ',')) {
    ss >> ios::skipws >> s.group >> ios::skipws >> ios::skipcm
       >> s.rating >> ios::skipws >> ios::skipcm;
    std::getline(ss, s.info);
    Insert(s);
  }
  return true;
}

bool DataHolder::Save(const std::string &file_name) {
#ifdef DEBUG
  std::stringstream debug_ss;
  debug_ss << "Saving file(" << file_name << ')';
  LOG_DURATION(debug_ss.str());
#endif

  std::ofstream ofs(file_name, std::ios::binary);
  if (!ofs.is_open())
    return false;

  Save(ofs);
  return true;
}

void DataHolder::Save(std::ostream &os) {
#ifdef DEBUG
  LOG_DURATION("Dump data");
#endif
  char delim = ',';
  std::stringstream ss;
  for (const auto &id : _index.GetIds()) {
    const Student &s = _data[id];
    s.PrintAll(ss, delim);
    ss << '\n';
  }

  const auto &buffer = ss.str();
  os.write(&buffer[0], buffer.size());
}

void DataBase::Select(std::istream &is) {
#ifdef DEBUG
  LOG_DURATION("Process selection");
#endif
  auto process_tree = ParseSelectCondition(is);
  ConstructIndex(process_tree->Process(base::_index));
}
void DataBase::Reselect(std::istream &is) {
#ifdef DEBUG
  LOG_DURATION("Process selection");
#endif
  auto process_tree = ParseSelectCondition(is);
  ConstructIndex(process_tree->Process(_index));
}

void DataBase::Print(std::istream &is, std::ostream &os) {
  auto process_items = ParsePrintCondition(is);
  auto sort_item = process_items.back();
  process_items.pop_back();
  
  if (sort_item == "id") {
    Print(os, process_items, _index.GetIds());
  } else if (sort_item == "name") {
    Print(os, process_items, _index.GetNameMap());
  } else if (sort_item == "group") {
    Print(os, process_items, _index.GetGroupMap());
  } else if (sort_item == "rating") {
    Print(os, process_items, _index.GetRatingMap());
  } else {
    throw std::logic_error("Uknown sort_by field");
  }
}

void DataBase::Delete() {
  for (auto id : _index.GetIds())
    Remove(id);
  _index.Clear();
}

void DataBase::Add(std::istream &is) {
  Student s;
  using namespace util;
  std::getline(is, s.name, ',');
  is >> s.group >> ios::skipws >> ios::skipcm >> s.rating;
  Insert(s);
}

void DataBase::Process(std::istream &is, std::ostream &os) {
  std::string query;
  is >> query;
  for (auto &c : query)
    c = std::tolower(c);
  if (query == "select") {
    Select(is);
  } else if (query == "reselect") {
    Reselect(is);
  } else if (query == "print") {
    Print(is, os);
  } else if (query == "delete") {
    Delete();
  } else if (query == "add") {
    Add(is);
  } else if (query == "dump") {
    std::string file_name;
    std::getline(is, file_name);
    Save(file_name);
  } else if (query == "save") {
    Save("__database.txt");
  } else if (query == "exit") {
    exit(0);
  } else if (query == "help") {
    os << "This is the student database.  Usage:\n"
       << "  select [conditions] end\n"
       << "  reselect [conditions] end\n"
       << "  print <columns> [sortby <column>] end\n"
       << "  add <student-name>, <student-group>, <student-rating>\n"
       << "  delete\n"
       << "  save\n"
       << "  dump <file-name>\n"
       << "  exit\n\n";
    os << "Request types:\n"
       << "  select      Make selection on the full data with given conditions\n"
       << "  reselect    Make selection on previous sample with given conditions\n"
       << "  print       Print last sample columns in given order\n"
       << "  add         Add new student to database w/o saving to file\n"
       << "  delete      Remove last selection from the full data w/o saving to file\n"
       << "  save        Save changes to default database file '__databse.txt'\n"
       << "  dump        Save changes to given file\n"
       << "  exit        Close program\n\n";
    os << "Columns of database:\n"
       << "  name        string:  Student full name\n"
       << "  group       integer: Student group ID\n"
       << "  rating      real:    Student rating\n"
       << "  info        string:  Student additional info (ONLY for print)\n\n";
    os << "Conditions are of two types: simple and complex\n"
       << "Simple condition:\n"
       << "  <column> <compare-operation={==, !=, >=, >, <=, <}> <value>\n"
       << "Complex condition:\n"
       << "  <simple-condition> <logical-operation={or, and}> <simple-condition>\n"
       << "  (<complex-condition>) <logical-operation={or, and}> <simple-condition>\n\n";
    os << "Examples:\n"
       << "  add Ivanov I.I., 209, 4.7\n"
       << "  select (name == \"Iv*\" or name == \"Pe*\") and rating > 3.5 end\n"
       << "  reselect group < 210 end\n"
       << "  print name group rating end\n"
       << "  dump changed.txt\n";
  } else {
    os << "Unknown request. Use help for info\n";
  }
}

void DataBase::ConstructIndex(const std::set<size_t> &ids) {
  _index.Clear();
  for (auto id : ids)
    _index.Insert(id, _data[id]);
}

void DataBase::Print(std::ostream &os, Columns what_col, const IdSet &ids) {
  char delim = ' ';
  for (auto id : ids) {
    if(what_col.empty())
      _data.at(id).PrintAll(os, delim);
    else
      for (const auto &col : what_col) {
        _data.at(id).Print(os, col);
        os << delim;
      }
    os << '\n';
  }
}

void DataBase::Print(std::ostream &os, Columns what_col, const NameMap &names) {
  for (const auto &elem : names)
    Print(os, what_col, elem.second);
}
void DataBase::Print(std::ostream &os, Columns what_col,
                     const GroupMap &groups) {
  for (const auto &elem : groups)
    Print(os, what_col, elem.second);
}
void DataBase::Print(std::ostream &os, Columns what_col,
                     const RatingMap &ratings) {
  for (const auto &elem : ratings)
    Print(os, what_col, elem.second);
}