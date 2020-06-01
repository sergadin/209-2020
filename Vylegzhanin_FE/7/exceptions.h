#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstring>
using namespace std;

class Exception {
	string funcname_;
 	string msg_;
public:
 	Exception(const string& msg):
 		 msg_     (     msg)
 		{}
 	const string& Message () const {return      msg_;}
};

class MatrixElemException: public Exception {
	int i_;
	int j_;
	int n_;
	int m_;
public:
	MatrixElemException(int i, int j, int n, int m):
		Exception("Out of matrix's range"),
		i_(i),
		j_(j),
		n_(n),
		m_(m)
		{}
	int GetI() const {return i_;}
	int GetJ() const {return j_;}	
	int GetN() const {return n_;}
	int GetM() const {return m_;}	
};

class MatrixSizeException: public Exception {
	int n_;
	int m_;
public:
	MatrixSizeException(const string& msg, int n, int m):
		Exception(msg),
		n_(n),
		m_(m)
		{}
	int GetN() const {return n_;}
	int GetM() const {return m_;}
};

class NullPtrException: public Exception {
public:
	NullPtrException(const string& msg):
		Exception(msg)
		{}
};

class ServerException: public Exception {
public:
	ServerException(const string& msg):
		Exception(msg)
		{}
};


class DatabaseException: public Exception {
public:
	DatabaseException(const string& msg):
		Exception(msg)
		{}	
};

#endif //EXCEPTIONS_H