#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstring>
using namespace std;

class Exception {
	string funcname_;
 	string msg_;
public:
 	Exception(const string& funcname, const string& msg):
 		 funcname_(funcname),
 		 msg_     (     msg)
 		{}
 	const string& Message () const {return      msg_;}
 	const string& FuncName() const {return funcname_;}
};

class MatrixSizeException: public Exception {
	int n_;
	int m_;
public:
	MatrixSizeException(const string& funcname, const string& msg, int n, int m):
		Exception(funcname, msg),
		n_(n),
		m_(m)
		{}
	int GetN() const {return n_;}
	int GetM() const {return m_;}
};

class NullPtrException: public Exception {
public:
	NullPtrException(const string& funcname, const string& msg):
		Exception(funcname, msg)
		{}
};

class ServerException: public Exception {
public:
	ServerException(const string& funcname, const string& msg):
		Exception(funcname, msg)
		{}
};


class DatabaseException: public ServerException {
public:
	DatabaseException(const string& funcname, const string& msg):
		ServerException(funcname, msg)
		{}	
};

#endif //EXCEPTIONS_H