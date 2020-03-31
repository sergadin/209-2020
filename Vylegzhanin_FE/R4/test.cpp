#include "server_part.h"
#include "server_part.cpp"

int main() {
	Session s;
	ImplementSelect(parse("foo").conditions, s);
}