#include "server_part.h"
#include "server_part.cpp"

int main() {
	Session s;
	Database db("test.dbase");
	db.HandleQuery("foo", s);
}