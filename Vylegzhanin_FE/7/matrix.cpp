#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <iostream>
#include <fstream>
#include "matrix.h"
#include "exceptions.h"
using namespace std;


//чтение/запись отдельных интов (чтоб не загромождать)
void write_int(ofstream& fout, int* pnum) {
	fout.write(reinterpret_cast<char*>(pnum),sizeof(int));
}

void read_int(ifstream& fin, int* pnum) {
	fin.read(reinterpret_cast<char*>(pnum), sizeof(int));
}


Matrix& Matrix::operator=(const Matrix& other) {
	if(this == &other) {
		return *this;
	}

	for(int i = 0; i < n_; i++) {
		delete[] data_[i];
	}
	delete[] data_;

	n_ = other.n_;
	m_ = other.m_;
	data_ = new int*[n_];
	for(int i = 0; i < n_; i++) {
		data_[i] = new int[m_];

		for(int j = 0; j < m_; j++){
			data_[i][j] = other.data_[i][j];
		}
	}

	return *this;
}

void Matrix::Print() const {
	cout << "matrix " << n_ << "x" << m_ << ":" << endl;

	for(int i = 0; i < n_; i++) {
		for(int j = 0; j < m_; j++) {
			cout << data_[i][j] << " ";
		}
		cout << endl;
	}
}

void Matrix::WriteToFout(ofstream& fout) const {
	for(int i = 0; i < n_; i++) {
		for(int j = 0; j < n_; j++) {
			write_int(fout, &data_[i][j]);
		}
	}
}

Matrix::Matrix(int n, int m, ifstream& fin) {
	if (!fin) {
		throw NullPtrException("Matrix::Matrix(raw data)", "bad data pointer");
	}
	if (n <= 0 || m <= 0) {
		throw MatrixSizeException("Matrix::Matrix(raw data)", "non-positive Matrix dimensions", n, m);
	}
	n_ = n;
	m_ = m;
	data_ = new int*[n];

	for(int i = 0; i < n; i++) {
		data_[i] = new int[m];
		for(int j = 0; j < m; j++) {
			read_int(fin, &data_[i][j]);
		}
	}
}

Matrix::Matrix(int n, int m, int** data) {
	if (data == NULL) {
		throw NullPtrException("Matrix::Matrix(raw data)", "bad data pointer");
	}
	if (n <= 0 || m <= 0) {
		throw MatrixSizeException("Matrix::Matrix(raw data)", "non-positive Matrix dimensions", n, m);
	}
	n_ = n;
	m_ = m;
	data_ = data;

	//лучше сейчас, чем никогда
	//(проверка, что данные по этому
	//указателю вообще доступны)
	int t = 0;
	for(int i = 0; i < n; i++) {
		t += data[i][m-1];
	}
}

Matrix::Matrix(const Matrix& other) {
	n_ = other.n_;
	m_ = other.m_;
	data_ = new int*[n_];
	for(int i = 0; i < n_; i++) {
		data_[i] = new int[m_];

		for(int j = 0; j < m_; j++){
			data_[i][j] = other.data_[i][j];
		}
	}
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {


	if(lhs.m_ != rhs.n_) {
		throw MatrixSizeException("Matrix::operator*", "non-consistent dimensions while multiplicating", lhs.m_, rhs.n_);
	}

	int new_n = lhs.n_;
	int new_m = rhs.m_;
	int mid_size = lhs.m_;

	int** newdata = new int*[new_n];
	for(int i = 0; i < new_n; i++) {
		newdata[i] = new int[new_m];

		for(int j = 0; j < new_m; j++) {
			newdata[i][j] = 0;
			for(int k = 0; k < mid_size; k++) {
//				cout << newdata[i][j] << "+=" << lhs.data_[i][k] << '*' << other.data_[k][j] << endl;
				newdata[i][j] += lhs.data_[i][k] * rhs.data_[k][j];
			}
		}
	}

	return Matrix(new_n, new_m, newdata);
}

bool operator<(const Matrix& lhs, const Matrix& rhs) {
	if(lhs.n_ < rhs.n_) {
		return true;
	}
	if(lhs.n_ > rhs.n_) {
		return false;
	}
	if(lhs.m_ < rhs.m_) {
		return true;
	}
	if(lhs.m_ > rhs.m_) {
		return false;
	}
	for(int i = 0; i < lhs.n_; i++) {
		for(int j = 0; j < lhs.m_; j++) {
			if(lhs.data_[i][j] < rhs.data_[i][j]) {
				return true;
			}
			if(lhs.data_[i][j] > rhs.data_[i][j]) {
				return false;
			}
		}
	}

	return false;
}



Matrix::~Matrix() {
	for(int i = 0; i < n_; i++) {
		delete[] data_[i];
	}

	delete[] data_;
}


#endif //MATRIX_CPP