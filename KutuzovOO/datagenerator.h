//////////////////////////////////////////////////////////
//                                                      //
//      The function generates a random database        //
//                                                      //
//////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <ctime>
#include <algorithm>
#include <set>



void hellomsg()
{
  std::cout <<"You can make several types of requests:\n";
  std::cout << "\"show\" - Prints the database to the screen\n";
  std::cout << "\"canmake DETAILNAME, QUANT\" - Displays information about whether the device can be built\n";
  std::cout << "\"make DETAILNAME, QUANT\" - Assemble the detail\n";
  std::cout << "\"adddetail DETAILNAME, QUANT\" - Add a detail to the database \n";
  std::cout << "\"exit\" - terminate program \n";
  std::cout << "\"savetofile FILENAME1 FILENAME2\" -  save the database to files\n";
  std::cout << "Input comand : \n";
}



void makedatabase(int size_, int levels,const std::string &namefileI, const std::string &namefile)
{
    std::string s = "abridgment";
    std::vector<std::string> items;
    std::ifstream fin;
    fin.open("minecraft_items.txt");
    std::string line;
    while(getline(fin, line))
    {
        items.push_back(line);
    }
    std::ofstream ofs(namefile);
    for(int i=0;i<levels;i++)
    {
        std::vector<std::string> tmp;
        for(int j=0;j<(size_/levels);j++)
        {
            std::next_permutation(s.begin(), s.end());
            tmp.push_back(s);
        }
        srand(i*i +12);
        for(auto elm: tmp)
        {
            ofs << elm << ", ";
            int rec = rand() % 10;
            rec++;
            for(int k=0;k<rec-1;k++)
            {
                ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  << ", ";
            }
            ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  << std::endl;

        }
        for(auto elm: tmp)
        {
            items.push_back(elm);
        }
    }
    fin.close();
    fin.open("minecraft_items.txt");
    std::ofstream offs(namefileI);
    while(getline(fin, line))
    {
        offs << line << ", " << rand() % 100 << std::endl;
    }
    std::string sp =  "abridgment";
    for(int i=0;i<size_;i++)
    {
        offs << sp << ", " << rand() % 100 << std::endl;
        next_permutation(sp.begin(), sp.end());
    }

}
