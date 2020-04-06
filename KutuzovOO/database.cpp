#include "database.h"
#include <sstream>

Database::Database(const std::string &filename_items, const std::string &filename_recipes)
{
  std::ifstream fin;
  fin.open(filename_items);
  std::string str;
  while(getline(fin,str))
  {
      line x;
      x.name = str;
      x.quant = 1;
      data.insert(x);
  }
  //data.print();
  std::ifstream fin1;
  fin1.open(filename_recipes);
  while(getline(fin1,str))
  {
      std::stringstream ss(str);
      std::string f;
      std::getline(ss,f,',');
      DeviceName dname = f;
      Recipe rrecipe;
      while(std::getline(ss,f,','))
      {
          int quantt;
          ss >> quantt;
          ss.ignore(1);
          rrecipe[f] = quantt;
      }
      known_recipes[dname] = rrecipe;
  }

}

void Database::print() const
{
    std::cout << "Items in storage:\n";
    data.print();
    std::cout << "Known recipes:\n";
    for(auto elm: known_recipes)
    {
      std::cout << elm.first << " - ";
      for(auto items: elm.second)
      {
          std::cout << items.first << " " << items.second <<"; ";
      }
      std::cout << std::endl;
    }

}
