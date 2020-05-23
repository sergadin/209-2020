#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <map>
#include <algorithm>    

using namespace std;

string more(const string &com1,const string &com2)
{
	if(com1.size()>com2.size())
	{
		return com1;
	}
	if(com1.size()<com2.size())
	{
		return com2;
	}
	if(com1>=com2)
	{
		return com1;
	}
	else
	{
		return com2;
	}
}

int main(void)
{
	int q,i;
	for(q=0;q<2;q++)
	{
		cout << q << endl;
		for(i=0;i<3;i++)
		{
			cout << "i-" << i << endl;
			if(i==2)
			{
				break;
			}
		}
	}
	/*map <int,int> m;
	map <int,int> :: iterator it;
	m[1]=1;
	m[2]=2;
	m[3]=3;
	for( it = m.begin(); it != m.end(); ++it) 
	{
		cout << it->first << " : " << it->second << endl;
 	}*/

return 0;
}
