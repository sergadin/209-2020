#ifndef DB_H
#define DB_H

#include <map>
#include <set>

#include "matrix.h"
#include "exceptions.h"

using namespace std;

class Database {
	map<int, set<Matrix> > data_;
	string filename_;

public:
	Database(const string filename);
	//конструктор, который делает ReloadFromFile

	void ReloadFromFile(const string filename);
	//заменяет filename_ на новый,
	//старую информацию -- на содержимое файла

	~Database();
	void SaveToFile() const;

	bool ContainsMatricesWithWidth(int m) const;
	const set<Matrix>& MatricesWithWidth(int m) const;

	void AddMatrix(const Matrix& mat);

	void Clear();

	void PrintInfo() const;
};

#endif //DB_H