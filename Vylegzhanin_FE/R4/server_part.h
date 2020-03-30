#include<algorithm>
#include<vector>
#include<list>
#include<map>
#include<string>
#include<cstdlib>
#include<iostream>
//#include<cassert>

using namespace std;
//--------------------------------------------------------------------------
//                        какие-то базовые структуры
//--------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
//                 структуры, представляющие запрос в памяти
//---------------------------------------------------------------------------
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
    union {
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
//---------------------------------------------------------------------------
//                       сама база данных
//---------------------------------------------------------------------------
class Database {
	vector<LessonInfo> recs_;
	map<    string, IndicesList> teachers_;
	map<       int, IndicesList>   groups_;
	map<       int, IndicesList>    rooms_;
	map<DateTime_t, IndicesList>    times_;
	map<    string, IndicesList> subjects_;
public:
	~Database();
	Database();
	Database(string filename);//конструктор из файла (запускает LoadFromFile)
	
	Database(const Database& other, SearchConditions criteria);
	//этот конструктор и осуществляет выборку из other записей, удовл. критериям

	Database& operator= (const Database& other);

	int DbSize() const;
	void AddRecord(LessonInfo rec);
	void RemoveRecords(SearchConditions criteria);
//	void LoadFromFile(string filename);
	void SaveToFile(string filename) const;
};
//должны существовать два экземпляра этого класса:
//ALL_DATA и SELECTED_DATA.

//---------------------------------------------------------------------------
//                       парсер и т.д.
//---------------------------------------------------------------------------
int    ImplementSelect  (SearchConditions& sc);//возвращает число выбранных записей
int    ImplementReselect(SearchConditions& sc);
void   ImplementInsert  (SearchConditions& sc);
int    ImplementRemove  (SearchConditions& sc);//возвращает число удалённых записей
string ImplementPrint   (SearchConditions& sc);//возвращает напечатанную таблицу

string ImplementCommand(const Command& t);//перенаправляет на настоящие обработчики

Command parse(const string& query);
//растаскивание строчки на кусочки

string MainQueryHandler(const string& query);
//вот эта функция работает как чёрный ящик и доступна извне сервера