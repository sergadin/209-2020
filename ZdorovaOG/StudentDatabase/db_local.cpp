#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "database/database.h"

int main() {
  DataBase db;
  db.Load("__database.csv");
  while (std::cin) {
    try {
      db.Process(std::cin, std::cout);
    } catch (std::exception &ex) {
      std::cerr << ex.what() << std::endl;
    }
  }
  return 0;
}