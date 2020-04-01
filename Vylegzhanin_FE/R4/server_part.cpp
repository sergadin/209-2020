#include "server_part.h"
#include <sstream>

const string DayNumbers[] = {"MON", "TUE", "WEN", "THU", "FRI", "SAT", "SUN"};
Database __ALL_DATA;
const string __ALL_DATA_FILENAME("__ALL_DATA.dbase");

//--------------------------------------------------------------------------
//                        какие-то базовые структуры
//--------------------------------------------------------------------------
WeekDay GetDayFromString(string s) {
	int pos = -1;
	for(int i = 0; i < 7; i++) {
		if(DayNumbers[i] == s) {
			pos = i;
			break;
		}
	}

	switch(pos){
		case 0: return MON;
		case 1: return TUE;
		case 2: return WED;
		case 3: return THU;
		case 4: return FRI;
		case 5: return SAT;
		case 6: return SUN;
		default: /*EXCEPTION */ break;
	}
}
//---------------------------------------------------------------------------
//                 структуры, представляющие запрос в памяти
//---------------------------------------------------------------------------
void SetInfo(LessonInfo& record, Cond cond_data) {//помещает в запись информацию из условия
//возможно, нужна проверка, что cond_data.relation = EQUAL
//или это ограничит функционал?
	switch(cond_data.field) {
		case TEACHER: record.teacher_ = cond_data.value.str;    break;
		case    ROOM: record.room_    = cond_data.value.number; break;
		case   GROUP: record.group_   = cond_data.value.number; break;
		case     DAY: record.day_     = GetDayFromString(cond_data.value.str); break;
		case    TIME: record.time_    = cond_data.value.number; break;
		case SUBJECT: record.subject_ = cond_data.value.str;    break;

		case SORT: default: /*EXCEPTION: BAD FIELD*/ break;
	}
}

bool CheckCondition(LessonInfo record, Cond condition) {//проверка, удовлетворяет ли запись условию
	int num;
	string str;

	switch(condition.field) {
		case TEACHER: str = record.teacher_; break;
		case SUBJECT: str = record.subject_; break;

		case    ROOM: num = record.room_; break;
		case   GROUP: num = record.group_; break;
		case    TIME: num = record.time_; break;
		case     DAY: num = record.day_; break;//приведение Weekday(enum) к типу int
		default:break;
	}

	switch(condition.field) {
		//type = string
		case TEACHER:
		case SUBJECT:
		switch(condition.relation) {
			case GT:
				return (str > condition.value.str);
			case LT:
				return (str < condition.value.str);
			case EQUAL:
				return (str == condition.value.str);
			case IN:
				/*проверка, подходит ли str под маску condition.value.str*/

			default:
			/*EXCEPTION: BAD RELATION OF STRINGS*/
			break;
		}
		break;

		//type=int
		case  ROOM:
		case GROUP:
		case  TIME:
		case   DAY:
		switch(condition.relation) {
			case GT:
				return (num > condition.value.number);
			case LT:
				return (num < condition.value.number);
			case EQUAL:
				return (num == condition.value.number);
			case IN:
				return (num >= condition.value.diap.first &&
						num <= condition.value.diap.second);

			default:
			/*EXCEPTION: BAD RELATION OF INTEGERS*/
			break;
		}
		break;


		default: /*это значит не разобран случай*/ break;
	}
}

//---------------------------------------------------------------------------
//                       сама база данных
//---------------------------------------------------------------------------
Database::~Database() {
	//кажется, пока здесь ничего не должно быть
}

Database::Database() {
	//кажется, пока здесь ничего не должно быть
	recs_n_ = 0;
}

Database::Database(string filename) {
	//импорт из файла...
}

Database::Database(const Database& other, const SearchConditions conds) {
	IndicesList indlist = other.SelectByConditionList(conds);
	for(IndicesList::iterator it = indlist.begin(); it != indlist.end(); it++) {
		AddRecord(**it);
	}
}


IndicesList Database::SelectByCondition(Cond condition) const {
//здесь должно быть нечто объёмное
//отчасти похоже на checkcondition(), но сложнее
//несколько switch и т.д.
}

IndicesList Database::SelectByConditionList(const SearchConditions cond_list) const {
	IndicesList res;
	for(SearchConditions::const_iterator it  = cond_list.begin();
										 it != cond_list.end();
										 it++) {
		IndicesList other = SelectByCondition(*it);

		IndicesList temp;//пересекаем res с выборкой по новому условию
		set_intersection(res.begin(), res.end(),
						 other.begin(), other.end(),
						 temp.begin());
		res = temp;
	}

	return res;
}

template<typename T> void AddToMapList(map<T,IndicesList>& data, T key, LessonInfo* value) {
	if(data.count(key) == 0) {
		data[key] = IndicesList(1,value);
	}
	else data[key].push_back(value);
}

template<typename T> void RemoveFromMapList (map<T,IndicesList>& data, T key, LessonInfo* value) {
	if(data.count(key) == 0) {
		/*EXCEPTION: NOTHING IS REMOVED*/
	}
	IndicesList& indlist = data[key];
	for(IndicesList::iterator it = indlist.begin(); it != indlist.end(); it++) {
		if(*it == value) {
			indlist.erase(it);
		}
	}
}

void Database::AddRecord(LessonInfo rec) {
	recs_.push_back(rec);
	recs_n_++;
	LessonInfo* ptr = &recs_.back();

	AddToMapList(teachers_, rec.teacher_, ptr);
	AddToMapList(  groups_, rec.group_,   ptr);
	AddToMapList(   rooms_, rec.room_,    ptr);
	AddToMapList(    days_, rec.day_,     ptr);
	AddToMapList(   times_, rec.time_,    ptr);
	AddToMapList(subjects_, rec.subject_, ptr);
}

void Database::RemoveRecord(LessonInfo* ptr) {
	

	//recs_.erase(ptr);


	//!!!!!
	//нужно удалить из vector<LessonInfo>
	//элемент *ptr,
	//и при этом не сломать все остальные существующие указатели
	//(уменьшить тех, кто после, на один?)
	//было бы гораздо удобнее, если бы там был список
	//или что-то другое STL-вское

	recs_n_--;
	RemoveFromMapList(teachers_, ptr->teacher_, ptr);
	RemoveFromMapList(  groups_, ptr->group_,   ptr);
	RemoveFromMapList(   rooms_, ptr->room_,    ptr);
	RemoveFromMapList(    days_, ptr->day_,     ptr);
	RemoveFromMapList(   times_, ptr->time_,    ptr);
	RemoveFromMapList(subjects_, ptr->subject_, ptr);
}

void Database::RemoveRecords(const SearchConditions conds) {
	IndicesList blacklist = SelectByConditionList(conds);

	for(IndicesList::iterator it = blacklist.begin(); it != blacklist.end(); it++) {
		RemoveRecord(*it);
	}
}

void Database::SaveToFile(string filename) const {
	//запись в файл...
}

void SetupGlobalDatabase() {
	__ALL_DATA = Database(__ALL_DATA_FILENAME);
}
void ShutdownGlobalDatabase() {
	__ALL_DATA.SaveToFile(__ALL_DATA_FILENAME);
}
//---------------------------------------------------------------------------//
//                             пользовательское                              //
//---------------------------------------------------------------------------//
Session::~Session() {}
Session::Session() {}

int Session::DoSelect(const SearchConditions& sc) {
	selection_ = Database(__ALL_DATA, sc);
	return selection_.DbSize();
}

int Session::DoReselect(const SearchConditions& sc) {
	selection_ = Database(selection_, sc);
	return selection_.DbSize();
}

//---------------------------------------------------------------------------
//                       парсер и т.д.
//---------------------------------------------------------------------------
int ImplementSelect  (const SearchConditions& sc, Session& s) {
	return s.DoSelect(sc);
}

int ImplementReselect(const SearchConditions& sc, Session& s) {
	return s.DoReselect(sc);
}

void ImplementInsert  (const SearchConditions& sc) {
// не выдаёт ошибку, если поля заполнены по нескольку раз,
//а вместо этого запоминает последнее; незаполненные поля -> мусор
	LessonInfo rec;
	for(list<Cond>::const_iterator it = sc.begin(); it != sc.end(); it++) {

		if(it->relation != EQUAL) {
			/*EXCEPTION: INSERT плохо описан*/
		}

		SetInfo(rec, *it);//помещает в запись информацию из условия
	}
	__ALL_DATA.AddRecord(rec);
}

int ImplementRemove  (const SearchConditions& sc) {//возвращает число удалённых записей
	int res = __ALL_DATA.DbSize();

	__ALL_DATA.RemoveRecords(sc);

	return res - __ALL_DATA.DbSize();
}
string ImplementPrint   (const SearchConditions& sc, const Database& db) {//возвращает напечатанную таблицу
	string table;

//  ....

	return table;
}

string to_string(int num) {
	stringstream ss;
	ss << num;
	return ss.str();
}

string ImplementCommand(const Command& t, Session& s) {//перенаправляет на настоящие обработчики
	string res("ok\n");
	switch(t.cmd) {
		case SELECT  :
			res = "Selected "   + to_string(ImplementSelect  (t.conditions, s)) + " records successfully.\n";
			break;
		case RESELECT:
			res = "Reselected " + to_string(ImplementReselect(t.conditions, s)) + " records successfully.\n";
			break;
		case REMOVE:
			res = "Removed "    + to_string(ImplementRemove  (t.conditions   )) + " records successfully.\n";
			break;
		case INSERT:
			ImplementInsert (t.conditions);
			res = "Inserted one record successfully.\n"; 
			break;
		case PRINT:
			res = ImplementPrint(t.conditions, __ALL_DATA);
			break;
		default:
			/*EXCEPTION: BAD COMMAND*/
			break;
	}

	return res;
}

Command parse(const string& query) {
	//растаскивание строчки на кусочки...
}

string MainQueryHandler(const string& query, Session& s){
	return ImplementCommand(parse(query), s);
}