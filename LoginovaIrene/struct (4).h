
#ifndef REC_H
#define REC_H

#include <memory>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define HASH 6


class student
{
	private:
		char name[64];
		double rating = 0;
		
		int group = 0;
		//char * info;
	public:
        student( const char * my_name="",double rt=0,int gr=0){ rating=rt; group=gr; strcpy(name, my_name);}
		~student(){	name[0]=0; rating = 0; group = 0;}
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
		
		double get_rt(){	return rating;}
		list_node * get_next(){	return next;}
		student * get_data(){	return data;}
    friend class list;
};

class list
{
    private:
        list_node * root;
        list_node * curr;
    public:
		list(){ root = 0; curr = 0;}
        list(student * prime){   root = new list_node( prime); curr = 0;}
        list( list_node * head){   root = head;    curr = 0;}
        ~list(){   del();  root = 0;  curr = 0; }
        int add( student * prime);
		int del_it(student * prime);
        void print( FILE * stream = stdout );
        void del(){   root->del(); root = 0;}
		
		int is_empty(){return (root == 0);}
		
		list_node * get_root(){	return root;}
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
		
		tree_node * get_left(){ return left;}
		tree_node * get_right(){ return right;}
		char * get_name(){ return name;}
		student * get_data(){ return data;}
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
		
		int is_empty(){return (root == 0);}
		
		tree_node * get_root(){ return root;}
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
		
		tree * get_tree(){	return my_tree;}
		list * get_list(){	return my_list;}
		int get_group(){ return group;}
		hash_node * get_next(){ return next;}
    friend class hash;
};

class hash
{
	private:
        hash_node *root[HASH];
		hash_node * curr;
		int hash_fun( student * prime ){	return (prime->get_gr() / 100) - 1;}
		int hash_fun( int group ){			return (group / 100) - 1;}
    public:
		hash(){curr = 0;}
        hash(student * prime){   root[ hash_fun(prime) ] = new hash_node( prime); curr = 0;}
        ~hash(){   del();  for(int i=0; i<HASH; i++)	root[i] = 0;  curr = 0; }
		int fill(int n, FILE * in);
        int add(student * prime);
		int del_it(student * prime);
        void print( FILE * stream = stdout );
        void del(){   for(int i=0; i<HASH; i++)	root[i]->del();}
		
		int get_index( int group){	return hash_fun(group);}
		hash_node * get_elem(int index){	return root[index];}
};


enum cmd_type
{   CMD_NONE, DELETE, RESELECT, SELECT, UPDATE};

enum cond_type
{   COND_NONE, EQ, NE, LT, GT, LE, GE, SET};

enum oper_type
{   OPER_NONE, OR, AND };


class comand
{
	public:
		cmd_type    type;
		cond_type   c_name;
		cond_type   c_rat;
		cond_type   c_gr;
		oper_type   oper;
		char name[64];
		double rating = 0;
		int group = 0;
	public:
		comand( const char * my_name="",double rt=0,int gr=0){ rating=rt; group=gr; strcpy(name, my_name);}
		~comand(){}
		int analyze();
		int analyze_upd(comand * cmd);
		
		char *get_name(){ return name;}
		double get_rt(){	return rating;}
		int get_gr(){	return group;}
		
		cond_type find_cond(char *);
		void print(FILE * stream = stdout);
		void clear(){	type = CMD_NONE; c_name = c_rat = c_gr = COND_NONE; oper = OPER_NONE, strcpy(name,""); rating=0; group=0;}
};

class sub_sess
{
    private:
        sub_sess * next = 0;
        student * data = 0;
    public:
        sub_sess(student * prime){   data = prime;}
        ~sub_sess(){ data = 0;}
        void del(){ if(next)    next->del(); next=0;}
		
		sub_sess * get_next(){	return next;}
		student * get_data(){	return data;}
    friend class Session;
};

class Session
{
	private:
		int usr = 0;
		sub_sess * root;
		sub_sess * curr;
	public:
		Session(){ root = 0; usr = 0; curr = 0;}
		~Session(){	usr = 0; root = 0; curr = 0;}
		int add( student * prime);
		int del_it(student * elem);
		void print(FILE * stream = stdout);
		void eraze(){	if(root)	root->del(); root = 0; curr = 0;}
		
		int inside(student * elem);
		
		
		sub_sess * get_root(){	return root;}
		void set_root(sub_sess * r__t){	root = r__t;}
		void set_next(sub_sess * prev, sub_sess * next){	prev->next = next;}
};


class Database
{
	private:
		hash * my_hash;
		Session * sess;
	public:
		Database(hash * base = 0){ my_hash = base; sess = new Session();}
		~Database(){ my_hash = 0; delete sess; }
		void base_print(FILE * stream = stdout){ my_hash->print(stream);}
		void sess_print(FILE * stream = stdout){ sess->print(stream);}
		int add(student * prime){	my_hash->add(prime);}
		void clear_sess(){	sess->eraze();}
		void do_select(comand * cmd);
		void sub_select_group(hash_node * head, comand * cmd);
		void sub_select_tree(tree * head, comand * cmd);
		void sub_select_subtree(tree_node * node, comand * cmd);
		void sel_rat(student * prime, comand * cmd);
		void sub_select_list(list * head, comand * cmd);
		
		void reselect(comand * cmd);
};






#endif