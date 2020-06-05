OBJS_PARSER	= _obj/node.o _obj/token.o _obj/condition_parser.o
OBJS_DATABASE = _obj/database.o _obj/index.o _obj/student.o
OBJS_SERVER = _obj/server.o _obj/client_socket.o
OBJS = $(OBJS_DATABASE) $(OBJS_PARSER) $(OBJS_SERVER) _obj
OUT_TESTS = tests/_test_condition_parsing tests/_test_tokenize
OUT = _db_local _input_builder $(OUT_TESTS)
CC	 = g++
FLAGS	 = -g -O2 --std=c++1z -fstack-protector-all -Wall -Wextra -Wunused -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

all: db_local db_server tests run_tests

db_local: parser database
	$(CC) $(FLAGS) $(OBJS_PARSER) $(OBJS_DATABASE) db_local.cpp -o _db_local

db_server: parser database server_part
	$(CC) $(FLAGS) $(OBJS_PARSER) $(OBJS_DATABASE) $(OBJS_SERVER) db_server.cpp -o _db_server -lpthread

tests: parser $(OUT_TESTS)

tests/_test_condition_parsing: tests/test_condition_parsing.cpp
	$(CC) $(FLAGS) tests/test_condition_parsing.cpp $(OBJS_PARSER) _obj/index.o -o tests/_test_condition_parsing

tests/_test_tokenize: tests/test_tokenize.cpp
	$(CC) $(FLAGS) tests/test_tokenize.cpp $(OBJS_PARSER) _obj/index.o -o tests/_test_tokenize

run_tests: tests
	@echo "========== RUN TESTS =========="
	@for test in $(OUT_TESTS); do $$test; done
	@echo "============= END ============="

parser: $(OBJS_PARSER)

database: $(OBJS_DATABASE)

server_part: $(OBJS_SERVER)

input_builder:
	$(CC) $(FLAGS) data_generator/file_generator.cpp -o _input_builder

_obj:
	@mkdir -p _obj

_obj/condition_parser.o: _obj/token.o parser/condition_parser.cpp
	$(CC) -c $(FLAGS) parser/condition_parser.cpp -o _obj/condition_parser.o

_obj/token.o: _obj/node.o parser/token.cpp
	$(CC) -c $(FLAGS) parser/token.cpp -o _obj/token.o

_obj/node.o: _obj/index.o parser/node.cpp | _obj
	$(CC) -c $(FLAGS) parser/node.cpp -o _obj/node.o 

_obj/database.o: _obj/index.o database/database.cpp
	$(CC) -c $(FLAGS) database/database.cpp -o _obj/database.o

_obj/index.o: _obj/student.o database/index.cpp
	$(CC) -c $(FLAGS) database/index.cpp -o _obj/index.o

_obj/student.o: database/student.cpp | _obj
	$(CC) -c $(FLAGS) database/student.cpp -o _obj/student.o

_obj/server.o: server/server.cpp | _obj
	$(CC) -c $(FLAGS) server/server.cpp -o _obj/server.o

_obj/client_socket.o: server/client_socket.cpp | _obj
	$(CC) -c $(FLAGS) server/client_socket.cpp -o _obj/client_socket.o

clean:
	rm -rf $(OBJS) $(OUT)
