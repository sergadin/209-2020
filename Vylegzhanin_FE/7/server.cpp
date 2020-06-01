#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <cstdio>

using namespace std;

#include "server.h"

void DbServer::GetInputMessage(int length) {
	//получает от соединения сообщение
	//(перед которым отправлена его длина)
	//и сохраняет его в буфер

	
	bzero(buf_, BUFFER_SIZE);

	int read_bytes = recv(client_fd_, buf_, length, MSG_WAITALL);

	if(read_bytes < length) {
		cout << "read " << read_bytes << " bytes instead of claimed " << length << endl;
		throw QueryException("message must be not full");
	}
	else {
		cout << "successfully read " << read_bytes << "=" <<  length << " bytes" << endl << flush;
	}
}



void DbServer::SendQueryResult(QueryResult qr) {
	//отправляет ответ на запрос по текущему адресу
	cout << "enter SendQueryResult" << endl << flush;

	int future_len = 0;
	if(qr.err_code != ERRC_OK) {
		//если ошибка -- отправляем только <код ошибки><сообщение> 
		future_len = sizeof(err_code_t) + qr.err_msg.size();

		cout << "sending err message (" << future_len << "=" << sizeof(err_code_t) << "+" << qr.err_msg.size()<< " bytes)" << endl << flush;
		cout << "message: '" << qr.err_msg << "'" << endl << flush;
		SendInteger(future_len);
		SendData(&qr.err_code, sizeof(qr.err_code));
		SendData(qr.err_msg.c_str(), qr.err_msg.size());

		return;
	}

	//если не ошибка -- отправляем <ВСЁ ОК><число матриц><матрица><матрица>...
	//<матрица> = <n><m>< n*m чисел>
	int matrix_n = qr.output.size();
	
	int overall_integers_n = 1;
	//общее число отправляемых int'ов

	for(auto matr_it = qr.output.begin(); matr_it != qr.output.end(); matr_it++) {
		overall_integers_n += 2 + matr_it->GetN() * matr_it->GetM();
	}

	future_len = sizeof(err_code_t) + overall_integers_n * sizeof(int);

	cout << "sending all that we want: " << matrix_n << " matrices (" << overall_integers_n << " integers) = " << future_len << " bytes." << endl << flush;

	SendInteger(future_len);
	SendData(&qr.err_code, sizeof(err_code_t));
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
	cout << "sending something (" << len << " bytes)" << "... "<<flush;
	write(client_fd_, pdata, len);
	cout <<"...sent." << endl << flush;
}

void DbServer::SendInteger(int num){
	cout << "sending integer num = " << num << "... " << flush;
//	SendData(&num, sizeof(int));
	write(client_fd_, &num, sizeof(int));//удобнее для дебага
	cout << "...sent." << endl << flush;
}

void DbServer::HandleQuery(int length){
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)
	cout <<  "enter HandleQuery" << endl << flush;

	GetInputMessage(length);
	
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
	
//	int cycle_iteration = 0;

	while(true) {
		cout << "entering the cycle" << endl;

		client_fd_ = accept(server_fd_, reinterpret_cast<struct sockaddr *>(&address_), &address_len_);
		if(client_fd_ < 0) {
			throw ServerException("cannot accept connection");
		}
		cout << "connected; waiting for data..." << flush;
		try {
			int future_len = 0;
				
			recv(client_fd_, &future_len, sizeof(int), MSG_WAITALL);

			cout << "received future_len = " << future_len << endl;

			if(future_len < 0) {
				cout << "negative future_len" << endl;
				throw QueryException("message len should be positive");
			}

			if(future_len == BUFFER_SIZE) {
				cout << "message is too long" << endl;
				throw QueryException("message is too long");
			}


			int code;
			recv(client_fd_, &code, sizeof(input_code_t), MSG_WAITALL);
			cout << "input_code = " << code << endl << flush;
			/*
			TODO: получение типа сообщения из вх.данных
			*/
			
			if(code == Q_STANDARD) {
				cout << "got standard type; len = " << future_len - sizeof(int) << "; let's handle it!" << endl;
				HandleQuery(future_len - sizeof(int));
				cout << "query answered." << endl << flush;
			}

			if(code == Q_CLEAR) {
				db_.Clear();
				SendQueryResult(QueryResult());
			}

			if(code == Q_SHUTDOWN) {
				cout << "sending message about shutdown" << endl;
				SendQueryResult(QueryResult(ERRC_SHUTDOWN, "Recieved 'shutdown'."));
				break;//для while
			}
		}
		catch(ServerException se) {
			cout << "ошибка на сервере" << endl;
			cout << se.Message() << endl;
			break;
			/*
			TODO: сообщение об ошибке 
			*/
		}
		catch(Exception e) {
			cout << "неизвестная ошибка" << endl;
			cout << e.Message() << endl;
			break;
			/*
			TODO: отправка клиенту сообщение об ошибке
			*/
		}
	}


	cout << "SERVER IS NOW OFF" << endl;
} 