#include<iostream>
#include<map>
#include <string>
#include <vector>
typedef enum {SELECT, RESELECT, INSERT, UPDATE, DELETE, PRINT} CommandType;
typedef enum { CLASSROOM, SUBJECT, TEACHER, DAY_TIME, GROUP } Field;

class exeption{
    struct DataError();
    struct SyntaxError();
};

class Query{
    Query(const string &q);
    ~Query();
};

class Database{
	public:
    Database(const string &filename);
    ~Database();
    void save();
    Result process(const string &q, session s);
};

struct Timetable {
    std::string subject;
    std::string teacher;
    std::string day_time;
    unsigned int classroom;
    unsigned int group;
};


class condition
{
Field field_;
Relation rel_;
Value val_;
};

class Session
{
    Session();
    ~Session();
};

class Result
{
    Result();
    ~Result();
};
