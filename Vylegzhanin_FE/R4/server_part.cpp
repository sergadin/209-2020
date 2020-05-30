#include "server_part.h"
#include "exceptions.h"
#include <sstream>

const string DayNumbers[] = {"MON", "TUE", "WEN", "THU", "FRI", "SAT", "SUN"};

//----------------------------------------------------------------------------//
//                        какие-то базовые структуры                          //
//----------------------------------------------------------------------------//

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
		default:
			string err_msg("Bad weekday ");
			err_msg += s; 
			throw DatatypeException(string("GetDayFromString"), err_msg);
			break;
	}

	return MON;//костыль для строгого компилятора
}

string to_string(int num) {
	stringstream ss;
	ss << num;
	return ss.str();
}

//----------------------------------------------------------------------------//
//                 структуры, представляющие запрос в памяти                  //
//----------------------------------------------------------------------------//
string StringFormOfData(Field field, LessonInfo data) {
	switch(field) {
		case TEACHER: return data.teacher_;
		case SUBJECT: return data.subject_;
		case ROOM:    return to_string(data.room_ );
		case GROUP:   return to_string(data.group_);
		case TIME:    return to_string(data.time_ );
		case WEEKDAY: return DayNumbers[data.day_];
		default: return string("NaN");
	}
}

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

		case SORT: default: /*EXCEPTION: BAD FIELD*/
			string err_msg("Bad field type");
			throw QuerySyntaxException("SetInfo", err_msg);
			break;
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
				string err_msg("Impossible relation for type 'string'");
				throw QuerySyntaxException("CheckCondition", err_msg);
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
				string err_msg("Impossible relation for type 'int'");
				throw QuerySyntaxException("CheckCondition", err_msg);
				break;
		}
		break;


		default: /*это значит не разобран случай*/ break;
	}

	return false;//костыль для строгого компилятора
}

//----------------------------------------------------------------------------//
//                        сама база данных                                    //
//----------------------------------------------------------------------------//

IndicesList Database::SelectByCondition(Cond condition) {
	IndicesList res;
//здесь должно быть нечто объёмное
//отчасти похоже на checkcondition(), но сложнее
//несколько switch и т.д.
	return res;
}

template<typename T> list<T> IntersectionOfLists(list<T> a, list<T> b) {
	list<T> res;
	for(typename list<T>::iterator i = a.begin(); i != a.end(); i++) {
		bool in_b = false;
		T value = *i;

		for(typename list<T>::iterator j = b.begin(); j != b.end(); j++) {
			if(*j == value) {
				in_b = true;
				break;
			}
		}

		if(in_b) {
			res.push_back(value);
		}
	}

	return res;
}

IndicesList Database::SelectByConditionList(SearchConditions cond_list) {

	if (cond_list.begin() == cond_list.end()) {//если условий нет -- возвращаем все записи
		IndicesList all;
		for(list<LessonInfo>::iterator it = recs_.begin();it != recs_.end(); it++) {
			all.push_back(it);
		}
		return all;
	}

	IndicesList res = SelectByCondition(*cond_list.begin());

	for(SearchConditions::const_iterator it  = cond_list.begin();
										 it != cond_list.end();
										 it++) {
		res = IntersectionOfLists(res, SelectByCondition(*it));
	}

	return res;
}

template<typename T> void Database::AddToMapList(map<T,IndicesList>& data, T key, DbIndex value) {
	if(data.count(key) == 0) {
		data[key] = IndicesList(1,value);
	}
	else data[key].push_back(value);
}

template<typename T> void Database::RemoveFromMapList (map<T,IndicesList>& data, T key, DbIndex value) {
	if(data.count(key) == 0) {
		string err_msg("key isn't found in map list");
		throw DatabaseException("Database::RemoveFromMapList", err_msg);
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
	DbIndex index = recs_.end();

	AddToMapList(teachers_, rec.teacher_, index);
	AddToMapList(  groups_, rec.group_,   index);
	AddToMapList(   rooms_, rec.room_,    index);
	AddToMapList(    days_, rec.day_,     index);
	AddToMapList(   times_, rec.time_,    index);
	AddToMapList(subjects_, rec.subject_, index);
}

void Database::RemoveRecord(DbIndex index) {
	RemoveFromMapList(teachers_, index->teacher_, index);
	RemoveFromMapList(  groups_, index->group_,   index);
	RemoveFromMapList(   rooms_, index->room_,    index);
	RemoveFromMapList(    days_, index->day_,     index);
	RemoveFromMapList(   times_, index->time_,    index);
	RemoveFromMapList(subjects_, index->subject_, index);

	recs_.erase(index);
	recs_n_--;
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

Database::Database(string filename) {
	//загрузка из файла
}

Database::~Database() {
	SaveToFile(filename_);
}

void Database::ImplementInsert  (const SearchConditions& sc) {
// не выдаёт ошибку, если поля заполнены по нескольку раз,
//а вместо этого запоминает последнее; незаполненные поля -> мусор
	LessonInfo rec;
	for(list<Cond>::const_iterator it = sc.begin(); it != sc.end(); it++) {

		if(it->relation != EQUAL) {
			string err_msg("cond.relation != EQUAL in SearchCondition");
			throw QuerySyntaxException("Database::ImplementInsert", err_msg);
		}

		SetInfo(rec, *it);//помещает в запись информацию из условия
	}
	AddRecord(rec);
}

int Database::ImplementSelect  (const SearchConditions& sc, Session& s) {
	s.SetSelection(SelectByConditionList(sc));
}

int Database::ImplementReselect(const SearchConditions& sc, Session& s) {
	IndicesList old_selection = s.GetSelection();
	s.SetSelection(IntersectionOfLists(old_selection,
									   SelectByConditionList(sc))
				  );
}


int Database::ImplementRemove  (const SearchConditions& sc) {//возвращает число удалённых записей
	int res = Size();

	RemoveRecords(sc);

	return res - Size();
}

string Database::ImplementPrint   (const SearchConditions& sc, Session& s) {//возвращает напечатанную таблицу

	IndicesList indlist = s.GetSelection();
	size_type tablesize = intlist.size();
	vector<string> table;

	SearchConditions::const_iterator sorting_begin;

	//перенос данных в соотв. строки
	for(SearchConditions::const_iterator sc_it = sc.begin(); sc_it != sc.end(); sc++) {
		if(sc_it->field == SORT) {
			sc_it++;
			sorting_begin = sc_it;
			break;
		}

		string row;
		for(list<DbIndex>::iterator ind_it = indlist.begin(); ind_it != indlist.end(); ind_it++) {
			switch(sc_it->field) {
				case TEACHER: case ROOM: case GROUP: case DAY: case TIME: case SUBJECT:
				row += " "; row += StringFormOfData(sc_it->field, **ind_it); break;
				default: throw QuerySyntaxException("Database::ImplementPrint", "Unknown data field");
			}
		}
	}

	
	return table;
}

string Database::ImplementCommand(const Command& t, Session& s) {//перенаправляет на настоящие обработчики
	string res("ok\n");

	try {
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
				res = ImplementPrint(t.conditions);
				break;
			default:
				throw QuerySyntaxException("Database::ImplementCommand", "Unknown command");
				break;
		}

	}	
	catch(QuerySyntaxException& ex) {
		cerr << "Query syntax error in function " << ex.FuncName() << ':' << endl;
		cerr << ex.Message() << endl;
		return ex.Message();
	}
	catch(...) {
		string err_msg("Unknown exception occured"); 
		cerr << err_msg << endl;
		return err_msg;
	}

	return res;
}

string Database::HandleQuery(const string& query, Session& s){
	return ImplementCommand(parse(query), s);
}

//----------------------------------------------------------------------------//
//                              парсер и т.д.                                 //
//----------------------------------------------------------------------------//


Command parse(const string& query) {
	Command res;
	//растаскивание строчки на кусочки...
	return res;
}
