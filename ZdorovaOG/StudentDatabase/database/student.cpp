#include "student.h"

#include <iostream>
#include <stdexcept>

void Student::Print(std::ostream &os, const std::string &field) const {
  if (field == "name") {
    os << name;
  } else if (field == "group") {
    os << group;
  } else if (field == "rating") {
    os << rating;
  } else if (field == "info") {
  	os << info;
  } else {
    throw std::logic_error("Unknown field " + field);
  }
}

void Student::PrintAll(std::ostream &os, char delim) const {
  for(const auto& field : {"name", "group", "rating"}) {
  	Print(os, field);
  	os << delim;
  }
  Print(os, "info");
}