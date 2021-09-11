#include<iostream>
#include<string>
#include<fstream>
#include"database.h"

using namespace std;

Database::Database(){

}

Database::Database(const std::string &file_items, const std::string &file_recipe){//get data from files
	std::ifstream fan;
	fan.open(file_items);
	std::string str;
	while(getline(fan,str))
	{
		std::stringstream ss(str);
		std::string x;
		std::getline(ss,x,',');
		int t;
		ss  >>t;
		avalableitems[x]=t;
	}	
	std::ifstream fin1;
	fin1.open(file_recipe);
	while(getline(fin1,str))
	{
		std::stringstream ss(str);
		std::string f;
		std::getline(ss,f,',');
		ss.ignore(1);
		std::string dname = f;
		LinkedList a;
		while(std::getline(ss,f,','))
		{
			line qw;
			ss >> qw.quant;
			qw.name=f;
			ss.ignore(2);
			a.insert(qw);
		}	
		recipe.insert(dname, a);
	}
}

void Database::print() const{
	std::cout << "items in storage:\n";
	for(auto i:avalableitems)
		std::cout<<i.first<<" : "<<i.second<<std::endl;
	std::cout << "***********************************************************************************************************\n";
	std::cout << "avalable recipes:\n";
	std::cout << "***********************************************************************************************************\n";
	recipe.show();
}
void Database::Database_to_file(const std::string &file_items, const std::string &file_recipe){
	std::ofstream ofi(file_items);
	std::ofstream ofr(file_recipe);
	for(auto i:avalableitems)
		ofi<<i.first<<", "<<i.second<<std::endl;
	auto t=recipe.root;
	recipe.printt(t,ofr);
	ofi.close();
	ofr.close();
}

void Database::add_recipe(const std::string &item_name, LinkedList recip){
	avalableitems[item_name]=0;
	recipe.insert(item_name, recip);
}

std::map<std::string, int> Database::delete_detail(const std::string &item_name, int kolv){
	auto tmp=recipe.search(item_name);
	std::map<std::string, int> A;
	int h=0;
	if(tmp==NULL){
		A["status"]=-1;
		return A;
	}
	h=avalableitems[item_name]-kolv;
	if(h<0){
		A["status"]=-3;
		return A;
	}
	A["status"]=0;
	avalableitems[item_name]=h;
	return A;
}

void Database::add_detail (const std::string &item_name, int kolv){
auto tmp=recipe.search(item_name);
	if(tmp==NULL){
		LinkedList B;
		add_recipe(item_name,B);
		}
		avalableitems[item_name]+=kolv;	
}

std::map<std::string, int> Database::hm_make(const std::string &item_name){
	auto tmp=recipe.search(item_name);
	auto i=tmp->card.front;
	std::map<std::string, int> A;
	int h=0;
	if(tmp==NULL){
		A["status"]=-1;
		return A;
	}
	if(i!=NULL)
		h=avalableitems[i->data.name]/i->data.quant;
	else {
		A["status"]=-2;
		return A;
	}	
	i=i->next;
	while(i!=NULL){
		h=min(h,avalableitems[i->data.name]/i->data.quant);
		i=i->next;
	}
	auto B=this->can_make(item_name,h+1);
	B["*ans*"]=h;
	return B;
}

std::map<std::string, int> Database::can_make(const std::string &item_name, int kolv){
	auto tmp=recipe.search(item_name);
	std::map<std::string, int> A;
	int h=0;
	A["status"]=0;
	if(tmp==NULL){
		A["status"]=-1;
		return A;
	}
	auto i=tmp->card.front;
	if(i==NULL){
		A["status"]=-2;
		return A;
	}
	while(i!=NULL){		
		h=avalableitems[i->data.name]/i->data.quant;
		if(h<kolv){
			A["status"]=1;
			A[i->data.name]=i->data.quant*kolv-avalableitems[i->data.name];
		}
		i=i->next;
	}
	return A;
}

bool Database::MakeDetail(const std::string &item_name, int kolv){
	if(this->can_make(item_name,kolv)["status"]==0){
		auto i=recipe.search(item_name)->card.front;
		while(i!=NULL){
			avalableitems[i->data.name]-=kolv*i->data.quant;
			i=i->next;
		}
		avalableitems[item_name]+=kolv;
		return true;
	}
	else
		return false;
}

//ещё какято функция

/*int main(){
	Database A("i.txt","r.txt");
	A.Database_to_file("ii.txt","rr.txt");
	line t;
	t.name="beef";
	t.quant=4;
	LinkedList S;
	S.insert(t);
	std::string k="aaaa";
	A.Add_detail(k,12,S);
//	k="aaaaaa";
	A.MakeDetail(k,100);
	A.print();
	std::map<std::string, int> B=A.hm_make(k);
	for(auto i:B)
		if(i.first!="status")
			std::cout<<i.first<<" "<<i.second<<endl;
	std::cout<<B["status"]<<endl;
return 0;
}
*/
