#include "Server.h"

int main()
{
	using namespace R2;
	//
	try 
	{
		Server server;
		server.ConnectDatabase("database.bin");
		server.Start();
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