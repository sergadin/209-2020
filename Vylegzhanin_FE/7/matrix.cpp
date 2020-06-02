#include <iostream>
#include <fstream>
using namespace std;

#include "matrix.h"
#include "exceptions.h"


//чтение/запись отдельных интов (чтоб не загромождать дальнейший код)
void write_int(ostream& out_stream, int* pnum) {
	out_stream.write(reinterpret_cast<char*>(pnum),sizeof(int));
}

void read_int(istream& in_stream, int* pnum) {
	in_stream.read(reinterpret_cast<char*>(pnum), sizeof(int));
}

int int_from_buffer(char* buf){
	int* ptr = reinterpret_cast<int*>(buf);
	return *ptr;
}

Matrix::~Matrix() {
	for(int i = 0; i < n_; i++) {
		delete[] data_[i];
	}

	delete[] data_;
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

Matrix::Matrix(int n, int m) {
	CheckDimensions(n,m);

	n_ = n;
	m_ = m;
	data_ = new int*[n];

	for(int i = 0; i < n; i++) {
		data_[i] = new int[m];
		for(int j = 0; j < m; j++) {
			data_[i][j] = 0;
		}
	}
}

Matrix::Matrix(int n, int m, istream& in_stream) {
	if (!in_stream) {
		throw NullPtrException("bad istream& in_stream");
	}

	CheckDimensions(n,m);
	
	n_ = n;
	m_ = m;
	data_ = new int*[n];

	for(int i = 0; i < n; i++) {
		data_[i] = new int[m];
		for(int j = 0; j < m; j++) {
			read_int(in_stream, &data_[i][j]);
		}
	}
}

Matrix::Matrix(char* buf) {
	if(!buf) {
		throw NullPtrException("bad char* buffer");
	}

	n_ = int_from_buffer(buf);
	buf += sizeof(int);
	m_ = int_from_buffer(buf);
	buf += sizeof(int);

	CheckDimensions(n_,m_);


	data_ = new int*[n_];

	for(int i = 0; i < n_; i++) {
		data_[i] = new int[m_];
		for(int j = 0; j < m_; j++) {
			data_[i][j] = int_from_buffer(buf);
			buf += sizeof(int);
		}
	}
}

Matrix::Matrix(int n, int m, int** data) {
	if (data == NULL) {
		throw NullPtrException("bad int* data");
	}

	CheckDimensions(n,m);

	n_ = n;
	m_ = m;
	data_ = data;

	//лучше упадёт сейчас, чем непонятно когда
	//(проверка, что данные по этому
	//указателю вообще доступны)
	int t = 0;
	for(int i = 0; i < n; i++) {
		t += data[i][m-1];
	}
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

void Matrix::WriteToOstream(ostream& fout) const {
	for(int i = 0; i < n_; i++) {
		for(int j = 0; j < n_; j++) {
			write_int(fout, &data_[i][j]);
		}
	}
}

void Matrix::CheckDimensions(int n, int m) {
	if(n <= 0 || m <= 0) {
		throw MatrixSizeException("non-positive matrix dimensions", n, m);
	}
}

int Matrix::GetN() const {return n_;}
int Matrix::GetM() const {return m_;}
int Matrix::GetElem(int i, int j) const {
	if(i < 0 || i > n_ || j < 0 || j > m_) {
		throw MatrixElemException(i,j,n_,m_);
	}

	return data_[i][j];
}

Matrix operator*(const Matrix& lhs, const Matrix& rhs) {
	cout << "trying to multiply" << lhs.GetN() << "x" << lhs.GetM() << " and " << rhs.GetN() << "x" << rhs.GetM() << endl << flush;

	if(lhs.GetM() != rhs.GetN()) {
		throw MatrixSizeException("non-consistent dimensions while multiplicating", lhs.GetM(), rhs.GetN());
	}

	int new_n = lhs.GetN();
	int new_m = rhs.GetM();
	int mid_size = lhs.GetM();

	int** newdata = new int*[new_n];
	for(int i = 0; i < new_n; i++) {
		newdata[i] = new int[new_m];

		for(int j = 0; j < new_m; j++) {
			newdata[i][j] = 0;
			for(int k = 0; k < mid_size; k++) {
//				cout << newdata[i][j] << "+=" << lhs.data_[i][k] << '*' << other.data_[k][j] << endl;
				newdata[i][j] += lhs.GetElem(i,k) * rhs.GetElem(k,j);
			}
		}
	}

	return Matrix(new_n, new_m, newdata);
}

bool operator<(const Matrix& lhs, const Matrix& rhs) {
	int ln = lhs.GetN(), rn = rhs.GetN();
	int lm = lhs.GetM(), rm = rhs.GetM();
	
	if(ln < rn) {
		return true;
	}
	if(ln > rn) {
		return false;
	}
	if(lm < rm) {
		return true;
	}
	if(lm > rm) {
		return false;
	}
	for(int i = 0; i < ln; i++) {
		for(int j = 0; j < lm; j++) {
			int le = lhs.GetElem(i,j);
			int re = rhs.GetElem(i,j);
			if(le < re) {
				return true;
			}
			if(le > re) {
				return false;
			}
		}
	}

	return false;
}



