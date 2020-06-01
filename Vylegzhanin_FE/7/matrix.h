#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
using namespace std;

void write_int(ostream& out_stream, int* pnum);
void  read_int(istream&  in_stream, int* pnum);
//краткая запись для read и write

class Matrix {
	int n_, m_;
	int** data_;


public:
	~Matrix();
	Matrix(const Matrix& other);

	Matrix(int n, int m);
	//пустая матрица

	Matrix(int n, int m, istream& fin);
	//_двоичные_ данные забираются из потока

	Matrix(int n, int m, int** data);
	//динамический массив после этого конструктора связывается  
	//с матрицей (в частности, удаляется её деструктором,
	//как только матрица перестаёт существовать)
	

	Matrix& operator=(const Matrix& other);

	void Print() const;
	//распечатка в cout

	void WriteToOstream(ostream& fout) const;
	//печать содержимого в поток в двоичном формате

	void CheckDimensions(int n, int m);
	//проверка на отрицательность, вызов исключения

	int GetN() const;
	int GetM() const;
	int GetElem(int i, int j) const;
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs);
//обычное матричное умножение

bool   operator<(const Matrix& lhs, const Matrix& rhs);
//оператор сравнения нужен для корректной работы std::set<Matrix>

#endif //MATRIX_H