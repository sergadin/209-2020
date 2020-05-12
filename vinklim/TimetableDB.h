#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace R2
{
	struct Cell
	{
		size_t 	teacherId,
			courseId,
			t0;
	};

	struct Course
	{
		size_t 	courseId;
		string	title;
	};

	struct Teacher
	{
		size_t 	teacherId;
		string	name;
		string	profTitle;
	};

	struct Group
	{
		size_t 	groupId;
		size_t	year;
	};

	struct Lecture
	{
		size_t 	courseId;
		size_t 	groupId;
		size_t	teacherId;
		size_t	day;
		size_t	t0;
	};

	class TimetableDB
	{
		size_t nL, nRooms, nC;

		// Data

		mutable vector<Lecture*> timetable;
		mutable map<size_t, Teacher*> teachers;
		mutable map<size_t, Course*> courses;
		mutable map<size_t, Group*> groups;

		// Indexes

		mutable map<size_t, Lecture*> course_group_2_lecture;
		mutable map<size_t, Lecture*> course_teacher_2_lecture;
		mutable map<size_t, size_t> time_2_room;
		mutable map<size_t, vector<Lecture*> > idx_room_lectures;
		mutable map<size_t, vector<Lecture*> > idx_teacher_lectures;
		mutable map<size_t, vector<Lecture*> > idx_group_lectures;
		mutable map<size_t, vector<Lecture*> > idx_course_lectures;
		void init(int nL_, int nRooms_);
	public:
		TimetableDB()
		{
			init();
		}
		read();
		save();

		// ------------------------------------------------------------------------
		// Queries

		void insertTeacher(const string& name, const string& profTitle) const;
		void insertCourse(const string& title) const;
		void insertGroup(size_t groupId, size_t year) const;
		void insertLecture(size_t r, size_t t0, size_t len, size_t courseId, size_t groupId, size_t teacherId) const;
		void string selectDay(size_t day) const;
		void string selectRoom(size_t r) const;
		void string selectLectures4Course(size_t courseId) const;
		void string selectLectures4Group(size_t groupId) const;
		void string selectLectures4Teacher(size_t teacherId) const;
		void string selectLectures4Room(size_t r) const;
		void string selectTotalCourseLen(size_t courseId) const;
		void string selectTotalTeacherLoad(size_t teacherId) const;
	};
}