#include <iostream>
#include <string>
#include <fstream>
#include "database.h"

using namespace std;

avl::avl(){
	root = NULL;
}

avl::~avl(){
	makeEmpty(root);
}

void avl::makeEmpty(node* t){
	//cout<<"puck"<<endl;
	if (t==NULL){
		return;
	}
	makeEmpty(t->left);
	makeEmpty(t->right);
	delete t;
}

node* avl::insert(string x,LinkedList a, node* t){
	if(t == NULL){
		t=new node;
		t->data=x;
		t->height=0;
		t->left = t->right = NULL;
		t->card = a;	
	}
	else if(x < t->data){
		t->left=insert(x, a, t->left);
		if(height(t->left) - height(t->right) == 2){
			if(x < t->left->data){
				t = srr(t);
			}else{
				t = lrr(t);
			}
		}
	}
	else if(x > t->data){
		t->right = insert(x, a, t->right);
		if(height(t->right) - height(t->left) == 2){
			if(x < t->right->data){
				t = slr(t);
			}else{
				t = llr(t);
			}
		}
	}
	t->height = max(height(t->left),height(t->right))+1;
	return t;
}
node* avl::srr(node* &t){
	if(t->left != NULL){
		node* tmp = t->left;
		t->left = tmp->right;
		tmp->right = t;
		t->height = max(height(t->left),height(t->right))+1;
		tmp->height = max(height(tmp->left),height(t))+1;
		return tmp;
	}
	return t;
}

node* avl::slr(node* &t){
	if(t->right != NULL){
		node* tmp = t->right;
		t->right = tmp->left;
		tmp->left = t;
		t->height = max(height(t->left),height(t->right))+1;
		tmp->height = max(height(t),height(tmp->right))+1;
		return tmp;
	}
	return t;

}

node* avl::llr(node* &t){
	if(t->right != NULL)
		t->right = srr(t->right);
	return slr(t);
}

node* avl::lrr(node* &t){
	if(t->left != NULL)
		t->left = slr(t->left);
	return srr(t);
}

node* avl::findmin(node* t){
	if(t == NULL)
		return NULL;
	else if(t->left == NULL)
		return t;
	else
		return findmin(t->left);
}

node* avl::findmax(node* t){
	if(t == NULL)
		return NULL;
	else if(t->right == NULL)
		return t;
	else
		return findmax(t->right);
}

node* avl::remove(string x, node* t){
	node* tmp;
	if(t == NULL)
		return NULL;
	else if(x<t->data)
		t->left=remove(x, t->left);
	else if(x>t->data)
		t->right=remove(x, t->right);
	else if(t->left && t->right){
		tmp = findmin(t->right);
		t->data = tmp->data;
		t->card = tmp->card;
		t->right = remove(t->data, t->right);
	}else{
		tmp = t;
		if(t->left == NULL)
			t = t->right;
		else if(t->right == NULL)
			t = t->left;
		delete tmp;
	}
	if(t == NULL)
		return t;
	t->height = max(height(t->left),height(t->right))+1;
	if(height(t->left) - height(t->right) == 2){
		if(height(t->left->left) - height(t->left->right) == 1)
			return slr(t);
		else
			return llr(t);
	}else if(height(t->right) - height(t->left) == 2){
		if(height(t->right->right)-height(t->right->left) == 1)
			return srr(t);
		else
			return lrr(t);
	}
	return t;
}

int avl::height(node* t){
	return (t == NULL ? -1 : t->height);
}

void avl::insert(string x, LinkedList a){
	root = insert(x, a, root);
}

void avl::remove(string x){
	root = remove(x,root);
}

void avl::printt(node* t,std::ofstream& off) const{
	if(t == NULL)
		return;
	printt(t->left, off);
	off << t->data << ", ";
	t->card.printListt(off);
	off <<endl;
	printt(t->right, off);
}

void avl::print(node* t) const{
	if(t == NULL)
		return;
	print(t->left);
	std::cout << t->data << ", ";
	t->card.printList();
	std::cout<<endl;
	print(t->right);
}

node* avl::searchh(std::string& item_name,node* t){
	if(t==NULL)
		return NULL;
	if(item_name<t->data)
		return searchh(item_name, t->left);
	else if(item_name>t->data)
		return searchh(item_name, t->right);
	else
		return t;
}

node* avl::search(std::string item_name){
	return searchh(item_name,root);
}

void avl::show() const{
	print(root);
	cout << endl;
}

void avl::copy(avl &A, node* b){
	if(b==NULL)
		return;
	A.insert(b->data,b->card);
	copy(A,b->left);
	copy(A,b->right);
	//A.show();
}

/*int main(){
	LinkedList m[10];
	avl A,B;
	for(int i=0;i<100;++i){
		line q;
		q.name = 'a'+i%17+i%19;
		q.quant = i*i+1;
		m[i/10].insert(q);
	}
	string w="q";
	for(int i=0;i<10;i++){
		char e='a'+i;
		w=w+"q";
		A.insert(w,m[i]);
	}
	w="";
	for(int i=0;i<10;i++){
		w=w+"qq";
		A.remove(w);
	}
	B=A;
	B.show();
	return 0;
}*/
