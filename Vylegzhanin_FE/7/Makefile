#флаги из комп. класса, но без -weffc++, потому что он требует
#initialization list для конструкторов классов (неудобно)
F=-Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
	-Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
	-Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor \
	-Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual \
	-Wsign-promo -std=gnu++0x

%.o: %.cpp
	c++ -c $F $< -o $@

matrix.cpp: matrix.h exceptions.h
db.cpp: exceptions.h db.h
matrixtest.cpp: db.h matrix.h 
server.cpp: server.h
servtest.cpp: db.h matrix.h server.h

test_matrix: matrix.o db.o matrixtest.o
	c++ $F matrix.o db.o matrixtest.o -o test_matrix

test_server: matrix.o db.o server.o servtest.o
	c++ $F matrix.o db.o server.o servtest.o -o test_server

