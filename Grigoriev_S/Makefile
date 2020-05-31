
all: mm.exe

sadist: mms.exe

mm.exe: marketMaker_main.o market.o sqlite3.o
	c++ -g -o mm.exe marketMaker_main.o market.o sqlite3.o -lpthread -ldl

marketMaker_main.o: marketMaker_main.cpp
	c++ -g -c marketMaker_main.cpp

market.o: market.cpp market.h utility.h RBTree.h
	c++ -g -c market.cpp


mms.exe: marketMaker_mains.o markets.o sqlite3.o
	c++ -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
    -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
    -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor \
    -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual \
    -Wsign-promo -Weffc++ -std=gnu++0x -g -o mm.exe marketMaker_main.o market.o sqlite3.o -lpthread -ldl

marketMaker_mains.o: marketMaker_main.cpp
	c++ -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
    -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
    -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor \
    -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual \
    -Wsign-promo -Weffc++ -std=gnu++0x -g -c marketMaker_main.cpp
	
markets.o: market.cpp market.h utility.h
	c++ -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default \
    -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
    -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor \
    -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual \
    -Wsign-promo -Weffc++ -std=gnu++0x -g -c market.cpp

sqlite3.o: sqlite3.c
	gcc -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-defaulti \
    -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op \
    -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
    -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
    -Wno-pointer-sign -std=gnu99 -g -c sqlite3.c
