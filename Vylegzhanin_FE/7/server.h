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

#define DEFAULT_PORT 1235
#define QUEUE_LEN 5
#define BUFFER_SIZE 1024

typedef enum {Q_STANDARD, Q_SHUTDOWN, Q_CLEAR} input_code_t;

//сервер
class DbServer {
	Database db_;
	int port_;
    struct sockaddr_in address_;
	socklen_t address_len_;

	char buf_[BUFFER_SIZE];
	int server_fd_;
	int client_fd_;

	void GetInputMessage(int length);
	//получает от соединения сообщение
	//(перед которым отправлена его длина)
	//и сохраняет его в буфер

	void SendQueryResult(QueryResult qr);
	//отправляет ответ на запрос

	void SendData(const void* pdata, size_t len);
	//отправляет что угодно

	void SendInteger(int num);
	//отправляет одно число

	void HandleQuery(int length);
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