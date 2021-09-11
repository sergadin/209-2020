
#ifndef REC_H
#define REC_H

#include <memory>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define HASH 6
#define Num_Client 10


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
		void write(int sock);
		
        char *get_name(){ return name;}
		double get_rt(){	return rating;}
		int get_gr(){	return group;}
		
		void set_name(char * my_name){	strcpy(name, my_name);}
		void set_group(int gr){	group = gr;}
		void set_rating(int rt){	rating = rt;}
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
		void write(int sock);
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
		void rec_write( tree_node * node, int deg, int sock);
		tree_node * parent_ret(tree_node * node);
    public:
        tree(student * prime ){   root = new tree_node( prime ); curr = 0;}
        tree( tree_node * head){   root = head;    curr = 0;}
        ~tree(){   del();  root = 0;  curr = 0; }
        int init(student * prime ){   root = new tree_node( prime); curr = 0;}
        int add( student * prime );
        int del_it( student * prime );
        void print ( FILE * stream = stdout);
		void write(int sock);
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
		void write(int sock);
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

class cond_for_name
{
	private:
		cond_for_name * next;
		cond_type c_name;
		char name[64];
	public:
		cond_for_name(){}
		cond_for_name(cond_type C, char * my_name){ c_name = C; strcpy(name, my_name); next = 0;}
		~cond_for_name(){ next = 0; c_name = COND_NONE;}
		
		void clear(){	if(next)	next->clear(); next = 0;}
		char *get_name(){ return name;}
		cond_type get_cond(){	return c_name;}
		cond_for_name * get_next(){	return next;}
	friend class comand;
};

class cond_for_rat
{
	private:
		cond_for_rat * next;
		cond_type c_rat;
		double rating = 0;
	public:
		cond_for_rat(){}
		cond_for_rat(cond_type C, double rat){ c_rat = C; rating = rat; next = 0;}
		~cond_for_rat(){ next = 0; c_rat = COND_NONE;}
		
		void clear(){	if(next)	next->clear(); next = 0;}
		double get_rt(){	return rating;}
		cond_type get_cond(){	return c_rat;}
		cond_for_rat * get_next(){	return next;}
	friend class comand;
};

class cond_for_gr
{
	private:
		cond_for_gr * next;
		cond_type c_gr;
		int group = 0;
	public:
		cond_for_gr(){}
		cond_for_gr(cond_type C, int gr){ c_gr = C; group = gr; next = 0;}
		~cond_for_gr(){ next = 0; c_gr = COND_NONE;}
		
		void clear(){	if(next)	next->clear(); next = 0;}
		int get_gr(){	return group;}
		cond_type get_cond(){	return c_gr;}
		cond_for_gr * get_next(){	return next;}
	friend class comand;
};


class comand
{
	public:
		cmd_type    type;
		cond_for_name * name_root;
		cond_for_rat  * rat_root;
		cond_for_gr   * gr_root;
	public:
		comand(){ name_root = 0; rat_root = 0; gr_root = 0;}
		~comand(){ name_root = 0; rat_root = 0; gr_root = 0;}
		int analyze(char * com);
		int analyze_upd();
		cond_type find_cond(char *);
		
		int gr_sort();
		int rat_sort();
		int name_sort();
		
		void print(FILE * stream = stdout);
		void clear();
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
		void write( int sock );
		void eraze(){	if(root)	root->del(); root = 0; curr = 0;}
		
		int inside(student * elem);
		
		int get_size(){	curr = root; int size = 0; while(curr){	size ++; curr = curr->next;} return size;}
		
		sub_sess * get_root(){	return root;}
		void set_root(sub_sess * r__t){	root = r__t;}
		void set_next(sub_sess * prev, sub_sess * next){	prev->next = next;}
};


class Database
{
	private:
		hash * my_hash;
		Session * sess[Num_Client];
		int sock = 0;
	public:
		Database(hash * base = 0){ my_hash = base; for(int i=0; i<Num_Client; i++)	sess[i] = new Session();}
		~Database(){ my_hash = 0; }
		void set_socket( int i){ sock = i;} 
		void base_print(FILE * stream = stdout){ my_hash->print(stream);}
		void base_write(){ my_hash->write(sock);}
		
		void sess_print(FILE * stream = stdout){ sess[0]->print(stream);}
		void sess_write(){ sess[sock]->write(sock);}
		
		int add(student * prime){	my_hash->add(prime);}
		void clear_sess(){	sess[sock]->eraze();}
		void do_select(comand * cmd);
		
		int sess_size(){ return sess[sock]->get_size();}
		
		void sub_select_group(hash_node * head, comand * cmd);
		void sub_select_tree(tree * head, comand * cmd);
		void sub_select_subtree(tree_node * node, comand * cmd);
		
		void sub_select_list(list * head, comand * cmd);
		
		void rec_sel_tree(tree_node * curr, cond_for_name * n_eq, char * min, int smn, char * max, int smx, comand * cmd);
		void reselect(comand * cmd);
		void update(comand * cmd);
		
		int sel_gr(student * prime, comand * cmd);
		int sel_rat(student * prime, comand * cmd);
		int sel_name(student * prime, comand * cmd);
};






#endif
