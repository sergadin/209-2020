#include "database.h"

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
  data.print();
  Recipe k;
  k["asd"] = 5; //In the development
  k["med"] = 7; //In the development
  known_recipes["Lol"] = k; // In the development

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
