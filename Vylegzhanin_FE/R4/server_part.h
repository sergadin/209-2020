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

//----------------------------------------------------------------------------//
//                        сама база данных                                    //
//----------------------------------------------------------------------------//

//typedef list<vector<LessonInfo>::iterator> IndicesList;
//это кажется более безопасным, но
//при изменении списка итераторы ломаются (возможно)
//было бы проще с list вместо vector

typedef list<LessonInfo*> IndicesList;


template<typename T> void AddToMapList (map<T,IndicesList>& data, T key, LessonInfo value);
//добавляет элемент value в конец списка data[key]
//(происходит при добавлении элемента в базу)

template<typename T> void RemoveFromMapList (map<T,IndicesList>& data, T key, LessonInfo* value);
//обратная операция

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
	
	Database(const Database& other, const SearchConditions conds);
	//создаёт новую базу -- выборку из записей, удовл. данным условиям

	IndicesList SelectByCondition(Cond condition) const;
	//возвращает список всех записей, удовл. данному условию

	IndicesList SelectByConditionList(const SearchConditions conds) const;
	//возвращает список всех записей, удовл. данным условиям

	void AddRecord(LessonInfo rec);
	void RemoveRecord(LessonInfo* prec);
	void RemoveRecords(const SearchConditions conds);
	void SaveToFile(string filename) const;

	int DbSize() const {return recs_n_;}
};
/*должен существовать один глобальный экземпляр класса Database,
  и ещё по одному -- для каждого пользователя.
*/
//сделать класс для глобального с автом. записью/загрузкой?
//пока что аналоги конструктора/деструктора:
void SetupGlobalDatabase();
void ShutdownGlobalDatabase();

//----------------------------------------------------------------------------//
//                              пользовательское                              //
//----------------------------------------------------------------------------//
class Session {
private:
	Database selection_;
// возможно, что-то ещё добавится
public:
	~Session();
	Session();
	int DoSelect  (const SearchConditions& sc);
	int DoReselect(const SearchConditions& sc);
};

//----------------------------------------------------------------------------//
//                        парсер и т.д.                                       //
//----------------------------------------------------------------------------//

//редиректы на методы класса Session
int    ImplementSelect  (const SearchConditions& sc, Session& s);
//возвращает число выбранных записей
int    ImplementReselect(const SearchConditions& sc, Session& s);

//редиректы на методы класса Database экземпляра __ALL_DATA
void   ImplementInsert  (const SearchConditions& sc);
int    ImplementRemove  (const SearchConditions& sc);
//возвращает число удалённых записей
string ImplementPrint   (const SearchConditions& sc, const Database& db);
//возвращает напечатанную таблицу;
//печать табличек из сессии может пригодиться для дебага

//перенаправляет на обработчики; формирует ответ на запрос
string ImplementCommand(const Command& t, Session& s);

//растаскивание строчки на кусочки
Command parse(const string& query);

//вот эта функция работает как чёрный ящик и доступна извне сервера
string MainQueryHandler(const string& query, Session& s);

#endif