#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>
#include "RecordSet.h"

#define DAY_LEN 8
#define N_AUD 5

namespace R2
{
	// struct Cell
	// {
	// 	size_t 	teacherId, 
	// 			courseId,
	// 			t0;
	// };

	struct Course
	{
		size_t 	courseId;
		string	title; 

		Course(size_t courseId_, const string& title_)
		{
			courseId = courseId_, title = title_;
		}
	};

	struct Teacher
	{
		size_t 	teacherId;
		string	name; 
		string	profTitle; 

		Teacher(size_t teacherId_, const string& name_, const string& profTitle_)
		{
			teacherId = teacherId_, name = name_, profTitle = profTitle_;
		}
	};

	struct Group
	{
		size_t 	groupId;
		size_t	year;

		Group(size_t groupId_, size_t year_)
		{
			groupId = groupId_, year = year_;
		}
	};

	struct Lecture
	{
		size_t 	courseId;
		size_t 	groupId;
		size_t	teacherId;
		size_t	day;
		size_t	t0, len; 
		size_t	r;

		Lecture(size_t r, size_t day, size_t t0, size_t len, size_t courseId, size_t groupId, size_t teacherId)
		{
			this->r = r;
			this->day = day;
			this->t0 = t0;
			this->len = len;
			this->courseId = courseId;
			this->groupId = groupId;
			this->teacherId = teacherId;
		}
	};

	class TimetableDB
	{
		size_t nL, nRooms, nC;

		// Data

		mutable vector<Lecture*> timetable;

		mutable map<size_t,Teacher*> teachers;	mutable size_t teacherMaxId;
		mutable map<size_t,Course*> courses;	mutable size_t courseMaxId;
		mutable map<size_t,Group*> groups;		

		// Indexes

		mutable map<string,size_t> teacherIds;
		mutable map<string,size_t> courseIds;

		// mutable map<size_t,Lecture*> course_group_2_lecture;
		// mutable map<size_t,Lecture*> course_teacher_2_lecture;
		// mutable map<size_t,size_t> time_2_room;

		// mutable map<size_t,vector<Lecture*> > idx_room_lectures;
		// mutable map<size_t,vector<Lecture*> > idx_teacher_lectures;
		// mutable map<size_t,vector<Lecture*> > idx_group_lectures;
		// mutable map<size_t,vector<Lecture*> > idx_course_lectures;

		mutable map<size_t,vector<size_t> > idx_teacher_lectures;
		mutable map<size_t,vector<size_t> > idx_group_lectures;
		mutable map<size_t,vector<size_t> > idx_course_lectures;

		void init(int nL_, int nRooms_)
		{
			nL = nL_, nRooms = nRooms_, nC = nL * nRooms;
			//
			timetable.resize(nC);
			for (size_t j = 0; j < nC; ++j)
				timetable[j] = nullptr;
			//
			teacherMaxId = 0;
			courseMaxId = 0;
		}

	public:
		TimetableDB()
		{
			init(DAY_LEN * 7, N_AUD);
		}

		void read(const string& path) const
		{
			ifstream infile(path);
			vector<string> Q;
			string line;
			while (getline(infile, line))
			{
				if (line[line.size()-1] == '\n')
					line = line.substr(0, line.size()-1);
				Q.push_back(line);
			}
			infile.close();
			for (auto line : Q)
			{
				auto path1 = path;
				if (line.substr(0, 4) == "read" || line.substr(0, 4) == "save")
				{
					auto a = split(line, ' ');
					path1 = a[1];
				}
				processRequest(line, path1);
			}
		}

		void save(const string& path) const
		{
			string q = "";
			for (auto const& [id, o] : courses)
			{
				q += "insert course title=" + o->title + "\n";
			}
			for (auto const& [id, o] : groups)
			{
				q += "insert group id=" + toStr(o->groupId) + " year=" + toStr(o->year) + "\n";
			}
			for (auto const& [id, o] : teachers)
			{
				q += "insert teacher name=" + o->name + " title=" + o->profTitle + "\n";
			}
			//
			for (size_t r = 0; r < N_AUD; ++r)
			{
				for (size_t day = 0; day < 7; ++day)
				{
					for (size_t t0 = 0; t0 < DAY_LEN; ++t0)
					{
						auto cellId = getCellIndex(day * DAY_LEN + t0, r);
						Lecture* lecture = timetable[cellId];
						if (!lecture)
							continue;
						if (lecture->t0 != t0)
							continue;
						//
						q += "insert lecture room=" + toStr(r) + " day=" + toDay(day) + 
							 " t0=" + toStr(t0) + " len=" + toStr(lecture->len) + 
							 " teacher=" + teachers[lecture->teacherId]->name + 
							 " group=" + toStr(lecture->groupId) + 
							 " course=" + courses[lecture->courseId]->title + "\n";
					}
				}
			}
			//
			// cout << q << path << "\n-----------------" << endl;
			ofstream f;
			f.open(path);
			f << q;
			f.close();
		}

		// ------------------------------------------------------------------------
		// Queries

		RecordSet insertTeacher(const string& name, const string& profTitle) const
		{
			if (teacherIds.find(name) != teacherIds.end())
				return errorSet("Teacher already exists");
			//
			++teacherMaxId;
			size_t id = teacherMaxId;
			teachers[id] = new Teacher(id, name, profTitle);
			teacherIds[name] = id;
			//
			return okSet("1");
		}

		RecordSet insertCourse(const string& title) const
		{
			if (courseIds.find(title) != courseIds.end())
				return errorSet("Course already exists");
			//
			++courseMaxId;
			size_t id = courseMaxId;
			courses[id] = new Course(id, title);
			courseIds[title] = id;
			//
			return okSet("1");
		}

		RecordSet insertGroup(size_t groupId, size_t year) const
		{
			if (groups.find(groupId) != groups.end())
				return errorSet("Group already exists");
			//
			groups[groupId] = new Group(groupId, year);
			//
			return okSet("1");
		}

		inline size_t getCellIndex(size_t t, size_t r) const
		{
			return t + r * 7 * DAY_LEN;
		}

		RecordSet insertLecture(size_t r, size_t day, size_t t0, size_t len, size_t courseId, size_t groupId, size_t teacherId) const
		{
			if (r < 0 || r >= N_AUD)
				return errorSet("wrong aud id");
			if (day < 0 || day >= 7)
				return errorSet("wrong day number");
			if (t0 < 0 || t0 >= DAY_LEN)
				return errorSet("wrong lecture time in a day");
			if (t0+len-1 >= DAY_LEN)
				return errorSet("wrong lecture length");
			//
			size_t t = day * DAY_LEN + t0;
			size_t cellId = getCellIndex(t, r);
			for (auto t1 = cellId; t1 < t + len; ++t1)
			{
				if (timetable[t1])
					return errorSet("time is occupied");
			}
			//
			Lecture* lecture = new Lecture(r, day, t0, len, courseId, groupId, teacherId);
			//
			for (auto t1 = cellId; t1 < cellId + len; ++t1)
			{
				timetable[t1] = lecture;
			}
			//
			// indexes
			//
			if (idx_course_lectures.find(courseId) == idx_course_lectures.end())
				idx_course_lectures[courseId] = vector<size_t>();
			if (idx_teacher_lectures.find(teacherId) == idx_teacher_lectures.end())
				idx_teacher_lectures[teacherId] = vector<size_t>();
			if (idx_group_lectures.find(groupId) == idx_group_lectures.end())
				idx_group_lectures[groupId] = vector<size_t>();
			//
			addToList(idx_course_lectures[courseId], cellId);
			addToList(idx_teacher_lectures[teacherId], cellId);
			addToList(idx_group_lectures[groupId], cellId);
			//
			printTT();
			//
			return okSet("1");
		}

		inline void removeFromList(vector<size_t>& v, size_t x) const
		{
			size_t j0 = -1;
			for (size_t j = 0; j < v.size(); ++j)
			{
				if (v[j] == x)
				{
					j0 = j;
					break;
				}
			}
			if (j0 == -1)
				return;
			//
			if (v.size() == 1)
			{
				v.clear();
				return;
			}
			//
			cout << "resizing..." << endl;
			for (size_t j = j0; j < v.size()-1; ++j)
				v[j] = v[j+1];
			v.resize(v.size()-1);
		}

		inline void addToList(vector<size_t>& v, size_t x) const
		{
			size_t j0 = -1;
			for (size_t j = 0; j < v.size(); ++j)
			{
				if (v[j] == x)
				{
					j0 = j;
					break;
				}
			}
			if (j0 != -1)
				return;
			//
			j0 = -1;
			for (size_t j = 0; j < v.size(); ++j)
			{
				if (v[j] > x)
				{
					j0 = j;
					break;
				}
			}
			//
			v.resize(v.size()+1);
			if (j0 == -1)
			{
				v[v.size()-1] = x;
			}
			else
			{
				for (size_t j = v.size()-1; j > j0; --j)
					v[j] = v[j-1];
				v[j0] = x;
			}
		}
		
		inline RecordSet removeLecture(size_t cellId) const
		{
			// cout << "cellId = " << cellId << endl;
			Lecture* lecture = timetable[cellId];
			if (!lecture)
				return errorSet("no lecture");
			//
			size_t t = cellId % (DAY_LEN * 7);
			size_t t0 = t % DAY_LEN;
			//
			// if (lecture->t0 != t0)
			// 	lecture = timetable[cellId - (lecture->t0 - t0)];
			//
			// cout << "cellId = " << cellId << endl;
			removeFromList(idx_course_lectures[lecture->courseId], cellId);
			removeFromList(idx_teacher_lectures[lecture->teacherId], cellId);
			removeFromList(idx_group_lectures[lecture->groupId], cellId);
			// cout << "* removed" << endl;
			for (size_t t = cellId; t <= cellId + lecture->len; ++t)
				timetable[t] = nullptr;
			delete lecture;
			//
			printTT();
			//
			return okSet("1");
		}

		inline RecordSet removeLecture(size_t day, size_t t0, size_t r) const
		{
			auto cellId = getCellIndex(day * DAY_LEN + t0, r);
			return removeLecture(cellId);
		}

		inline void printTT() const
		{
			printf("TIMETABLE\n");
			for (int r = 0; r < N_AUD; ++r)
			{
				printf("%02d : ", r);
				for (int day = 0; day < 7; ++day)
				{
					for (int t0 = 0; t0 < DAY_LEN; ++t0)
					{
						Lecture* l = timetable[getCellIndex(day * DAY_LEN + t0, r)];
						if (l)
							printf("%c", courses[l->courseId]->title[0]);
						else
							printf(".");
					}
					printf("   ");
				}
				printf("\n");
			}
			printf("\n");
			for (int r = 0; r < N_AUD; ++r)
			{
				printf("%02d : ", r);
				for (int day = 0; day < 7; ++day)
				{
					for (int t0 = 0; t0 < DAY_LEN; ++t0)
					{
						Lecture* l = timetable[getCellIndex(day * DAY_LEN + t0, r)];
						if (l)
							printf("%d", l->groupId);
						else
							printf(".");
					}
					printf("   ");
				}
				printf("\n");
			}
			printf("~~\n");
			//
			cout << "teachers: {";
			int j = 0;
			for (auto const& [id, val] : teachers)
			{
				if (j > 0) cout << ", ";
			    cout << id << ':' << val->name << ':' << val->profTitle;
			    j++;
			}
			cout << "}" << endl;
			//
			cout << "courses: {";
			j = 0;
			for (auto const& [id, val] : courses)
			{
				if (j > 0) cout << ", ";
			    cout << id << ':' << val->title;
			    j++;
			}
			cout << "}" << endl;
		}

		inline void pushLecture(RecordSet& rs, Lecture* lecture) const
		{
			if (!lecture)
				throw Exception("no lecture!");
			//
			rs.pushValues(
				toDay(lecture->day),
				toStr(lecture->t0), 
				toStr(lecture->groupId), 
				courses[lecture->courseId]->title, 
				teachers[lecture->teacherId]->name 
				);
		}

		RecordSet selectDay(size_t day, size_t room) const
		{
			RecordSet rs;
			for (size_t t0 = 0; t0 < DAY_LEN; t0++)
			{
				size_t t = day * DAY_LEN + t0;
				size_t cellId = getCellIndex(t, room);
				Lecture* lecture = timetable[cellId];
				if (lecture)
				{
					if (lecture->t0 == t0)
					{
						pushLecture(rs, lecture);
					}
				}
			}
			return rs;
		}
		//{}

		RecordSet selectLectures4Room(size_t room) const
		{
			RecordSet rs;
			for (size_t day = 0; day < 7; ++day)
			{
				for (size_t t0 = 0; t0 < DAY_LEN; ++t0)
				{
					size_t t = day * DAY_LEN + t0;
					size_t cellId = getCellIndex(t, room);
					Lecture* lecture = timetable[cellId];
					if (lecture)
					{
						if (lecture->t0 == t0)
						{
							pushLecture(rs, lecture);
						}
					}
				}
			}
			return rs;
		}

		RecordSet selectLectures4Course(size_t courseId) const
		{
			RecordSet rs;
			for (int j = 0; j < idx_course_lectures[courseId].size(); ++j)
			{
				size_t cellId = idx_course_lectures[courseId][j];
				Lecture* lecture = timetable[cellId];
				pushLecture(rs, lecture);
			}
			return rs;
		}

		RecordSet selectLectures4Group(size_t groupId) const
		{
			RecordSet rs;
			for (int j = 0; j < idx_group_lectures[groupId].size(); ++j)
			{
				size_t cellId = idx_group_lectures[groupId][j];
				Lecture* lecture = timetable[cellId];
				pushLecture(rs, lecture);
			}
			return rs;
		}

		RecordSet selectTotalCourseLen(size_t courseId) const;

		RecordSet selectLectures4Teacher(size_t teacherId) const
		{
			RecordSet rs;
			for (int j = 0; j < idx_teacher_lectures[teacherId].size(); ++j)
			{
				size_t cellId = idx_teacher_lectures[teacherId][j];
				Lecture* lecture = timetable[cellId];
				pushLecture(rs, lecture);
			}
			return rs;
		}

		RecordSet selectTotalTeacherLoad(size_t teacherId) const
		{
			size_t teacherLoad = 0;
			for (int j = 0; j < idx_teacher_lectures[teacherId].size(); ++j)
			{
				size_t cellId = idx_teacher_lectures[teacherId][j];
				Lecture* lecture = timetable[cellId];
				teacherLoad += lecture->len;
			}
			RecordSet rs;
			rs.pushValues(teachers[teacherId]->name, toStr(teacherLoad));
			return rs;
		}

		// ------------------------------------------------------------------------
		// Language

		inline static TimetableDB& Instance()
		{
			static TimetableDB s;
			return s;
		}

		inline vector<string> split(const string& s_, char sep) const
		{
			string s = s_;
			vector<string> a;
			size_t i0 = s.find(sep);
			while (i0 != -1)
			{
				string item = s.substr(0, i0);
				if (item.size() > 0)
					a.push_back(item);
				s = s.substr(i0+1);
				i0 = s.find(sep);
			}
			if (s.size() > 0)
				a.push_back(s);
			return a;
		}

		inline void parseNameValue(const string& s, string& name, string& val) const
		{
			auto a = split(s, '=');
			if (a.size() == 0)
			{
				name = "", val = "";
				return;
			}
			if (a.size() == 1)
			{
				name = s, val = "";
				return;
			}
			name = a[0];
			val = "";
			for (size_t j = 1; j < a.size(); ++j)
			{
				if (j > 1)
					val += "=";
				val += a[1];
			}
		}

		inline RecordSet okSet(const string& msg) const
		{
			RecordSet rs;
			rs.pushValues("OK", msg);
			return rs;
		}

		inline RecordSet errorSet(const string& msg) const
		{
			RecordSet rs;
			rs.pushValues("ERROR", msg);
			return rs;
		}

		inline static int convertToInt(const string& s)
		{
			int x;
			sscanf(s.c_str(), "%d", &x);
			return x;
		}

		inline static int convertToDay(const string& s)
		{
			if (s == "MON") return 0;
			if (s == "TUE") return 1;
			if (s == "WED") return 2;
			if (s == "THU") return 3;
			if (s == "FRI") return 4;
			if (s == "SAT") return 5;
			if (s == "SUN") return 6;
			return -1;
		}

		inline static string toDay(int day)
		{
			if (day == 0) return "MON";
			if (day == 1) return "TUE";
			if (day == 2) return "WED";
			if (day == 3) return "THU";
			if (day == 4) return "FRI";
			if (day == 5) return "SAT";
			if (day == 6) return "SUN";
			return "";
		}

		inline static string toStr(size_t x)
		{
			char s[126];
			sprintf(s, "%d", x);
			return string(s);
		}

		inline RecordSet processRequest(const string& query, const string& dbPath) const
		{
			auto Q = split(query, ' ');
			if (Q.size() == 0)
				return errorSet("empty query");
			//
			string command = Q[0];
			if (command == "save")
			{
				cout << "SAVING... ";
				save(dbPath);
				cout << " done!" << endl;
				return okSet("1");
			}
			else if (command == "read")
			{
				cout << "READING... " << endl;
				read(dbPath);
				cout << " done!" << endl;
				return okSet("1");
			}
			//
			if (Q.size() <= 1)
				return errorSet("invalid query: object missing");
			//
			if (command == "insert")
			{
				string objectName = Q[1];
				//
				if (objectName == "teacher")
				{
					if (Q.size() < 4)
						return errorSet("invalid query: invalid arguments");
					//
					string name, title;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "name")
							name = _v;
						else if (_n == "title")
							title = _v;
					}
					//
					if (name.size() == 0 || title.size() == 0)
						return errorSet("invalid query: missing arguments: " + query);
					//
					return insertTeacher(name, title);
				}
				else if (objectName == "course")
				{
					if (Q.size() < 3)
						return errorSet("invalid query: invalid arguments");
					//
					string title;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "title")
							title = _v;
					}
					//
					if (title.size() == 0)
						return errorSet("invalid query: missing arguments (course) " + query);
					//
					return insertCourse(title);
				}
				else if (objectName == "group")
				{
					if (Q.size() < 4)
						return errorSet("invalid query: invalid arguments: " + query);
					//
					int id = 0, year = 0;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "id")
							id = convertToInt(_v);
						else if (_n == "year")
							year = convertToInt(_v);
					}
					//
					if (id == 0 || year == 0)
						return errorSet("invalid query: missing arguments: " + query);
					//
					return insertGroup(id, year);
				}
				else if (objectName == "lecture")
				{
					size_t r = 0;
					size_t day = -1;
					size_t t0 = -1;
					size_t len = 0;
					string courseTitle;
					size_t groupId = -1;
					string teacherName;
					//
					if (Q.size() < 9)
						return errorSet("invalid query: invalid arguments: " + query);
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						// cout << "_n=" << _n << "   _v=" << _v << endl;
						if (_n == "room")
							r = convertToInt(_v);
						else if (_n == "day")
							day = convertToDay(_v);
						else if (_n == "t0")
							t0 = convertToInt(_v);
						else if (_n == "len")
							len = convertToInt(_v);
						else if (_n == "course")
							courseTitle = _v;
						else if (_n == "group")
							groupId = convertToInt(_v);
						else if (_n == "teacher")
							teacherName = _v;
					}
					//
					// cout << r << " " << day << " " << t0 << " " << len << " " << courseTitle << " " 
					// 	 << groupId << " " << teacherName << endl;
					if (r == 0 || day == -1 || t0 == -1 || len <= 0 || courseTitle.size() == 0 || teacherName.size() == 0)
						return errorSet("invalid query: missing arguments: " + query);
					//
					if (teacherIds.find(teacherName) == teacherIds.end())
						return errorSet("teacher " + teacherName + " not found");
					size_t teacherId = teacherIds[teacherName];
					//
					if (courseIds.find(courseTitle) == courseIds.end())
						return errorSet("course " + courseTitle + " not found");
					size_t courseId = courseIds[courseTitle];
					//
					if (groups.find(groupId) == groups.end())
						return errorSet("group not found");
					//
					return insertLecture(r, day, t0, len, courseId, groupId, teacherId);
				}
			}
			else if (command == "remove")
			{
				string objectName = Q[1];
				//
				if (objectName == "lecture")
				{
					size_t r = 0;
					size_t day = -1;
					size_t t0 = -1;
					//
					if (Q.size() < 5)
						return errorSet("invalid query: invalid arguments");
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "room")
							r = convertToInt(_v);
						else if (_n == "day")
							day = convertToDay(_v);
						else if (_n == "t0")
							t0 = convertToInt(_v);
					}
					//
					if (r == 0 || day == -1 || t0 == -1)
						return errorSet("invalid query: missing arguments: " + query);
					//
					removeLecture(day, t0, r);
				}
			}
			else if (command == "select")
			{
				string objectName = Q[1];
				//
				if (objectName == "day")
				{
					if (Q.size() < 4)
						return errorSet("invalid query: invalid arguments");
					//
					int day = -1, room = -1;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "day")
							day = convertToDay(_v);
						else if (_n == "room")
							room = convertToInt(_v);
					}
					//
					if (room == -1 || day == -1)
						return errorSet("invalid query: missing arguments: " + query);
					//
					return selectDay(day, room);
				}
				else if (objectName == "room")
				{
					if (Q.size() < 3)
						return errorSet("invalid query: invalid arguments");
					//
					int room = -1;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "room")
							room = convertToInt(_v);
					}
					//
					if (room == -1)
						return errorSet("invalid query: missing arguments: " + query);
					//
					return selectLectures4Room(room);
				}
				else if (objectName == "course_lectures")
				{
					if (Q.size() < 3)
						return errorSet("invalid query: invalid arguments");
					//
					string courseTitle;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "course")
							courseTitle = _v;
					}
					//
					if (courseTitle.size() == 0)
						return errorSet("invalid query: missing arguments: " + query);
					//
					if (courseIds.find(courseTitle) == courseIds.end())
						return errorSet("course " + courseTitle + " not found");
					size_t courseId = courseIds[courseTitle];
					//
					return selectLectures4Course(courseId);
				}
				else if (objectName == "group_lectures")
				{
					if (Q.size() < 3)
						return errorSet("invalid query: invalid arguments");
					//
					int id = -1;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "group")
							id = convertToInt(_v);
					}
					//
					if (id == -1)
						return errorSet("invalid query: missing arguments: " + query);
					//
					return selectLectures4Group(id);
				}
				else if (objectName == "teacher_lectures")
				{
					if (Q.size() < 3)
						return errorSet("invalid query: invalid arguments");
					//
					string teacherName;
					//
					for (int k = 2; k < Q.size(); ++k)
					{
						string _n, _v;
						parseNameValue(Q[k], _n, _v);
						if (_n == "teacher")
							teacherName = _v;
					}
					//
					if (teacherName.size() == 0)
						return errorSet("invalid query: missing arguments: " + query);
					//
					if (teacherIds.find(teacherName) == teacherIds.end())
						return errorSet("teacher " + teacherName + " not found");
					// cout << "teacherId: ?" << endl;
					size_t teacherId = teacherIds[teacherName];
					//
					// cout << "teacherId: " << teacherId << endl;
					return selectLectures4Teacher(teacherId);
				}
			}
			return okSet("0");
		}
	};
}
