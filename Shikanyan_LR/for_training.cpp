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

int main(void)
{
	char ch[232], ch2[10] = "123456780";
	string st;
	string s("dwd qwdqdq");
	string s2("2222dwd qwdqdq");
	st = string(ch2);
	s.append(" 11 11");
	strcpy(ch,s.c_str());
	cout << st << endl << st.size() << endl;
	cout << ch << strlen(ch) << endl; // 6, ?????? ????? ??????
	strcpy(ch,s2.c_str());
	cout << ch << strlen(ch);

	return 0;
}
