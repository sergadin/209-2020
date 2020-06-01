#include <iostream>
using namespace std;

#include "db.h"
#include "matrix.h"


int main() {
	int** A = new int*[2];
	A[0] = new int[2];
	A[1] = new int[2];
	A[0][0] = 0;
	A[0][1] = -1;
	A[1][0] = 1;
	A[1][1] = 0;
	Matrix ma(2,2,A);

//	Matrix mc = ma * ma;

	Database db("temp.dbase");
	
	QueryResult qr = db.InteractWithMatrix(ma);
	cout << "err_msg='" << qr.err_msg << "'; res:" << endl;
	for(auto it = qr.output.begin(); it != qr.output.end(); it++) {
		it->Print();
	}
//	db.AddMatrix(ma);
//	db.AddMatrix(mc);


//	db.PrintInfo();

//	cout << mc.data_[0][0] << " " << mc.data_[0][1] << endl;
//	cout << mc.data_[1][0] << " " << mc.data_[1][1] << endl;


}