#include <fstream>
#include <iostream>
#include <map>
#include <set>
using namespace std;

#include "exceptions.h"
#include "db.h"



Database::~Database() {
	SaveToFile();
}

void Database::SaveToFile() const {
	ofstream fout(filename_);
	if(!fout) {
		throw ServerException("Database::SaveToFile", "Unable to save file");
	}
	fout.write("DBASE",5);

	int db_size = data_.size();

	write_int(fout, &db_size);

	for(auto it = data_.begin(); it != data_.end(); it++) {

		int m = it->first;
		const set<Matrix>& m_set = it->second;
		int set_size = m_set.size();


		cout << "Saving matrices with m=" << m << endl;

		write_int(fout, &m);
		write_int(fout, &set_size);

		for(auto set_it = m_set.begin(); set_it != m_set.end(); set_it++) {
			int n = set_it->n_;
			write_int(fout, &n);
			set_it->WriteToFout(fout);//печать матрицы
		}
	}

	fout.close();

}

Database::Database(const string filename) {
	ReloadFromFile(filename);
}

void Database::ReloadFromFile(const string filename) {
	filename_ = filename;
	ifstream fin(filename);

	char test_extension[6] = "";
	fin.read(test_extension, 5);
	if(strcmp(test_extension,"DBASE")!= 0) {
		cout << "file don't have correct extension! database is considered empty" << endl;
		return;
	}

	int db_size;
	read_int(fin, &db_size);
	for(int db_i = 0; db_i < db_size; db_i++) {
		int m;
		set<Matrix> m_set;
		int set_size;

		read_int(fin, &m);
		read_int(fin, &set_size);

		for(int set_j = 0; set_j < set_size; set_j++) {
			int n;
			read_int(fin, &n);
			m_set.insert(Matrix(n,m,fin));
			//считывание матрицы из входных данных (см. соотв. конструктор)
		}
		data_.insert({m,m_set});
	}

	fin.close();
}

bool Database::ContainsMatricesWithWidth(int m) const {
	return (data_.find(m) != data_.end());
}

const set<Matrix>& Database::MatricesWithWidth(int m) const {
	if(!ContainsMatricesWithWidth(m)) {
		throw DatabaseException("Database::MatricesWithWidth", "matrices with that width not found");
	}

	return data_.find(m)->second;
}

void Database::InsertMatrix(const Matrix& mat) {
	int m = mat.m_;
	set<Matrix> new_set;

	if (ContainsMatricesWithWidth(m)) {
		auto pos = data_.find(m);
		new_set = pos->second;
		data_.erase(pos);
	}
	//чтобы добавить элемент в уже существующий
	//список, нужно сначала убрать его из set'а
	new_set.insert(mat);
	data_.insert({m, new_set});
}

void Database::Clear() {
	data_.clear();
}

void Database::PrintInfo() const {
	cout << "{---INFO ABOUT DB----" << endl;
	int total_num = 0;
	for(auto it = data_.begin(); it != data_.end(); it++) {
		int curr_num = (it->second).size();
		total_num += curr_num; 
	
		cout << "of form [(-)x" << it->first << "]: " << curr_num << " matrices." << endl;

		//--{более подробное---
		///*
		cout << "namely:" << endl;
		for(auto m_it = (it->second).begin(); m_it != (it->second).end(); m_it++) {
			m_it->Print();
		}
		cout << endl;
		//*/
		//---более подробное}--
		
	}
	cout << "totally: " << total_num <<  " matrices." << endl;
		cout << "----INFO ABOUT DB---}" << endl << endl;
}

QueryResult Database::InteractWithMatrix(const Matrix& mat) {
	QueryResult result;
	result.err_code = 0;

	cout << "db is interacting with mat=" << endl;
	mat.Print();
	cout << "let us see..." << endl;
	if(!ContainsMatricesWithWidth(mat.m_)) {
		cout << "found no matrices to multiply with" << endl;
		cout << "try adding new matrix to the base..." << endl;
		InsertMatrix(mat);
		cout << "added successfully." << endl;
		return result;
	}


	const set<Matrix>& right_multiplies = MatricesWithWidth(mat.m_);


	cout << "found " << right_multiplies.size() << " matrices to multiply with. namely: {{" << endl; 

	for(auto it = right_multiplies.begin(); it != right_multiplies.end(); it++) {
		cout << "we can multiply by" << endl;
		it->Print();
		cout << endl << "and get" << endl;
		Matrix temp = mat * (*it);
		temp.Print();
		cout << endl;
		result.output.push_back(temp);

//		result.output.push_back(mat * (*it));
	}
	cout << "}}. returning this list." << endl;
	return result;
}

QueryResult Database::InteractWithMatrixFromIfstream(ifstream& fin) {
	int n,m;
	fin >> n;
	fin >> m;
	return InteractWithMatrix(Matrix(n, m, fin));
}