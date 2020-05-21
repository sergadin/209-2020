#include "database.h"
#include <sstream>

Database::Database()
{

}

Database::Database(const std::string &filename_items, const std::string &filename_recipes)
{
  std::ifstream fin;
  fin.open(filename_items);
  std::string str;
  while(getline(fin,str))
  {
      std::stringstream ss(str);
      line x;
      std::getline(ss,x.name,',');
      ss  >> x.quant;
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
      ss.ignore(1);
      DeviceName dname = f;
      Recipe rrecipe;
      while(std::getline(ss,f,','))
      {
          int quantt;
          ss >> quantt;
          ss.ignore(1);
          rrecipe[f] = quantt;
          ss.ignore(1);
      }
      known_recipes[dname] = rrecipe;
  }

}
void Database::DatabaseFromFile(const std::string &filename_items, const std::string &filename_recipes)
{
  std::ifstream fin;
  fin.open(filename_items);
  std::string str;
  while(getline(fin,str))
  {
      std::stringstream ss(str);
      line x;
      std::getline(ss,x.name,',');
      ss  >> x.quant;
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
      ss.ignore(1);
      DeviceName dname = f;
      Recipe rrecipe;
      while(std::getline(ss,f,','))
      {
          int quantt;
          ss >> quantt;
          ss.ignore(1);
          rrecipe[f] = quantt;
          ss.ignore(1);
      }
      known_recipes[dname] = rrecipe;
  }
}

void Database::DatabaseToFile(const std::string &filename_items, const std::string &filename_recipes) const
{
      data.HashTableToFile(filename_items);
      std::ofstream ofs(filename_recipes);
      for(auto elm: known_recipes)
      {
        ofs << elm.first << ", ";
        for(auto items: elm.second)
        {
            ofs << items.first << ", " << items.second <<", ";
        }
        ofs << std::endl;
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
void Database::AddDetail(const std::string &name, int quant)
{
    line k;
    k.name = name;
    k.quant = quant;
    data.insert(k);
}

void Database::AddRecipe(const std::string &name, std::map<std::string,int> rec)
{
    known_recipes[name] = rec;
}
int Database::deleteDetail(const DeviceName &name)
{
    line k;
    k.name = name;
    k.quant = 0;
    return data.remove(k);
}
int Database::deleteDetail(const DeviceName &name, int quant)
{
  line k;
  k.name = name;
  k.quant = quant;
  return data.removepart(k);
}
int Database::dbsize() const
{
    return data.size();
}
MakeInfo Database::CanMake(const DeviceName &name, int quant)
{
    MakeInfo answer;
    answer.type = all_right;
    if(known_recipes.find(name) == known_recipes.end())
    {
          answer.type = no_recipe;
          return answer;
    }
    std::map<std::string,int> rec;
    rec = known_recipes[name];
    for(auto elm : rec)
    {
        line det;
        det.name = elm.first;
        det.quant = quant*elm.second;
        int ost =  data.searchP(det);
        if (ost > 0)
        {
            det.quant = ost;
            answer.deficit.push_back(det);
            answer.type =  no_details;
        }
    }
    return answer;

}

void Database::MakeDetail(const DeviceName &name, int quant)
{
  if(CanMake(name,quant).type !=all_right)
  {
        throw 1;
  }
  std::map<std::string,int> rec;
  rec = known_recipes[name];
  for(auto elm : rec)
  {
      line l;
      l.name = elm.first;
      l.quant = quant*elm.second;
      data.removepart(l);
  }
  line k;
  k.name  = name;
  k.quant = quant;
  data.insert(k);
}
