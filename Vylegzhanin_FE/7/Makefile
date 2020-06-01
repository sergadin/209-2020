#флаги из комп. класса, но без -weffc++, потому что он требует
#initialization list для конструкторов классов (неудобно)
F=-Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
	-Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
	-Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor \
	-Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual \
	-Wsign-promo -std=gnu++0x

matrix.cpp: matrix.h exceptions.h
db.cpp: matrix.cpp db.h
matrixtest.cpp: db.cpp 
server.cpp: db.cpp server.h
servtest.cpp: server.cpp

%.o: %.cpp
	c++ -c $F $< -o $@

test_matrix: matrix.o db.o matrixtest.o
	c++ $F matrix.o db.o matrixtest.o -o testmatrix
	./testmatrix

test_server: matrix.o db.o server.o servtest.o
	c++ $F matrix.o db.o server.o servtest.o -o testserver
	./testserver
