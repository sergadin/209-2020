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

#include <algorithm>
#include <vector>
#include <sys/select.h>

#include <string>
#include <iostream>
#include <istream>
using namespace std;

#include "db.h"

#define DEFAULT_PORT 1235
#define QUEUE_LEN 5
#define BUFFER_SIZE 1024
#define MAXIMAL_TIMEOUT {3,0}
typedef enum {Q_STANDARD, Q_SHUTDOWN, Q_CLEAR} input_code_t;

struct Client {
	int fd_;
public:
	Client(int fd):
		fd_(fd)
		{}
	int GetFd() {return fd_;}
};

//сервер
class DbServer {
	Database db_;
	int port_;


	char buf_[BUFFER_SIZE];
	int server_fd_;

	fd_set rfds_;
	vector<Client> clients_;

	bool time_to_stop_;

    struct sockaddr_in address_;
	socklen_t address_len_;

	vector<Client>::iterator current_client_fd_;
	//клиент, с которым сервер работает в настоящий момент
	//(именно с ним взаимодействуют все методы ниже)

	void GetInputMessage(int length);
	//получает от соединения сообщение заданной длины
	//и сохраняет его в буфер

	void SendQueryResult(QueryResult qr);
	//отправляет ответ на запрос

	void SendData(const void* pdata, size_t len);
	void SendInteger(int num);
	//обёрки для write

	int GetData(void* pdata, size_t len);
	int GetInteger(int* pnum);
	//обёртки для recv

	void HandleQuery(int length);
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)

	void InteractWithCurrentClient();

public:
	DbServer(int port, const string filename);
	~DbServer();

	void MainLoop();
	//взаимодействие с клиентами
};


#endif //SERVER_H