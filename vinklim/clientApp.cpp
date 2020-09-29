#include <string>
#include <iostream>
#include "Client.h"



void printRS(const R2::RecordSet& rs)
{
	using namespace std;
	for (size_t i = 0; i < rs.length(); ++i)
	{
		const auto& row = rs.row(i);
		for (size_t j = 0; j < row.size(); ++j)
		{
			if (j > 0)
				cout << "\t";
			cout << row[j];
		}
		cout << endl;
	}
}

int main()
{
	using namespace std;
	using namespace R2;
	//
	try
	{
		Client client("127.0.0.1", 8888);
		//
		//
		RecordSet rs;
		while (true)
		{
			string q;
			cout << "query: ";
			getline(cin, q);
			if (q == "exit")
				break;
			else if (q.substr(0,5) == "print")
			{
				printRS(rs);
				continue;
			}
			// q = "insert lecture 1 0 4 1 0 0 0";
			rs = client.query(q);		
			//break;
		}
	}
	catch (Exception& e1)
	{
		cout << e1.text() << endl;
	}
	catch(...)
	{
		cout << "Unexpected error!" << endl;
	}
	//
	return 0;
}