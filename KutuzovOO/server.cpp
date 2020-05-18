////////////////////////////////////////////////////////////////
//                                                            //
//      Working with the database from the command line       //
//                                                            //
////////////////////////////////////////////////////////////////


#include <iostream>
#include "database.h"
#include "datagenerator.h"




// g++ server.cpp database.cpp HashTable.cpp LinkedList.cpp -o server
using namespace std;

int main()
{
    /*  makedatabase(1000,10,"items.txt","recipe.txt");
      Database x("items.txt","recipe.txt");
      x.DatabaseToFile("kek.txt","pek.txt");*/
      Database x("test1.txt","test2.txt");
      map<string,int> r;
      r["kartoshka"] = 2;
      r["svekla"] = 3;
      x.AddDetail("zemlya",10);
      x.AddRecipe("Borsh",r);
      x.print();
      cout << endl;
      x.deleteDetail("zemlya");
      x.print();
      cout << endl;
      x.CanMake("Borsh");
      cout << endl;
      x.deleteDetail("palka",5);
      x.AddDetail("svekla",5);
      x.print();
      cout << endl;
      x.CanMake("Borsh");
      cout << endl;
      x.AddDetail("kartoshka",2);
      x.MakeDetail("Borsh");
      x.print();

      cout << x.dbsize() << endl;



      return 0;


}
