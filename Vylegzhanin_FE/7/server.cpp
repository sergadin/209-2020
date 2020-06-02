#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <cstdio>

using namespace std;

#include "server.h"

void DbServer::GetInputMessage(int length) {
	//получает от соединения сообщение заданной длины
	//и сохраняет его в буфер

	bzero(buf_, BUFFER_SIZE);
	//очистка буфера

	int read_bytes = GetData(buf_, length);

	if(read_bytes != length) {
		cout << "read " << read_bytes << " bytes instead of claimed " << length << endl;
		throw QueryException("read wrong amount of bytes");
	}
	else {
		cout << "successfully read " << length << " bytes" << endl << flush;
	}
}

void DbServer::SendQueryResult(QueryResult qr) {
	//отправляет ответ на запрос по текущему адресу

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

//	cout << "sending all that we want: " << matrix_n << " matrices (" << overall_integers_n << " integers) = " << future_len << " bytes." << endl << flush;

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

int DbServer::GetData(void* pdata, size_t len) {
	return recv(current_client_fd_->GetFd(), pdata, len, MSG_WAITALL);
}

void DbServer::SendData(const void* pdata, size_t len) {
//	cout << "sending something (" << len << " bytes)" << "... "<<flush;
	write(current_client_fd_->GetFd(), pdata, len);
//	cout <<"...sent." << endl << flush;
}

int DbServer::GetInteger(int* pnum) {
	return recv(current_client_fd_->GetFd(), pnum, sizeof(int), MSG_WAITALL);
}

void DbServer::SendInteger(int num){
//	cout << "sending integer num = " << num << "... " << flush;
	write(current_client_fd_->GetFd(), &num, sizeof(int));//удобнее для дебага
//	cout << "...sent." << endl << flush;
}

void DbServer::HandleQuery(int length){
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)
//	cout <<  "enter HandleQuery" << endl << flush;
	try {
		GetInputMessage(length);
	}
	catch(QueryException qe) {
		SendQueryResult(QueryResult(ERRC_BADDATA, qe.Message()));
	}
	catch(...) {
		cout << "unknown exception in DbServer::HandleQuery" << endl;
		SendQueryResult(QueryResult(ERRC_UNKNOWN, "unknown exception in DbServer::HandleQuery"));
		return;
	}

	QueryResult res = db_.InteractWithMatrixFromBuffer(buf_);

	SendQueryResult(res);
}
	

 // ------------ public: -----------------------
DbServer::DbServer(int port, const string filename):
	db_(filename),
	port_(port),
	buf_(),
	server_fd_(socket(AF_INET,SOCK_STREAM, 0)),
	rfds_(),
	clients_(1,Client(server_fd_)),
	time_to_stop_(false)
	{

	cout << "{creating server..." << endl;

//	server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
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


  //  clients_.push_back(Client(server_fd_));
  //  time_to_stop_ = false;

	cout << "done.}" << endl;
}

DbServer::~DbServer() {
	cout << "{shutting down server..." << endl << flush;

	close(server_fd_);

	cout << "done.}" << endl << flush;
}


void DbServer::InteractWithCurrentClient() {

	cout << "starting to interact with client " << current_client_fd_->GetFd() << "; waiting for data..." << endl;

	int future_len = 0;
		
	GetInteger(&future_len);
	cout << "received future_len = " << future_len << endl;

	if(future_len < 0) {
		cout << "negative future_len" << endl;
		throw QueryException("message len should be positive");
	}

	if(future_len == 0) {
	//пустое сообщение посылается, если клиент разрывает соединение
		cout << "client " << current_client_fd_->GetFd() << " left." << endl;
		clients_.erase(current_client_fd_);
		return;
	}

	if(future_len == BUFFER_SIZE) {
		cout << "message is too long" << endl;
		throw QueryException("message is too long");
	}


	int code;
	GetData(&code, sizeof(input_code_t));
	cout << "input_code = " << code << endl << flush;
	
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
		cout << "message sent" << endl;
		time_to_stop_ = true;
	}

}

void DbServer::MainLoop() {
	//в цикле: клиент подключается, передаёт матрицу,
	//получает обратно набор матриц, отключается.

	cout << "SERVER IS NOW ON" << endl;
	
//	int cycle_iteration = 0;
	try{

		while(!time_to_stop_) {
//			cout << "entering the cycle" << endl;
			FD_ZERO(&rfds_);
			FD_SET(server_fd_, &rfds_);
			int max_fd = 0;

			for(auto it = clients_.begin(); it != clients_.end(); it++) {
				FD_SET(it->GetFd(), &rfds_);

				if(it->GetFd() > max_fd) {
					max_fd = it->GetFd();
				}
			}

			struct timeval tv = MAXIMAL_TIMEOUT;

//			cout << "max_fd=" << max_fd << endl;
			int select_output =  select(max_fd+1, &rfds_, NULL, NULL, &tv);
			if(select_output == -1) {
				throw ServerException("cannot select");
			}
			if(select_output == 0) {
				continue;
			}


			cout << "there's something to select" << endl;
			for(auto it = clients_.begin()+1; it < clients_.end(); it++) {
				if(FD_ISSET(it->GetFd(), &rfds_)) {
					current_client_fd_ = it;
					//current_client_fd_ = it->GetFd();
					InteractWithCurrentClient();
				}
			}
			if(FD_ISSET(server_fd_, &rfds_)) {
				cout << "new client" << endl;
				int new_client_sock = accept(server_fd_, NULL, NULL);
				clients_.push_back(Client(new_client_sock));
			}
		}
	}
	catch(ServerException se) {
		cout << "ошибка на сервере" << endl;
		cout << se.Message() << endl;

		return;
		/*
		TODO: сообщение об ошибке 
		*/
	}
	catch(Exception e) {
		cout << "неизвестная ошибка" << endl;
		cout << e.Message() << endl;
		return;
		/*
		TODO: отправка клиенту сообщение об ошибке
		*/
	}

	cout << "SERVER IS NOW OFF" << endl;
} 