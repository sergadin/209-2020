#include "server_part.h"
#include "server_part.cpp"

int main() {
	Session s;
	Database db("test.dbase");
	string query("foo");
	db.HandleQuery(query, s);
}