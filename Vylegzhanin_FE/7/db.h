#ifndef DB_H
#define DB_H

#include <map>
#include <set>
#include <vector>
using namespace std;

#include "matrix.h"
#include "exceptions.h"

struct QueryResult {
	int err_code;
	vector<Matrix> output;
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

	//бесполезная обёртка; надо или обобщить, или заменить, или убрать
	QueryResult InteractWithMatrixFromIfstream(ifstream& fin);
};

#endif //DB_H