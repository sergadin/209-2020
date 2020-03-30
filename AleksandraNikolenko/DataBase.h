#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Query{
	private:
		vector<Condition> query_:
	public:
		Query(const string &qu);
		Library* realization(const Query &qu);
};

class Condition{
	private:
		string command_; //command or not?
		vector<Criterion> criterion_;
	public:	
		Condition(const string &str);
		void realization(const Condition &cond)
		{
			if(cond->command = "SELECT") 
				cond->select_();
			if(cond->command = "RESELECT")
				cond->reselect_();
			if(cond->command = "INSERT")
				cond->insert_();
			if(cond->command = "UPDATE")
				cond->update_();
			if(cond->command = "DELETE")
				cond->delete_();
			if(cond->command = "PRINT")
				cond->print_();
		}
};

class Criterion{
	private:
		string field_;
		string relation_;
		string comment_;
	public:
		Criterion(const string &str); //correct?
};


class DataBase{
	private:
		Library *library_;
	public:
		DataBase(const string &filename);
};

class Session{
	private:
		int code_;
		Library* result_select_;
		Library* result;
	public:
		
}

class Book {
        private:
                char author_[64];
                char title_[124];
                char publisher_[64];
                int class_1;
                int class_2;
                int class_3;
        public:
                Book(FILE *file);
};


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
                void sort(int first, int last)
                {
                        int i = first, j = last;
                        Book *tmp, *mid = library_[(i + j) / 2];
                        if(!(library_.emty()))
                        {
                                while(i <= j)
                                {
                                        while(library_[i] < mid)
                                                i++;
                                        while(library_[j] > mid)
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
                                        library_.sort(first, j);
                                if(last > i)
                                        library_.sort(i, last);
                        }
				}
				bool empty_library();
				
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


































