#include <iostream>
#include <string>
#include <vector>
using namespace std;
typedef enum {SELECT, RESELECT, INSERT, UPDATE, DELETE, PRINT} CommandType;
typedef enum {author, title, publisher, genre, theme} Field;
class Query{
	private:
		CommandType command_; 
		vector<Condition>* condition_:
		vector<Field>* fields_; //for print
	public:
		Query(const string &qu); //if command != print -> fields = NULL
};

class Condition{
	private:
		Field field;
		string relation_;
		string comment_; //value-?
	public:	
		Condition(const string &str);
};

class DataBase{
	private:
		Library *library_;
	public:
		DataBase(const string &filename);
		process(Session user)
		bool db_select(Session *user, Query *query);
		bool db_reselect(Session *user, Query *query);
		bool db_insert(Session *user, Query *query);
		bool db_update(Session *user, Query *query);
		bool db_delete(Session *user, Query *query);
		bool db_print(Session *user, Query *query);
};

class Session{
	private:
		string query_;
		Library* result;
	public:
		Session();
}

class Book {
        private:
                char author_[64];
                char title_[124];
                char publisher_[64];
                int class_1;
                int class_2;
                bool presence_;
        public:
                Book(FILE *file);
                bool compare(const Book &book);



class Library {
        private:
                vector<Book*> library_;
        public:
        		Library() {}
                Library(FILE* fin);
                void add_book(const Book *book)
                {
                        library_.push_back(book);
                        library_.sort(0, library_.size() - 1);
                }
                void sort(Field field, int first, int last)
                {
                        int i = first, j = last;
                        Book *tmp, *mid = (library_[(i + j) / 2])->field;
                        if(!(library_.emty()))
                        {
                                while(i <= j)
                                {
                                        while((library_[i])->field < mid)
                                                i++;
                                        while((library_[j])->field > mid)
                                                j--;
                                        if(i < j)
                                        {
                                                tmp = library_[i];
                                                library_[i] = library_[j];
                                                library_[j] = tmp;
                                                i++;
                                                j--;
                                        }
                                }
                                if(first < j)
                                        library_.sort(field, first, j);
                                if(last > i)
                                        library_.sort(field, i, last);
                        }
				}
				bool empty_library();
				void del_book();
				void change(Field field, string value);
				
};





class DB_Exception {
	private:
		int code_;
	    std::string messege_;
	public:
		DB_Exception(int c, std::string c)
		{
			code_ = c;
			messege_ = s;
		}
};


































