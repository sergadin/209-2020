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
    if(var == "test")
    {
          x.DatabaseFromFile("test_data.txt","test_recipe.txt");
    }
    else
    {
        makedatabase(1000,10,"items.txt","recipe.txt");
        x.DatabaseFromFile("items.txt","recipe.txt");
    }
    cout << "The database has been loaded successfully, and you can start working\n";
    while(1)
    {
      string zapros;
      cin >> zapros;
      std::transform(zapros.begin(), zapros.end(), zapros.begin(), ::tolower);
      if(zapros == "exit")
      {
          cout << "Good bye!\n";
          break;
      }
      if(zapros == "show")
      {
          x.print();
      }
      if(zapros == "canmake")
      {
          string n;
          cin >> n;
          if(x.CanMake(n) == 1)
          {
              cout << "Y\n";
          }
      }
      if(zapros == "make")
      {
        string n;
        cin >> n;
        if(x.CanMake(n) == 1)
        {
            x.MakeDetail(n);
        }
      }
      if(zapros == "adddetail")
      {
            string  n;
            int q;
            cin >> n >> q;
            x.AddDetail(n,q);
      }
    }
    return 0;
}
