#include <iostream>
#include <fstream>
#include <istream>
#include <string>

using namespace std;

#include "server.h"

char* DbServer::GetInputMessage(int ms) {
	//получает от соединения сообщение
	//(перед которым отправлена его длина).
	//!!этот динамический массив надо будет освободить!!
	
	/*
	TODO
	*/

	char* t = new char[20];
	return t;

}



void DbServer::SendQueryResult(QueryResult qr) {
	//отправляет ответ на запрос
	int future_len = 0;
	if(qr.err_code != ERRC_OK) {
		//если ошибка -- отправляем только <код ошибки><сообщение>
		future_len = sizeof(err_code_t) + qr.err_msg.size();
	
		SendInteger(future_len);
		write(as_, &qr.err_code, sizeof(err_code_t));
		write(as_, qr.err_msg.c_str(), qr.err_msg.size());
	
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

	write(as_, &qr.err_code, sizeof(err_code_t));
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

void DbServer::SendInteger(int num) {
	write(as_, &num, sizeof(int));
}

void DbServer::HandleQuery(int ms_input) {
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)

	char* input = GetInputMessage(ms_input);
	istringstream in_stream(input);
	
	QueryResult res = db_.InteractWithMatrixFromBinaryStream(in_stream);

	SendQueryResult(res);

	delete input;
}
	

 // ------------ public: -----------------------
DbServer::DbServer(int port, const string filename):
	db_(filename) {
	cout << "{creating server..." << endl;

	/*
		TODO: разворачивание сервера
	*/

	cout << "done.}" << endl;
}

DbServer::~DbServer() {
	cout << "{shutting down server..." << endl;
	/*
	TODO: сворачивание сервера
	*/

	cout << "done.}" << endl;
}

void DbServer::MainLoop() {
	//клиент подключается, передаёт матрицу,
	//получает обратно набор матриц, отключается.

	cout << "SERVER IS NOW ON" << endl;
	/*
	TODO
	*/

	while(true) {


//		if(ОСТАНОВКА) {
		if(true) {
			SendQueryResult(QueryResult(ERRC_SHUTDOWN, "Recieved 'shutdown'."));
			break;
		}
	}


	cout << "SERVER IS NOW OFF" << endl;
} 