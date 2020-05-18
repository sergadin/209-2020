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



using namespace std;



void makedatabase(int size_, int levels,const string &namefileI, const string &namefile)
{
    string s = "abridgment";
    vector<string> items;
    ifstream fin;
    fin.open("minecraft_items.txt");
    string line;
    while(getline(fin, line))
    {
        items.push_back(line);
    }
    ofstream ofs(namefile);
    for(int i=0;i<levels;i++)
    {
        vector<string> tmp;
        for(int j=0;j<(size_/levels);j++)
        {
            next_permutation(s.begin(), s.end());
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
            ofs << items[rand() % items.size()]  << ", " <<  rand() % 100  << endl;

        }
        for(auto elm: tmp)
        {
            items.push_back(elm);
        }
    }
    fin.close();
    fin.open("minecraft_items.txt");
    ofstream offs(namefileI);
    while(getline(fin, line))
    {
        offs << line << ", " << rand() % 100 << endl;
    }

}
