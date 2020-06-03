#ifndef DB_H
#define DB_H

#include <map>
#include <set>
#include <vector>
#include <sstream>
using namespace std;

#include "matrix.h"
#include "exceptions.h"

typedef enum{ERRC_OK, ERRC_BADISTREAM, ERRC_BADDATA, ERRC_SHUTDOWN, ERRC_UNKNOWN} err_code_t;

struct QueryResult {
	err_code_t err_code;
	string err_msg;
	vector<Matrix> output;

public:
	QueryResult():
		err_code(ERRC_OK),
		err_msg(),
		output()
		{}

	QueryResult(vector<Matrix>& data):
		err_code(ERRC_OK),
		err_msg(),
		output(data)
		{}

	QueryResult(err_code_t code, string msg):
		err_code(code),
		err_msg(msg),
		output()
		{}
};

class Database {
	map<int, set<Matrix> > data_;
	string filename_;

	bool ContainsMatricesWithWidth(int m) const;
	const set<Matrix>& MatricesWithWidth(int m) const;

public:
	Database(const string filename);
	//конструктор, который делает ReloadFromFile

	void ReloadFromFile(const string filename);
	//заменяет filename_ на новый,
	//старую информацию -- на содержимое файла

	~Database();
	void SaveToFile() const;
	
	void Clear();
	void PrintInfo() const;

	void InsertMatrix(const Matrix& mat);

	QueryResult InteractWithMatrix(const Matrix& mat);

	QueryResult InteractWithMatrixFromBuffer(char* buf);
};

#endif //DB_H