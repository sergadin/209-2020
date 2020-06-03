#include<vector>
#include<iostream>
#include<string>
#include<map>
#include<sstream>
#include <fstream>
#include <cstdlib>

#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

struct line //The device of parts along with its name
{
  std::string name;
  int quant;
  line& operator=(const line& rhs)
  {
      name = rhs.name;
      quant = rhs.quant;
      return *this;
  }
  bool  operator==(const line &rhs)
  {
      return name == rhs.name;
  }
};

class LinkedList
{
public:
	LinkedList(); //default constructor
	LinkedList(const LinkedList& ls);//copy constructor
	~LinkedList(); //destructor
	LinkedList& operator=(const LinkedList&); //assignment operator
	bool insert(const line&);
	bool remove(const line&);
  int removepart(const line&);
	bool search(const line&) const;
  int searchP(const line&) const; // Takes into account the number of parts returns how much is missing
	std::vector<line> get() const;
  void printListt(std::ofstream& off) const;
  void printList() const;

private:
	class Node
	{
	public:
		line data;
		Node* next;
		Node(line s)
    {
        data = s;
        next = NULL;
    }
		Node(line s,Node* nd)
    {
        data = s;
        next = nd;
    }
	};
	Node* front;
	void deepCopy(const LinkedList& ls);
	void deleteList();
  friend class avl;
  friend class Database;
  friend std::ostream& operator<< (std::ostream &out, const LinkedList &point);
};

#endif

#ifndef avl_H_INCLUDED
#define avl_H_INCLUDED

struct node{
	node* left;
	node* right;
	std::string data;
	int height;
	LinkedList card;
};
class avl{
	friend class Database;
	node* root;
	public:
	avl();
	~avl();
	void makeEmpty(node* t);
	node* insert(std::string x,LinkedList a, node* t);
	node* srr(node* &t);
	node* slr(node* &t);
	node* llr(node* &t);
	node* lrr(node* &t);
	node* findmin(node* t);
	node* findmax(node* t);
	node* remove(std::string x, node* t);
	node* searchh(std::string& item_name, node*t);
	node* search(std::string item_name);
	void copy(avl &A, node* b);
	avl& operator=(const avl &x){makeEmpty(root);copy(*this,x.root);return *this;}
	int height(node* t);
	void insert(std::string x, LinkedList a);
	void remove(std::string x);
	void print(node* t) const;
	void printt(node* t, std::ofstream& off) const;
	void show() const;

};
#endif

#ifndef database_included
#define database_included
class Database{
	avl recipe;
	std::map<std::string,int> avalableitems;
public:
	Database();// default constructor
	Database(const std::string &file_items, const std::string &file_recipe);//get data from files
	void print() const;
	void Database_to_file(const std::string &file_items, const std::string &file_recipe);
	void add_recipe(const std::string &item_name, LinkedList recip);
	void add_detail(const std::string &item_name, int kolv);
	void free(){delete [] recipe.root;}
	std::map<std::string, int> delete_detail(const std::string &item_name, int kolv=0);
	std::map<std::string, int> can_make(const std::string &item_name, int kolv);
	std::map<std::string, int> hm_make(const std::string &item_name);
	bool MakeDetail(const std::string &item_name, int kolv=1);
	//ещё какято функция
};

#endif
