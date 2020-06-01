#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <cstdio>

using namespace std;

#include "server.h"

void DbServer::GetInputMessage() {
	//получает от соединения сообщение
	//(перед которым отправлена его длина)
	//и сохраняет его в буфер
	
	int future_len = 0;
	
	recv(client_fd_, &future_len, sizeof(int), MSG_WAITALL);

	if(future_len < 0) {
		cout << "negative future_len" << endl;
		throw QueryException("message len should be positive");
	}

	if(future_len == BUFFER_SIZE) {
		cout << "message is too long" << endl;
		throw QueryException("message is too long");
	}
	
	bzero(buf_, BUFFER_SIZE);

	int read_bytes = recv(client_fd_, buf_, future_len, MSG_WAITALL);

	if(read_bytes < future_len) {
		cout << "read " << read_bytes << " bytes instead of claimed " << future_len << endl;
		throw QueryException("message must be not full");
	}
	else {
		cout << "successfully read " << read_bytes << "=" <<  future_len << " bytes" << endl << flush;
	}
}



void DbServer::SendQueryResult(QueryResult qr) {
	//отправляет ответ на запрос по текущему адресу
	cout << "enter SendQueryResult" << endl << flush;

	int future_len = 0;
	if(qr.err_code != ERRC_OK) {
		//если ошибка -- отправляем только <код ошибки><сообщение> 
		future_len = sizeof(err_code_t) + qr.err_msg.size() + 1;

		cout << "sending err message (" << future_len << "=" << sizeof(err_code_t) << "+" << qr.err_msg.size() + 1<< " bytes)" << endl << flush;
		cout << "message: '" << qr.err_msg << "'" << endl << flush;
		SendInteger(future_len);
		SendData(&qr.err_code, sizeof(qr.err_code));
		SendData(qr.err_msg.c_str(), qr.err_msg.size()+1);

		return;
	}

	//если не ошибка -- отправляем <ВСЁ ОК><число матриц><матрица><матрица>...
	//<матрица> = <n><m>< n*m чисел>
	int matrix_n = qr.output.size();
	
	int overall_integers_n = 1;
	//общее число отправляемых int'ов

	for(auto matr_it = qr.output.begin(); matr_it != qr.output.end(); matr_it++) {
		overall_integers_n += 1 + matr_it->GetN() * matr_it->GetM();
	}

	future_len = sizeof(err_code_t) + overall_integers_n * sizeof(int);

	cout << "sending all that we want: " << matrix_n << " matrices (" << overall_integers_n << " integers) = " << future_len << " bytes." << endl << flush;

	SendData(&qr.err_code, sizeof(err_code_t));
	SendInteger(future_len);
	SendInteger(matrix_n);

	for(auto matr_it = qr.output.begin(); matr_it != qr.output.end(); matr_it++) {
		int n = matr_it->GetN();
		int m = matr_it->GetM();
		SendInteger(n);
		SendInteger(m);
		for(int i = 0; i < n; i++) {
			for(int j = 0; j < m; j++) {
				SendInteger(matr_it->GetElem(i,j));
			}
		}
	}


}

void DbServer::SendData(const void* pdata, size_t len) {
	write(client_fd_, pdata, len);
}

void DbServer::SendInteger(int num){
	cout << "sending integer num = " << num << "... " << flush;
	SendData(&num, sizeof(int));
	cout << "...sent." << endl << flush;
}

void DbServer::HandleQuery(){
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)
	cout <<  "enter HandleQuery" << endl << flush;

	GetInputMessage();
	
	QueryResult res = db_.InteractWithMatrixFromBuffer(buf_);

	SendQueryResult(res);
}
	

 // ------------ public: -----------------------
DbServer::DbServer(int port, const string filename):
	db_(filename),
	port_(port)
	{
	cout << "{creating server..." << endl;


	server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd_ == -1) {
		throw ServerException("cannot get socket");
	}

	if(port < 0) {
		port_ = DEFAULT_PORT;
	}
	cout << "using port = " << port_ << endl;

	address_.sin_family = AF_INET;
	address_.sin_addr .s_addr = INADDR_ANY;
	address_.sin_port = htons(port);

	address_len_ = sizeof(address_);

    /*проверка сокета на занятость*/
    int on = 1;
    int setsockopt_output = setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(setsockopt_output == -1) {
    	throw ServerException("socket is busy");
    }

    /* сопоставляем адрес с сокетом */
    int bind_output = bind(server_fd_, reinterpret_cast<struct sockaddr *>(&address_), sizeof(address_));
    if(bind_output == -1) {
    	throw ServerException("cannot bind socket to address");
    }

    int listen_output = listen(server_fd_, QUEUE_LEN);
    if(listen_output == -1) {
    	throw ServerException("i can't hear you");
    }

	cout << "done.}" << endl;
}

DbServer::~DbServer() {
	cout << "{shutting down server..." << endl;

	close(server_fd_);

	cout << "done.}" << endl;
}

void DbServer::MainLoop() {
	//в цикле: клиент подключается, передаёт матрицу,
	//получает обратно набор матриц, отключается.

	cout << "SERVER IS NOW ON" << endl;
	
	int cycle_iteration = 0;

//  while(true) {
	while(cycle_iteration < 10) {

		cycle_iteration++;
		cout << "entering the cycle" << endl;

		client_fd_ = accept(server_fd_, reinterpret_cast<struct sockaddr *>(&address_), &address_len_);
		if(client_fd_ < 0) {
			throw ServerException("cannot accept connection");
		}

		try {
			int message_type = 2;
			cout << message_type << endl;
			/*
			TODO: получение типа сообщения из вх.данных
			*/
			
//          if(message_type == ``всё хорошо'')
			if(true) {
				HandleQuery();
			}

//			if(message_type == ``получить информацию о базе'')
			if(false) {

			}

//			if(message_type == ``остановить сервер'') {
			if(true) {
				cout << "sending message about shutdown" << endl;
				SendQueryResult(QueryResult(ERRC_SHUTDOWN, "Recieved 'shutdown'."));
				break;//для while
			}
		}
		catch(ServerException se) {
			/*
			TODO: сообщение об ошибке 
			*/
		}
		catch(Exception e) {
			/*
			TODO: отправка клиенту сообщение об ошибке
			*/
		}
	}


	cout << "SERVER IS NOW OFF" << endl;
} 