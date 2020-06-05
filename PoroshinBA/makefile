CC = g++
SERVER_PART = server/client_socket.cpp server/server.cpp
DATABASE_PART = Work.cpp
FLAGS = -g --std=c++1z

all: local server

local: Main.cpp
	$(CC) $(FLAGS) $(SERVER_PART) $(DATABASE_PART) Main.cpp -o out_local

server: Server.cpp
	$(CC) $(FLAGS) $(SERVER_PART) $(DATABASE_PART) Server.cpp -o out_server -lpthread

clean:
	rm -f out_local out_server