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
//---------------------------------------------------------------------------//
//                         какие-то базовые структуры                        //
//---------------------------------------------------------------------------//
typedef enum {MON, TUE, WED, THU, FRI, SAT, SUN} WeekDay;

struct LessonInfo {
	string teacher_;
	int room_;
	int group_;
	WeekDay day_;
	int time_;
	string subject_;
};

WeekDay GetDayFromString(string s);
string to_string(int num);//дубликат на всякий случай

template<typename T> list<T> IntersectionOfLists(list<T> a, list<T> b);
//----------------------------------------------------------------------------//
//   структуры, представляющие запрос в памяти (``синтаксический анализ'')    //
//----------------------------------------------------------------------------//
typedef enum {SELECT, RESELECT, INSERT, REMOVE, PRINT} CommandType;
typedef enum {TEACHER, ROOM, GROUP, DAY, TIME, SUBJECT, SORT} Field;
typedef enum {GT, LT, EQUAL, IN, NONE} Relation;
/// отношение IN означает попадание в диапазон целых чисел либо соответствие маске str

/// Field=SORT и Relation=NONE -- это костыли, которые нужны, чтобы
/// команда PRINT вписывалась в данную структуру данных

/// так, запросу "PRINT teacher group SORT group END" соотв. Command.conditions=
/// (TEACHER NONE ..) (GROUP NONE ..) (SORT NONE ..) (GROUP NONE ..)
/// [на месте точек -- мусор]

/// а запросу "SELECT teacher=Iv* group=100-499 subject=Analysis END" --
/// (TEACHER IN "Iv*") (GROUP IN (100,499)) (SUBJECT EQUAL "Analysis")

string StringFormOfData(Field field, LessonInfo data);

struct Cond {// Одно условие вида поле + отношение + константа
    Field field;
    Relation relation;
 
 //union не работает, если внутри есть классы;
 //поэтому заменил на struct
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

void SetInfo(LessonInfo& record, Cond cond_data);
//помещает в запись информацию из условия

bool CheckCondition(LessonInfo record, Cond condition);
//проверка, удовлетворяет ли запись условию (нужна для реализации Insert)


typedef list<LessonInfo>::iterator DbIndex;
typedef list<DbIndex> IndicesList;
//----------------------------------------------------------------------------//
//                              сессия                                        //
//----------------------------------------------------------------------------//
class Session {
private:
	IndicesList selection_;
public:
	~Session() {}
	Session() {}
	IndicesList GetSelection() const {return selection_;}
	void SetSelection(IndicesList s) {selection_ = s;}
};


//----------------------------------------------------------------------------//
//                        сама база данных                                    //
//----------------------------------------------------------------------------//
class Database {
private:

	list<LessonInfo> recs_;
	int recs_n_;

	map< string, IndicesList> teachers_;
	map<    int, IndicesList>   groups_;
	map<    int, IndicesList>    rooms_;
	map<WeekDay, IndicesList>     days_;
	map<	int, IndicesList>    times_;
	map< string, IndicesList> subjects_;

	IndicesList SelectByCondition(Cond condition);
	//возвращает список всех записей, удовл. данному условию

	IndicesList SelectByConditionList(SearchConditions conds);
	//возвращает список всех записей, удовл. данным условиям


	string filename_;

	template<typename T> void AddToMapList (map<T,IndicesList>& data, T key, DbIndex index);
	//добавляет элемент index в конец списка data[key]
	//(происходит при добавлении элемента в базу)

	template<typename T> void RemoveFromMapList (map<T,IndicesList>& data, T key, DbIndex index);
	//обратная операция

	void AddRecord(LessonInfo rec);
	void RemoveRecord(DbIndex index);
	void RemoveRecords(SearchConditions conds);
	void SaveToFile(string filename) const;

	int Size() const {return recs_n_;}

	//возвращают число выбранных записей
	int    ImplementSelect  (const SearchConditions& sc, Session& s);
	int    ImplementReselect(const SearchConditions& sc, Session& s);

	//редиректы на соотв. методы для Database
	void   ImplementInsert  (const SearchConditions& sc);
	int    ImplementRemove  (const SearchConditions& sc);

	//возвращает число удалённых записей
	string ImplementPrint   (const SearchConditions& sc, Session& s);

	//перенаправляет на обработчики; формирует ответ на запрос
	string ImplementCommand(const Command& t, Session& s);

	//растаскивание строчки на кусочки
	Command parse(const string& query);

public:
	~Database();
	Database(string filename);

	void SaveToFile() const;
	string HandleQuery(const string& query, Session& s);
};



#endif