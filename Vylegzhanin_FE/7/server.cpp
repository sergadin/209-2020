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



void DbServer::SendQueryResult(int ms, QueryResult qr) {
	//отправляет по данному соединению
	//набор матриц вместе с необх.данными (размерность, код ошибки)

	/*
	TODO

	*/
}



void DbServer::HandleQuery(int ms_input, int ms_output) {
	/*
		TODO
	*/

	char* str = GetInputMessage(ms_input);
	ifstream fin;
	/*
	TODO
	*/

	SendQueryResult(
		ms_output,
		db_.InteractWithMatrixFromIfstream(fin)
		//вместо второй строчки нужно что-то аналогичное,
		//но связанное с результатом работы GetInputMessage

	);

	delete str;
}
	//выполняется в цикле: получает запрос и отвечает на него
	//(используя другие функции)

 // ------------ public: -----------------------
DbServer::DbServer(int port, const string filename):
	db_(filename) {
	/*
		TODO
	*/
}

DbServer::~DbServer() {
	/*
	TODO
	*/
}

void DbServer::MainLoop() {
	//клиент подключается, передаёт матрицу,
	//получает обратно набор матриц, отключается.

	/*
	TODO
	*/
}