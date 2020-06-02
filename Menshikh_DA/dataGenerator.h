#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<fstream>

using namespace std;

string generate_word(int a){
	string word="qwe";
	int j=a;
	char t='a';
	while(j>0){
		t+=j%26;
		j=j/26;
		word+=t;
		t='a';
	}
	return word;
}

void generate_data(int size, const string &generated_recipe_file, const string &generated_items_file){
	vector<string> items;
	ifstream fin;
	fin.open("micraft_data.txt");
	string s;
	while(getline(fin, s)){
		items.push_back(s);
	}
	fin.close();
	ofstream ofs(generated_recipe_file);
	int m=items.size();
	for(int i=0;i<m;i++)
		ofs << items[i] << ", \n";
	for(int i=0;i<size;i++){
		string word=generate_word(i);
		srand(i*i-2);
		ofs << word << ", ";
		int t=rand()%10;
		for(int j=0;j<t;j++){
			int l=rand()%m;
			ofs << items[l]<<", "<<rand()%121+1<<", ";
		}
		items.push_back(word);
		ofs << endl;
		m++;
	}
	ofstream of(generated_items_file);
	for(int i=0;i<m;i++){
		of << items[i] << ", "<< rand()%(m+i)<<endl;
	}
}

/*int main(){
	string s1="r.txt", s2="i.txt";
	generate_data(100, s1, s2);
}*/





