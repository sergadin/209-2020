#include "database/database.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
  DataBase db;
  db.Load("__database.txt");
  while (std::cin) {
    db.Process(std::cin, std::cout);
  }
  return 0;
}