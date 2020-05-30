#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
using namespace std;

void write_int(ofstream& fout, int* pnum);
void  read_int(ifstream& fin , int* pnum);
//краткая запись для read и write

struct Matrix {
	int n_, m_;
	int** data_;

	~Matrix();
	Matrix(const Matrix& other);

	Matrix(int n, int m, int** data);
	//динамический массив после этого конструктора связывается  
	//с матрицей (в частности, удаляется её деструктором,
	//как только матрица перестаёт существовать)
	
	Matrix(int n, int m, ifstream& fin);
	//двоичные данные забираются из потока

	Matrix& operator=(const Matrix& other);

	void Print() const;
	//распечатка в cout

	void WriteToFout(ofstream& fout) const;
	//печать содержимого в поток в двоичном формате

	void CheckDimensions(const string& funcname, int n, int m);
	//проверка на отрицательность, вызов исключения
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);
//обычное матричное умножение

bool   operator<(const Matrix& lhs, const Matrix& rhs);
//оператор сравнения нужен для корректной работы std::set<Matrix>

#endif //MATRIX_H