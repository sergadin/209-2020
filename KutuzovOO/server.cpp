////////////////////////////////////////////////////////////////
//                                                            //
//      Working with the database from the command line       //
//                                                            //
////////////////////////////////////////////////////////////////

#include <iostream>
#include "database.h"
#include "datagenerator.h"
#include <fstream>
#include <sstream>
#include <algorithm>


// g++ server.cpp database.cpp HashTable.cpp LinkedList.cpp -o server

using namespace std;

int main()
{
    cout << "Welcome to the warehouse\n";
    cout << "Which database would you like to upload?\n";
    cout << "a) random generate\n";
    cout << "b) test\n";
    string var;
    cin >> var;
    Database x;
    if(var == "b")
    {
          x.DatabaseFromFile("test_data.txt","test_recipe.txt");
    }
    else
    {
        makedatabase(1000,10,"items.txt","recipe.txt");
        x.DatabaseFromFile("items.txt","recipe.txt");
    }
    cout << "The database is ready to work\n";
    hellomsg();

    while(1)
    {
      string str;
      getline(cin,str);
      if(str.size() == 0) continue;
      stringstream ss(str);
      string zapros;
      ss >> zapros;
      std::transform(zapros.begin(), zapros.end(), zapros.begin(), ::tolower);
      if(zapros == "exit")
      {
          cout << "Good bye!\n";
          break;
      }
      else if(zapros == "show")
      {
          x.print();
      }
      else if(zapros == "canmake")
      {
          line det;
          ss.ignore(1);
          getline(ss,det.name,',');
          ss  >> det.quant;
          MakeInfo inf;
          inf =  x.CanMake(det.name,det.quant);
          switch (inf.type)
          {
            case 0:
            {
                cout << "You can make this!\n";
                break;
            }
            case 1:
            {
                  cout << "I don't have a recipe. You can add it using the command \"AddRecipe DETAILNAME, FIRTST_ITEM, QUANT, SECOND_ITEM, QUANT ,... \" \n";
                  break;
            }
            case 2:
            {
                cout << "Not enough details: ";
                for(int i=0;i<inf.deficit.size()-1;i++)
                {
                      cout <<  inf.deficit[i].name << ", " << inf.deficit[i].quant <<  ", ";
                }
                cout <<  inf.deficit[inf.deficit.size()-1].name << ", " << inf.deficit[inf.deficit.size()-1].quant <<  "\n";
                break;
            }
          }
      }
      else if(zapros == "make")
      {
        line det;
        ss.ignore(1);
        getline(ss,det.name,',');
        ss  >> det.quant;
        x.MakeDetail(det.name,det.quant);
      }
      else if(zapros == "adddetail")
      {
          line det;
          ss.ignore(1);
          getline(ss,det.name,',');
          ss  >> det.quant;
          x.AddDetail(det.name,det.quant);
      }
      else if(zapros == "savetofile")
      {
          string name1, name2;
          ss >> name1 >> name2;
          x.DatabaseToFile(name1,name2);
      }
      else
      {
          cout << "I didn't understand your request\n";
      }
      hellomsg();
    }
    return 0;
}
