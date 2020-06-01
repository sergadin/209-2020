#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "database/database.h"

int main() {
  DataBase db;
  auto pid = db.RegisterUser();
  db.Load("__database.csv");
  while (std::cin) {
    try {
      db.Process(std::cin, std::cout, pid);
    } catch (std::exception &ex) {
      std::cerr << ex.what() << std::endl;
    }
  }
  return 0;
}