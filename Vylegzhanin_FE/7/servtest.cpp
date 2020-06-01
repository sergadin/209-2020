#include <iostream>
using namespace std;

#include "db.h"
#include "matrix.h"
#include "server.h"

int main() {
	DbServer serv(1234, "temp.dbase");
	
	serv.MainLoop();
}