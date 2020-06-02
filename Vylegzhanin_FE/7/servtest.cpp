#include <iostream>
using namespace std;

//#include "db.h"
//#include "matrix.h"
#include "server.h"

int main() {
	try {
	DbServer serv(1237, "temp.dbase");
	serv.MainLoop();
	}
	catch(ServerException se) {
		cout << "ServerException; msg='" << se.Message() << "'" << endl;
		perror("details");
	}

}