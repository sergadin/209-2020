#ifndef SERV_PART_H
#define SERV_PART_H


#include<algorithm>
#include<vector>
#include<list>
#include<map>
#include<string>
#include<cstdlib>
#include<iostream>
//#include<cassert>

using namespace std;
//--------------------------------------------------------------------------//
//                        какие-то базовые структуры                        //
//--------------------------------------------------------------------------//
typedef enum {MON, TUE, WEN, THU, FRI, SAT, SUN} WeekDay;

struct LessonInfo {
	string teacher_;
	int room_;
	int group_;
	WeekDay day_;
	int time_;
	string subject_;
};
typedef list<LessonInfo*> IndicesList;

WeekDay GetDayFromString(string s);
//---------------------------------------------------------------------------//
//  структуры, представляющие запрос в памяти (``синтаксический анализ'')    //
//---------------------------------------------------------------------------//
typedef enum {SELECT, RESELECT, INSERT, REMOVE, PRINT} CommandType;
typedef enum {TEACHER, ROOM, GROUP, DAY, TIME, SUBJECT, SORT} Field;
typedef enum {GT, LT, EQUAL, IN, NONE} Relation;
/// отношение IN означает попадание в диапазон целых чисел либо соответствие маске str

/// Field=SORT и Relation=NONE -- это костыли, которые нужны, чтобы
/// команда PRINT вписывалась в данную структуру данных

/// так, запросу "PRINT teacher group SORT group END" соответствует Command.conditions=
/// (TEACHER NONE ..) (GROUP NONE ..) (SORT NONE ..) (GROUP NONE ..)
/// [на месте точек -- мусор]

/// а запросу "SELECT teacher=Iv* group=100-499 subject=Analysis END" --
/// (TEACHER IN "Iv*") (GROUP IN (100,499)) (SUBJECT EQUAL "Analysis")

struct Cond {// Одно условие вида поле + отношение + константа
    Field field;
    Relation relation;
 //   union {
 	struct {
      int number;//одно значение
      pair<int,int> diap;//диапазон значений
      string str;
    } value; 
};

typedef list<Cond> SearchConditions;
struct Command {
    CommandType cmd;
    SearchConditions conditions;
};

void SetInfo(LessonInfo& record, Cond cond_data);//помещает в запись информацию из условия
bool CheckCondition(LessonInfo record, Cond condition);//проверка, удовлетворяет ли запись условию
//---------------------------------------------------------------------------//
//                       сама база данных                                    //
//---------------------------------------------------------------------------//
class Database {
	vector<LessonInfo> recs_;
	int recs_n_;

	map< string, IndicesList> teachers_;
	map<    int, IndicesList>   groups_;
	map<    int, IndicesList>    rooms_;
	map<WeekDay, IndicesList>     days_;
	map<	int, IndicesList>    times_;
	map< string, IndicesList> subjects_;
public:
	~Database();
	Database();
	Database(string filename);//конструктор из файла (запускает LoadFromFile)
	
	Database(const Database& other, const SearchConditions criteria);
	//этот конструктор и осуществляет выборку из other записей, удовл. критериям

	int DbSize() const;
	void AddRecord(LessonInfo rec);
	void RemoveRecords(const SearchConditions criteria);
	void SaveToFile(string filename) const;
};
/*должен существовать один глобальный экземпляр класса Database,
  и ещё по одному -- для каждого пользователя.
*/
Database __ALL_DATA;

//---------------------------------------------------------------------------//
//                             пользовательское                              //
//---------------------------------------------------------------------------//
class Session {
private:
	Database* selection_;//так проще заменять на новую
// + что-то вроде айди (?)
public:
	~Session();
	Session();
	int DoSelect  (const SearchConditions& sc);
	int DoReselect(const SearchConditions& sc);
};

//---------------------------------------------------------------------------//
//                       парсер и т.д.                                       //
//---------------------------------------------------------------------------//

//эти две команды работают с данными сессии (от них, возможно, избавлюсь)
int    ImplementSelect  (const SearchConditions& sc, Session& s);//возвращает число выбранных записей
int    ImplementReselect(const SearchConditions& sc, Session& s);

//эти - с глобальной информацией (поэтому ссылку на базу данных можно и убрать (?))
void   ImplementInsert  (const SearchConditions& sc,       Database& db);
int    ImplementRemove  (const SearchConditions& sc,       Database& db);//возвращает число удалённых записей
string ImplementPrint   (const SearchConditions& sc, const Database& db);//возвращает напечатанную таблицу

//switch (перенаправляет на обработчики, см.выше)
string ImplementCommand(const Command& t, Session& s);

//растаскивание строчки на кусочки
Command parse(const string& query);

//вот эта функция работает как чёрный ящик и доступна извне сервера
string MainQueryHandler(const string& query, Session& s);

#endif