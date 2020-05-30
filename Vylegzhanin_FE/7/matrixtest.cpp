#include <iostream>
#include "db.h"
#include "matrix.h"

using namespace std;

int main() {
	/*int** A = new int*[2];
	A[0] = new int[2];
	A[1] = new int[2];
	A[0][0] = 7;
	A[0][1] = 10;
	A[1][0] = 15;
	A[1][1] = 22;
	Matrix ma(2,2,A);*/

//	Matrix mc = ma * ma;

	Database db("temp.dbase");
	

//	db.AddMatrix(ma);
//	db.AddMatrix(mc);


	db.PrintInfo();

//	cout << mc.data_[0][0] << " " << mc.data_[0][1] << endl;
//	cout << mc.data_[1][0] << " " << mc.data_[1][1] << endl;


}