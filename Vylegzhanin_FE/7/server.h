#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <istream>
using namespace std;

#include "db.h"

#define DEFAULT_PORT 1234

//сервер
class DbServer {
	Database db_;
	int port_;
	char buf_[1024];
	int as_;

    struct sockaddr_in server_address_;

	char* GetInputMessage(int ms);
	//получает от соединения сообщение
	//(перед которым отправлена его длина).
	//[[[архитектура ещё не совсем продумана]]]

	void SendQueryResult(int ms, QueryResult qr);
	//отправляет по данному соединению
	//набор матриц вместе с необх.данными (размерность, код ошибки)

	void HandleQuery(int ms_input, int ms_output);
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)

public:
	DbServer(int port, const string filename);
	~DbServer();

	void MainLoop();
	//клиент подключается, передаёт матрицу,
	//получает обратно набор матриц, отключается.
};


#endif //SERVER_H