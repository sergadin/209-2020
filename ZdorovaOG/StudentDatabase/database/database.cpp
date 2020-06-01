#include "database.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "../parser/condition_parser.h"
#include "../parser/token.h"
#include "../tests/profile.h"
#include "util.h"

void DataHolder::Insert(const Student &s) {
  size_t id = _data.size();
  _data.push_back(s);
  _index.Insert(id, s);
}

bool DataHolder::Remove(size_t id) {
  if (!_index.HasId(id)) return false;
  return _index.Remove(id, _data[id]);
}

bool DataHolder::Load(const std::string &file_name) {
#ifdef DEBUG
  std::stringstream debug_ss;
  debug_ss << "Loading file(" << file_name << ')';
  LOG_DURATION(debug_ss.str());
#endif

  std::ifstream ifs(file_name, std::ios::binary);
  if (!ifs.is_open()) return false;

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
    ss >> ios::skipws >> s.group >> ios::skipws >> ios::skipcm >> s.rating >>
        ios::skipws >> ios::skipcm;
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
  if (!ofs.is_open()) return false;

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
    std::vector<std::string> columns = {"name", "group", "rating", "info"};
    for (auto &col : columns) {
      s.Print(ss, col);
      ss << (col == columns.back() ? '\n' : delim);
    }
  }

  const auto &buffer = ss.str();
  os.write(&buffer[0], buffer.size());
}

void DataBase::Select(std::istream &is, size_t pid) {
  std::lock_guard<std::mutex> lock(_mutex);
#ifdef DEBUG
  LOG_DURATION("Process selection");
#endif
  auto process_tree = ParseExpression(is);
  _indexes[pid] = ConstructIndex(process_tree->Process(base::_index));
}

void DataBase::Reselect(std::istream &is, size_t pid) {
#ifdef DEBUG
  LOG_DURATION("Process selection");
#endif
  auto process_tree = ParseExpression(is);
  _indexes[pid] = ConstructIndex(process_tree->Process(_indexes[pid]));
}

void DataBase::Print(std::istream &is, std::ostream &os, size_t pid) {
  auto [fields, sortby] = ParseFieldList(is);
  if (fields.empty()) fields = {"id", "name", "group", "rating", "info"};
  for (const auto &col : fields) {
    os << col << (col == fields.back() ? '\n' : _delim);
  }
  if (sortby == "id") {
    Print(os, fields, _indexes[pid].GetIds());
  } else if (sortby == "name") {
    Print(os, fields, _indexes[pid].GetNameMap());
  } else if (sortby == "group") {
    Print(os, fields, _indexes[pid].GetGroupMap());
  } else if (sortby == "rating") {
    Print(os, fields, _indexes[pid].GetRatingMap());
  } else {
    throw std::logic_error("Unknown sort_by field");
  }
}

void DataBase::Delete(size_t pid) {
  std::lock_guard<std::mutex> lock(_mutex);
  for (auto id : _indexes[pid].GetIds()) Remove(id);
  _indexes[pid].Clear();
}

void DataBase::Delete(size_t pid, const std::vector<size_t> &ids){
  for (auto id : ids) {
    Remove(id);
    _indexes[pid].Remove(id, _data[id]);
  }
}

void DataBase::Add(std::istream &is) {
  std::lock_guard<std::mutex> lock(_mutex);
  Student s;
  s.name = ParseString(is);
  s.group = ParseInteger(is);
  s.rating = ParseFloat(is);
  s.info = ParseString(is);
  Insert(s);
}

DataBase::Status DataBase::Process(std::istream &is, std::ostream &os,
                                   size_t pid) {
  std::string query;
  is >> query;
  for (auto &c : query) c = std::tolower(c);
  if (query == "select") {
    Select(is, pid);
    os << "Done";
  } else if (query == "reselect") {
    Reselect(is, pid);
    os << "Done";
  } else if (query == "print") {
    Print(is, os, pid);
  } else if (query == "delete") {
    Delete(pid);
    os << "Done";
  } else if (query == "remove") {
    auto tokens = Tokenize(is);
    std::vector<size_t> ids;
    for(auto& token : tokens)
      if(token.type == TokenType::INTEGER)
        ids.push_back(std::stoi(token.value));
      else
        throw std::logic_error("Expected only integer numbers");
    Delete(pid, ids);
    os << "Done";
  } else if (query == "add") {
    Add(is);
    os << "Done";
  } else if (query == "dump") {
    std::string file_name = ParseString(is);
    Save(file_name);
    os << "Done";
  } else if (query == "save") {
    Save(DB_INPUT);
    os << "Done";
  } else if (query == "exit") {
    os << "disconnected";
    return Status::Shutdown;
  } else if (query == "disconnect") {
    os << "disconnected";
    return Status::Close;
  } else if (query == "help") {
    os << "Help:\n";
    os << "This is the student database.  Usage:\n"
       << "  select [conditions] end\n"
       << "  reselect [conditions] end\n"
       << "  print <columns> [sortby <column>] end\n"
       << "  add <student-name> <student-group> <student-rating>"
          "<student-info> end\n"
       << "  delete\n"
       << "  save\n"
       << "  remove <list-of-ids> end\n"
       << "  dump <file-name> end\n"
       << "  exit\n\n";
    os << "Request types:\n"
       << "  select      Make selection on the full data with given "
          "conditions\n"
       << "  reselect    Make selection on previous sample with given "
          "conditions\n"
       << "  print       Print last sample columns in given order\n"
       << "  add         Add new student to database w/o saving to file\n"
       << "  delete      Remove last selection from the full data w/o saving "
          "to file\n"
       << "  remove      Remove given ids from the full data w/o saving "
          "to file\n"
       << "  save        Save changes to default database file "
          "'__databse.txt'\n"
       << "  dump        Save changes to given file\n"
       << "  disconnect  Disconnect from database\n"
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
       << "  <simple-condition> <logical-operation={or, and}> "
          "<simple-condition>\n"
       << "  (<complex-condition>) <logical-operation={or, and}> "
          "<simple-condition>\n\n";
    os << "Examples:\n"
       << "  add \"Ivanov I.I.\" 209 4.7 \"Good boy\" end\n"
       << "  select (name == \"Iv*\" or name == \"Pe*\") and rating > 3.5 end\n"
       << "  reselect group < 210 end\n"
       << "  print id name group rating end\n"
       << "  remove 42 666 2048 end\n"
       << "  dump \"changed.csv\" end\n";
  } else {
    throw std::logic_error("Unknown request. Use help for info");
  }
  return Status::Ok;
}

Index DataBase::ConstructIndex(const std::set<size_t> &ids) {
  Index index;
  for (auto id : ids) index.Insert(id, _data[id]);
  return index;
}

void DataBase::Print(std::ostream &os, Columns what_col, const IdSet &ids) {
  for (auto id : ids) {
    for (const auto &col : what_col) {
      if(col=="id")
        os << id;
      else
        _data.at(id).Print(os, col);
      os << (col == what_col.back() ? '\n' : _delim);
    }
  }
}

void DataBase::Print(std::ostream &os, Columns what_col, const NameMap &names) {
  for (const auto &elem : names) Print(os, what_col, elem.second);
}
void DataBase::Print(std::ostream &os, Columns what_col,
                     const GroupMap &groups) {
  for (const auto &elem : groups) Print(os, what_col, elem.second);
}
void DataBase::Print(std::ostream &os, Columns what_col,
                     const RatingMap &ratings) {
  for (const auto &elem : ratings) Print(os, what_col, elem.second);
}

void DataBase::SetDelim(char delim) { _delim = delim; }

int DataBase::RegisterUser() {
  std::lock_guard<std::mutex> lock(_mutex);
  int id = _indexes.size();
  _indexes[id] = base::_index;
  return id;
}

bool DataBase::EraseUser(size_t pid) { 
  std::lock_guard<std::mutex> lock(_mutex);
  return _indexes.erase(pid); 
}