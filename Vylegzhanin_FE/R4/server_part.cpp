#include "server_part.h"

string DayNumbers[] = {"MON", "TUE", "WEN", "THU", "FRI", "SAT", "SUN"};
Database ALL_DATA, SELECTED_DATA;

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
		case 2: return TUE;
		case 3: return TUE;
		case 4: return TUE;
		case 5: return TUE;
		case 6: return TUE;
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
}

//---------------------------------------------------------------------------
//                       сама база данных
//---------------------------------------------------------------------------
Database::~Database() {}

Database::Database() {
	//...

	recs_n_ = 0;
}

Database::Database(string filename) {}
Database::Database(const Database& other, const SearchConditions criteria) {}

int Database::DbSize() const {
	return recs_n_;
}

void Database::AddRecord(LessonInfo rec) {
	//...

	recs_n_++;
}

void Database::RemoveRecords(const SearchConditions criteria) {}

void Database::SaveToFile(string filename) const {}

//---------------------------------------------------------------------------//
//                             пользовательское                              //
//---------------------------------------------------------------------------//
Session::~Session() {
	delete selection_;
}
Session::Session() {
	selection_ = new Database();
}

int Session::DoSelect(const SearchConditions& sc) {
	*selection_ = Database(__ALL_DATA, sc);
	return selection_->DbSize();
}

int Session::DoReselect(const SearchConditions& sc) {
	*selection_ = Database(*selection_, sc);
	return selection_->DbSize();
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

void ImplementInsert  (const SearchConditions& sc, Database& db) {
/*пока что небезопасная версия:
не выдаёт ошибку, если поля заполнены по нескольку раз,
а вместо этого запоминает последнее; по дефолту там мусор*/
	LessonInfo rec = {};
	for(list<Cond>::const_iterator it = sc.begin(); it != sc.end(); it++) {
		
		if(it->relation != EQUAL) {
			/*EXCEPTION: INSERT плохо описан*/
		}

		SetInfo(rec, *it);//помещает в запись информацию из условия
	}
	__ALL_DATA.AddRecord(rec);
}

int ImplementRemove  (const SearchConditions& sc, Database& db) {//возвращает число удалённых записей
	int res = 0;

	return res;
}
string ImplementPrint   (const SearchConditions& sc, const Database& db) {//возвращает напечатанную таблицу
	string table;
	return table;
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
			res = "Removed "    + to_string(ImplementRemove  (t.conditions, __ALL_DATA)) + " records successfully.\n";
			break;
		case INSERT:
			ImplementInsert (t.conditions, __ALL_DATA);
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

Command parse(const string& query) {//растаскивание строчки на кусочки
}

string MainQueryHandler(const string& query, Session& s){//вот эта функция работает как чёрный ящик и доступна извне сервера
	return ImplementCommand(parse(query), s);
}