#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <pair>
#include <list>



class Detail
{
private:
    std::string name;
    std::vector<std::pair<Detail,int>>  Assembly_map;
public:
  Detail();
  Detail(const std::string &namedet); // Created without an Assembly map
  Detail(const std::string &namedet, const std::vector<std::pair<Detail,int>> &as_map);
  bool operator==(const Detail &rhs);
  Detail& operator= (const   Detail& other);
  ~Detail();
}


class Datebase
{
private:
  std::list<std::pair<<Detail,int>> Data;
  // For a quick search there will be a hash set
public:
  Datebase();
  Datebase(const std::string &filename);
  Database& operator= (const Database& other);
  Database(const Database& other, SearchConditions criteria);
  int AddDetail(const Detail &det);
  bool CanMake(const Detail &det) const;
  void MakeDetail(const Detal &det);
  int deleteDetail(const Detail &det);
  int dbsize() const;
  void SaveToFile(const std::string &filename);
  Detail &GetDetail(const string &name);
  void ChangeQuantity(const std::string &name, int newQuant);
  void DatebaseToFile(const std::string &namefile);


}
