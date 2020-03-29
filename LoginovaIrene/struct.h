
#ifndef REC_H
#define REC_H

#include <memory>
#include<stdio.h>
#include<string.h>
#include<time.h>

#define HASH 6

class student
{
	private:
		char name[64];
		double rating = 0;
		
		int group = 0;
		//char * info;
	public:
        student( const char * buff=0,double rt=0,int gr=0){ rating=rt; group=gr; strcpy(name, buff);}
		~student(){	name[0]=0; rating = 0; group = 0;}

		int init(const char * buff, double rt, int gr);
		void print(FILE * fp = stdout);
        char *get_name(){ return name;}
		double get_rt(){	return rating;}
		int get_gr(){	return group;}
		student &operator = (student &&x);
};

class list_node
{
    private:
        double rating = 0;
        list_node * next = 0;
        student * data = 0;
    public:
        list_node(student * prime){   data = prime; rating = prime->get_rt();}
        ~list_node(){ data = 0;	rating = 0;}
        void del(){ if(next)    next->del(); next=0; rating = 0;}
    friend class list;
};

class list
{
    private:
        list_node * root;
        list_node * curr;
    public:
        list(student * prime){   root = new list_node( prime); curr = 0;}
        list( list_node * head){   root = head;    curr = 0;}
        ~list(){   del();  root = 0;  curr = 0; }
        int init(student * prime){   root = new list_node( prime ); curr = 0;}
        int add( student * prime);
		int del_it(student * prime);
        void print( FILE * stream = stdout );
        void del(){   root->del(); root = 0;}
	friend class hash;
};

class tree_node
{
    private:
        char *name;
        tree_node * left = 0;
        tree_node * right = 0;
        student * data = 0;
    public:
        tree_node(student * prime = 0){   data = prime; name = prime->get_name();}
        ~tree_node(){ data = 0; name=0; }
        void del(){ if(left)    left->del(); left=0;  if(right)    right->del(); right=0;	name = 0; data = 0;}
    friend class tree;
};

class tree
{
    private:
        tree_node * root;
        tree_node * curr;
		void rec_print( tree_node * node, int deg,  FILE * stream);
		tree_node * parent_ret(tree_node * node);
    public:
        tree(student * prime ){   root = new tree_node( prime ); curr = 0;}
        tree( tree_node * head){   root = head;    curr = 0;}
        ~tree(){   del();  root = 0;  curr = 0; }
        int init(student * prime ){   root = new tree_node( prime); curr = 0;}
        int add( student * prime );
        int del_it( student * prime );
        void print ( FILE * stream = stdout);
        void del(){   root->del(); root = 0;}
	friend class hash;
};


class hash_node
{
    private:
        int group = 0;
        hash_node * next;
        tree * my_tree = 0;
        list * my_list = 0;
    public:
        hash_node(student * prime=0){    my_tree = new tree(prime); my_list = new list(prime);	group = prime->get_gr(); next = 0;}
        ~hash_node(){   group = 0;	next = 0;}
		void del(){ if(next)    next->del(); next=0; group = 0;}
    friend class hash;
};

class hash
{
    private:
        hash_node *root[HASH];
		hash_node * curr;
		int hash_fun( student * prime ){	return (prime->get_gr() / 100) - 1;}
    public:
        hash(student * prime){   root[ hash_fun(prime) ] = new hash_node( prime); curr = 0;}
        ~hash(){   del();  for(int i=0; i<HASH; i++)	root[i] = 0;  curr = 0; }
        int init(student * prime){  root[ hash_fun(prime) ] = new hash_node( prime); curr = 0;}
        int add(student * prime);
		int del_it(student * prime);
        void print( FILE * stream = stdout );
        void del(){   for(int i=0; i<HASH; i++)	root[i]->del();}
//	friend class Data_base;
};

enum cmd_type
{   CMD_NONE, RESELECT, SELECT};

enum cond_type
{   COND_NONE, EQ, NE, LT, GT, LE, GE};

enum oper_type
{   OPER_NONE, OR, AND };


class comand: public student
{
	private:
		cmd_type    type;
		cond_type   c_name;
		cond_type   c_rat;
		cond_type   c_gr;
		oper_type   oper;
		char sur_name[15];
	public:
		comand( const char * buff=0,double rt=0,int gr=0):student(buff, rt, gr){}
		~comand(){}
		int parse(char * );
		cond_type sur_parse();
};


class Data_base
{
	private:
		hash * my_hash;
		comand * curr;
	public:
		Data_base(){ my_hash = 0;}
		~Data_base(){ my_hash = 0;}
		int run(int argc, char **argv);
		int fill( int n, FILE * in);
};



#endif
