#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip> // for setw (tab) 
#include <ctime> 
#include <sstream>
#include <map>
#include <algorithm>   
using namespace std;



string random_name()
{
	string name;
	int i = 0;
	i = 5 + (rand() % 5);
	while(i != 0)
	{
		name += static_cast<char>(97 + (rand() % 26));
		i--;
	}
	name[0] = toupper(name[0]);
	return name;
}

string int_to_string(int i)
{
	string str;
	stringstream ss;      
	ss << i;    
	str = ss.str();
	return str;
}

string more(const string &com1,const string &com2)
{
	if(com1.size()>com2.size())
	{
		return com1;
	}
	if(com1.size()<com2.size())
	{
		return com2;
	}
	if(com1>=com2)
	{
		return com1;
	}
	else
	{
		return com2;
	}
}

bool number_or_not(const string &com, int i)
{
	if(com[i] == '0' || com[i] == '1' || com[i] == '2' || com[i] == '3' || com[i] == '4')
	{
		return 1; 
	}
	if(com[i] == '5' || com[i] == '6' || com[i] == '7' || com[i] == '8' || com[i] == '9')
	{
		return 1;
	}
	return 0;
}

int where_space(const string &com, int m)
{
	while(com[m] != ' ')
		{
			m++;
		}
	return m;
}

bool pole_or_not(const string &com)
{
	if(com == "teacher" || com == "group" || com == "subject" || com == "date_time" || com == "room")
	{
		return 1; 
	}
	return 0;
}

bool comand_or_not(const string &com)
{
	if(com == "select" || com == "reselect" || com == "print")
	{
		return 1; 
	}
	if(com == "generate" || com == "insert" || com == "remove" || com == "save")
	{
		return 1; 
	}
	return 0;
}
  
class MyException 
{

  int code_;
  string message_;
public:
	MyException(int code, const string& message) : code_(code), message_(message) {}
  	const string& message() const 
	{ return message_; }
	int code() const
	{ return code_; }
};
  
  
struct crit
{
    string pole;
    string low_val;
    string high_val;
    string val;
};

struct info 
{
	int index;
    string teacher;
    string group;
    string date_time;
    string room;
    string subject;
    void print()
    {
    	cout << endl << setw(10) << teacher << setw(5) << group << setw(6) << date_time; 
		cout << setw(6) << room << setw(10) << subject << "   " << index;
	}
	void clear()
	{
		teacher.clear();
		group.clear();
		date_time.clear();
		room.clear();
		subject.clear();
	}
};

struct proverka_insert_and_remove
{
    int teacher;
    int group;
    int subject;
    int date_time;
    int room;
};

struct condition // одна строчка
{
	string comand;
	vector<crit> criteri;
	void make(const string &one_zapr)
	{	
		int i=0, q=0, i_copy, poin=0, coord_of_point;
		crit cr;
		string rab;
		proverka_insert_and_remove prov;
		while(one_zapr[i] != ' ')
		{
			comand.push_back(one_zapr[i]);
			i++;
		} 
		if(comand_or_not(comand) == 0)
		{
			throw MyException(-3, "Ne comanda");
		}
 		i_copy = i;
		while(i < one_zapr.size() - 4 )
		{
			if(one_zapr[i] == ' ' && one_zapr[i+1] == ' ')
			{
				throw MyException(-2, "Dva probela");
			}
			i++;
		}
		i = i_copy + 1;//указатель на первой букве
		i_copy++;
		poin = 0;
		if(comand == "insert" || comand == "remove" || comand == "print")
		{
			prov.date_time = 0;
			prov.group = 0;
			prov.room = 0;
			prov.subject = 0;
			prov.teacher = 0;
		}
		if(comand == "save")
		{
			if(one_zapr != "save end")
			{
				throw MyException(11, "Komanda save ne korectno zadana");
			}
		}
		else if(comand == "generate")
		{
			if(one_zapr[i] == '0' && one_zapr[i+1] != ' ')
			{
				throw MyException(272, "V komande generate ne korectno zadano chislo");
			}
			while(i < one_zapr.size() - 4 )
			{
				if(one_zapr[i] == ' ')
				{
					throw MyException(312, "Komanda generate ne korectno zadana");
				}
				if(number_or_not(one_zapr,i)!= 1)
				{
					throw MyException(892, "V komande generate ne korectno zadano chislo");
				}
				cr.val += one_zapr[i]; 
				i++;
			}
			criteri.push_back(cr);
			cr.val.clear();
		}
		else if(comand == "print")
		{
			while(i < one_zapr.size() - 3 )
			{
				if(one_zapr[i] == ' ')
				{
					if(pole_or_not(cr.pole) == 0)
					{
						throw MyException(12, "Ne corectnoe pole");
					}
					if(cr.pole[0] == 'd')
		 			{
		 				prov.date_time++;
					}
					if(cr.pole[0] == 'g')
		 			{
		 				prov.group++;
					}
					if(cr.pole[0] == 'r')
		 			{
		 				prov.room++;
					}
					if(cr.pole[0] == 's')
		 			{
		 				prov.subject++;
					}
					if(cr.pole[0] == 't')
		 			{
		 				prov.teacher++;
					}
 					criteri.push_back(cr);
					cr.pole.clear();
				}
				else
				{
					cr.pole.push_back(one_zapr[i]);
				}
				i++;
			}
			if(prov.date_time > 1 || prov.group > 1 || prov.room > 1 || prov.subject > 1 || prov.teacher > 1)
			{
				cout << prov.date_time << prov.group << prov.room << prov.subject << prov.teacher;
				throw MyException(8, "Ykazani ne vse pola");
			} 
		}
		else if(comand == "select" || comand == "reselect" || comand == "insert" || comand == "remove")
		{
			while(i < one_zapr.size() - 3 )
			{
				if(one_zapr[i] == '=')
				{
					if(poin == 1)
					{
						throw MyException(-1, "= and =");
					}
					poin = 1;
					
				}
				if(one_zapr[i] == ' ')
				{
					if(poin = 0)
					{
						throw MyException(-1, "Net ="); 
					}
					poin = 0;
				}
				i++;
			}
			i = i_copy;
			while(i < one_zapr.size() - 3) 
			{
				while(one_zapr[i] != '=')
				{
					cr.pole.push_back(one_zapr[i]);
					i++;
				}
				if(pole_or_not(cr.pole) == 0)
				{
					cout << endl << cr.pole;
					throw MyException(-1, "Ne corectnoe pole");
				}
				i++;
				i_copy = i;
		 		if(comand == "insert" || comand == "remove")
		 		{
		 			if(cr.pole[0] == 'd')
		 			{
		 				prov.date_time++;
					}
					if(cr.pole[0] == 'g')
		 			{
		 				prov.group++;
					}
					if(cr.pole[0] == 'r')
		 			{
		 				prov.room++;
					}
					if(cr.pole[0] == 's')
		 			{
		 				prov.subject++;
					}
					if(cr.pole[0] == 't')
		 			{
		 				prov.teacher++;
					}
				}
				if(cr.pole == "teacher" || cr.pole == "subject")
				{
					while(one_zapr[i] != ' ')
					{
						cr.val.push_back(one_zapr[i]);
						i++;
					}
					criteri.push_back(cr);
					cr.val.clear();
					cr.pole.clear();
				}
				else if(comand == "select" || comand == "reselect")
				{
					poin = 0; 
					while(one_zapr[i] != '-' && one_zapr[i] != ' ')
					{
						if(number_or_not(one_zapr,i) == 0)
						{
							if(one_zapr[i] != '*' || poin == 1)  
							{
								throw MyException(4, "Ne corectnoe chislo");
							}
							poin = 1;
							if(one_zapr[i+1] != '-')
							{
								throw MyException(5, "Ne corectnoe chislo");
							}
						}
						cr.low_val.push_back(one_zapr[i]);
						i++;
					}
					if(one_zapr[i] == ' ')
					{
						cr.high_val = cr.low_val;
					}
					else if(one_zapr[i] == '-')
					{
						i++;
						while(one_zapr[i] != ' ')
						{
							if(number_or_not(one_zapr,i) == 0)
							{
								if(one_zapr[i] != '*' || poin == 1)  
								{
									throw MyException(6, "Ne corectnoe chislo");
								}
								poin = 1;
								if(one_zapr[i+1] != ' ')
								{
									throw MyException(7, "Ne corectnoe chislo");
								}
							}
							cr.high_val.push_back(one_zapr[i]);
							i++;
						}
					}
					if(cr.pole == "date_time" && cr.low_val == "*")
					{
						cr.low_val = "0";
					}
					if(cr.pole == "date_time" && cr.high_val == "*")
					{
						cr.high_val = "2359";
					}
					if(cr.pole == "date_time" &&  more(cr.high_val,"2400") == cr.high_val )
					{
						cout << "time - " << cr.high_val;
						throw MyException(23, "Time ne corektno ukazano");
					}
					if(cr.pole == "group" && cr.low_val == "*")
					{
						cr.low_val = "0";
					}
					if(cr.pole == "room" && cr.low_val == "*")
					{
						cr.low_val = "0";
					}
					if(cr.pole == "group" && cr.high_val == "*")
					{
						cr.high_val = "1000000";
					}
					if(cr.pole == "room" && cr.high_val == "*")
					{
						cr.high_val = "1000000";
					}
					criteri.push_back(cr);
					cr.low_val.clear();
					cr.high_val.clear();
					cr.pole.clear();		
				}
				else if(comand == "insert" || comand == "remove")//указатель, на первой цифре
				{
					while(one_zapr[i] != ' ')
					{
						if(number_or_not(one_zapr,i) == 0)
						{
							throw MyException(9, "Ne corectnoe chislo");
						}
						cr.low_val.push_back(one_zapr[i]);
						i++;
					}
					cr.high_val = cr.low_val;
					criteri.push_back(cr);
					cr.low_val.clear();
					cr.high_val.clear();
					cr.pole.clear();
				}	
				
				i++;
			}
			if(comand == "insert" || comand == "remove")
			{
				if(prov.date_time != 1 || prov.group != 1 || prov.room != 1 || prov.subject != 1 || prov.teacher != 1)
				{
					cout << prov.date_time << prov.group << prov.room << prov.subject << prov.teacher;
					throw MyException(8, "Ykazani ne vse pola");
				}
			}
		}
	}
	void clear()
    {
        comand.clear();
        criteri.clear();
    }
    void print()
    {
    	int i=0;
    	cout << endl << "Comand - " << comand << endl;
    	while(i < criteri.size())
    	{
    		cout << "crit number " << i << " : pole - " << criteri[i].pole;
			cout << " low val - " << criteri[i].low_val << " val - " << criteri[i].val << endl;
    		i++;
		}
    	
	}
};


struct resultat
{
    string stroka;
    vector<info> inf;
    void print()
    {	
    	if(stroka.empty() == 1)
    	{
    		for(int i = 0;i < inf.size();i++)
    		{
    			inf[i].print();	
			}
		}
		else
		{
			cout << endl << stroka;
		}
	}
	void clear()
	{
		stroka.clear();
		inf.clear();
	}
};

class Result
{
private:
	vector<resultat> res;
public:
	void add_element(resultat el)
	{
		res.push_back(el);
	}
	void print()
	{
		int i=0;
		cout << endl <<  "This is result" << endl;
		while(i < res.size())
		{
			res[i].print();
			i++;
		}
	}
};


class Query
{
private:
	vector<condition> com;
public:
	Query(const string &zapr) // разделить по end-ам и проверить, что end последний и перед end пробел 
	{
		int q = 0, i = 0;
		string one_zapr;
		condition s;
		if(zapr[zapr.size()-4] != ' ' || zapr[zapr.size()-3] != 'e' || zapr[zapr.size()-2] != 'n' || zapr[zapr.size()-1] != 'd')
		{
			throw MyException(13, "Zapros konchaetsa ne ' end' ");
		}
		while(i < zapr.size()-3)   //разделяем запрос по камандам 
		{
			if(zapr[i] == ' ' && zapr[i+1] == 'e' && zapr[i+2] == 'n' && zapr[i+3] == 'd')
			{ 
				one_zapr=zapr.substr(q,i+4-q);
				s.make(one_zapr);
				com.push_back(s);
				s.clear();
				q = i + 5;
				i = i + 4;
			}
			i++;
		}
	}
	int sizee()
	{
		int i = com.size();
		return i;
	}
	condition elem(int i)
	{
		return com[i];
	}
};


bool ok_or_not_ok(const crit &cr,const info &inf)
{
	if(cr.pole[0] == 's')
	{
		if(inf.subject == cr.val)
		{
			return 1;
		}
	}
	else if(cr.pole[0] == 't')
	{
		if(inf.teacher == cr.val)
		{
			return 1;
		}
	}
	else if(cr.pole[0] == 'd')
	{
		if(more(inf.date_time,cr.low_val) == inf.date_time && more(cr.high_val,inf.date_time) == cr.high_val)
		{
			return 1;
		}
	}
	else if(cr.pole[0] == 'r')
	{
		if(cr.low_val == "*" && more(cr.high_val, inf.room) == cr.high_val)
		{
			return 1;
		}
		if(cr.high_val == "*" && more(inf.room, cr.low_val) == inf.room)
		{
			return 1;
		}
		if(more(inf.room, cr.low_val) == inf.room && more(cr.high_val, inf.room) == cr.high_val)
		{
			return 1;
		}
	}
	else if(cr.pole[0] == 'g')
	{
		if(cr.low_val == "*" && more(cr.high_val,inf.group) == cr.high_val)
		{
			return 1;
		}
		if(cr.high_val == "*" && more(inf.group, cr.low_val) == inf.group)
		{
			return 1;
		}
		if(more(cr.high_val,inf.group) == cr.high_val && more(inf.group, cr.low_val) == inf.group)
		{
			return 1;
		}
	}
	return 0;
}



class Database
{
private:
	int global_index;
	vector<info> inf;
	vector<info> ses;
	map <string,vector<int> > mp_t;
	map <string,vector<int> > mp_g;
	map <string,vector<int> > mp_r;
	map <string,vector<int> > mp_d;
	map <string,vector<int> > mp_s;
public:
	void add_el_to_maps(info in)
	{
		mp_t[in.teacher].push_back(in.index);
		mp_g[in.group].push_back(in.index);
		mp_r[in.room].push_back(in.index);
		mp_d[in.date_time].push_back(in.index);
		mp_s[in.subject].push_back(in.index);
	}
	void del_el_from_maps(info in)
	{
		int i = 0;
		while(in.index != mp_t[in.teacher][i])
		{
			i++;
		}
		mp_t[in.teacher].erase(mp_t[in.teacher].begin() + i);
		vector<int>(mp_t[in.teacher]).swap(mp_t[in.teacher]);
		i = 0;
		while(in.index != mp_g[in.group][i])
		{
			i++;
		}
		mp_g[in.group].erase(mp_g[in.group].begin() + i);
		vector<int>(mp_g[in.group]).swap(mp_g[in.group]);
		i = 0;
		while(in.index != mp_r[in.room][i])
		{
			i++;
		}
		mp_r[in.room].erase(mp_r[in.room].begin() + i);
		vector<int>(mp_r[in.room]).swap(mp_r[in.room]);
		i = 0;
		while(in.index != mp_d[in.date_time][i])
		{
			i++;
		}
		mp_d[in.date_time].erase(mp_d[in.date_time].begin() + i);
		vector<int>(mp_d[in.date_time]).swap(mp_d[in.date_time]);
		i = 0;
		while(in.index != mp_s[in.subject][i])
		{
			i++;
		}
		mp_s[in.subject].erase(mp_s[in.subject].begin() + i);
		vector<int>(mp_s[in.subject]).swap(mp_s[in.subject]);
	}
	Database(const string &file_name)
	{
		string a;
		info line; 
		int i = 0;
		ifstream in;
		in.open(file_name.c_str());
		global_index = 0;
	    if (in.is_open())
	    {
	        while (getline(in, a))
	        {
	            while(a[i] != ' ')
	            {
	            	line.teacher.push_back(a[i]);
	            	i++;
				}
				i++;
				while(a[i] != ' ')
	            {
	            	line.group.push_back(a[i]);
	            	i++;
				}
				i++;
				while(a[i] != ' ')
	            {
	            	line.date_time.push_back(a[i]);
	            	i++;
				}
				i++;
				while(a[i] != ' ')
	            {
	            	line.room.push_back(a[i]);
	            	i++;
				}
				i++;
	            while(i < a.size())
	            {
	            	line.subject.push_back(a[i]);
	            	i++;
				}
				i = 0;
				line.index = global_index;
				global_index++;
				add_el_to_maps(line);
				inf.push_back(line);
				line.date_time.clear();
				line.group.clear();
				line.room.clear();
				line.subject.clear();
				line.teacher.clear();
				
	        }
	    }
	    in.close();
		i=0;	
		cout << "This is BAZA" << endl;
	    while(i < inf.size())
	    {
 	    	inf[i].print();
	    	i++;
		}
		cout  << endl << "This is end of BAZA"  << endl;
	}
	/*
	~Database();
	*/
	vector <int> association(condition c)
	{
		int i;
		vector<int> it;
		vector<int> ig;
		vector<int> id;
		vector<int> ir;
		vector<int> is;
		vector<int> v;
		vector<int> vts;
		vector<int> vgr;
		vector<int> vdgr;
		map <string,vector<int> >:: iterator iter;
		int t = 0, g = 0, d = 0, r = 0, s = 0;
		string teacher,group_low,group_high,date_time_low,date_time_high,room_low,room_high,subject;
		for(i = 0;i < c.criteri.size();i++)
		{
			if(c.criteri[i].pole == "teacher")
			{
				teacher = c.criteri[i].val;
				t++;
			}
			else if(c.criteri[i].pole == "group")
			{
				group_low = c.criteri[i].low_val;
				group_high = c.criteri[i].high_val;
				g++;
			}
			else if(c.criteri[i].pole == "date_time")
			{
				date_time_low = c.criteri[i].low_val;
				date_time_high = c.criteri[i].high_val;
				d++;
			}
			else if(c.criteri[i].pole == "room")
			{
				room_low = c.criteri[i].low_val;
				room_high = c.criteri[i].high_val;
				r++;
			}
			else if(c.criteri[i].pole == "subject")
			{
				subject = c.criteri[i].val;
				s++;
			}
		}
		if(t == 0)
		{
			for(i=0;i<global_index;i++)
			{
				it.push_back(i);
			}
		}
		else
		{
			it = mp_t[teacher];
		}
		if(g == 0)
		{
			for(i=0;i<global_index;i++)
			{
				ig.push_back(i);
			}
		}
		else
		{
			for( iter = mp_g.begin(); iter != mp_g.end(); ++iter) 
			{
				if(more(group_low,iter->first) == iter->first && more(iter->first,group_high) == group_high)
				{				
					ig.insert(ig.end(), mp_g[iter->first].begin(), mp_g[iter->first].end());
				}
		 	}
		}
		if(d == 0)
		{
			for(i=0;i<global_index;i++)
			{
				id.push_back(i);
			}
		}
		else
		{
			for( iter = mp_d.begin(); iter != mp_d.end(); ++iter) 
			{
				if(more(date_time_low,iter->first) == iter->first && more(iter->first,date_time_high) == date_time_high)
				{				
					id.insert(id.end(), mp_d[iter->first].begin(), mp_d[iter->first].end());
				}
		 	}
		}
		if(r == 0)
		{
			for(i=0;i<global_index;i++)
			{
				ir.push_back(i);
			}
		}
		else
		{
			for( iter = mp_r.begin(); iter != mp_r.end(); ++iter) 
			{
				if(more(room_low,iter->first) == iter->first && more(iter->first,room_high) == room_high)
				{				
					ir.insert(ir.end(), mp_r[iter->first].begin(), mp_r[iter->first].end());
				}
		 	}
		}
		if(s == 0)
		{
			for(i=0;i<global_index;i++)
			{
				is.push_back(i);
			}
		}
		else
		{
			is = mp_s[subject];
		}
		set_intersection(it.begin(), it.end(),is.begin(), is.end(),back_inserter(vts));
		set_intersection(ig.begin(), ig.end(),ir.begin(), ir.end(),back_inserter(vgr));
		set_intersection(id.begin(), id.end(),vgr.begin(), vgr.end(),back_inserter(vdgr));
		set_intersection(vdgr.begin(), vdgr.end(),vts.begin(), vts.end(),back_inserter(v));
		return v;
	}
	vector <int> intersection(condition c)
	{
		int i = 0;
		vector<int> v;
		vector<int> vts;
		vector<int> vgr;
		vector<int> vdgr;
		string teacher,group,date_time,room,subject;
		while(i < c.criteri.size())
		{
			if(c.criteri[i].pole == "teacher")
			{
				teacher = c.criteri[i].val;
			}
			else if(c.criteri[i].pole == "group")
			{
				group = c.criteri[i].low_val;
			}
			else if(c.criteri[i].pole == "date_time")
			{
				date_time = c.criteri[i].low_val;
			}
			else if(c.criteri[i].pole == "room")
			{
				room = c.criteri[i].low_val;
			}
			else if(c.criteri[i].pole == "subject")
			{
				subject = c.criteri[i].val;
				
			}
			i++;
		}
		set_intersection(mp_t[teacher].begin(), mp_t[teacher].end(),mp_s[subject].begin(), mp_s[subject].end(),back_inserter(vts));
		set_intersection(mp_g[group].begin(), mp_g[group].end(),mp_r[room].begin(), mp_r[room].end(),back_inserter(vgr));
		set_intersection(mp_d[date_time].begin(), mp_d[date_time].end(),vgr.begin(), vgr.end(),back_inserter(vdgr));
		set_intersection(vdgr.begin(), vdgr.end(),vts.begin(), vts.end(),back_inserter(v));
		return v;
	}
	
	void save()
	{
		ofstream out;
		int i = 0;
	    out.open("Base_for_out.txt");
	    if (out.is_open())
	    {
	    	while(i < inf.size())
	    	{
	    		out << inf[i].teacher << " " << inf[i].group << " " << inf[i].date_time << " ";
	    		out << inf[i].room << " " << inf[i].subject << endl;
	    		i++;
			}
	    }
	    out.close();
	    cout << endl << "FILE is SAVE" << endl;
	}
	void add_elem(const info& stroka)
	{
		add_el_to_maps(stroka);
		inf.push_back(stroka);
	}
	void delete_elem(int i)
	{
		del_el_from_maps(inf[i]);
		inf.erase(inf.begin() + i);
		vector<info>(inf).swap(inf);
	}
	resultat resresult_of_zapros(const condition &one_zapr)
	{
		resultat otvet;
		if(one_zapr.comand == "save")
		{
			save();
			otvet.stroka = "Save IS OK";
		}
		else if(one_zapr.comand == "generate")
		{
			int number;
			info inf;
			srand(unsigned(time(0)));
			number = atoi(one_zapr.criteri[0].val.c_str());
			while(number !=0)
			{
				inf.teacher = random_name();
				inf.date_time = int_to_string(1 + rand() % 2400);
				inf.group = int_to_string(100 + (rand() % 500));
				inf.room = int_to_string(100 + (rand() % 900));
				inf.subject = random_name();
				inf.index = global_index;
	        	global_index++;
				add_elem(inf);	
				number--;
			}
			otvet.stroka = "GENERATE is OK";
		}
		else if(one_zapr.comand == "remove")
		{
			int i = 0, q = 0;
			vector<int> ind_for_del;
			ind_for_del = intersection(one_zapr);
			while(i < ind_for_del.size())
			{
				while(ind_for_del[i] != inf[q].index)
				{
					q++;
				}
				delete_elem(q);
				i++;
			}
			otvet.stroka = "ELEMENT YDALEN (Remove is ok)";
		}
		else if(one_zapr.comand == "insert")
		{
			info inf;
			int i = 0;
			while(i < one_zapr.criteri.size())
			{
				if(one_zapr.criteri[i].pole[0] == 't')
				{
					inf.teacher = one_zapr.criteri[i].val;
				}
				else if(one_zapr.criteri[i].pole[0] == 's')
				{
					inf.subject = one_zapr.criteri[i].val;
				}
				else if(one_zapr.criteri[i].pole[0] == 'g')
				{
					inf.group = one_zapr.criteri[i].low_val;
				}
				else if(one_zapr.criteri[i].pole[0] == 'd')
				{
					inf.date_time = one_zapr.criteri[i].low_val;
				}
				else if(one_zapr.criteri[i].pole[0] == 'r')
				{
					inf.room = one_zapr.criteri[i].low_val;
				}
				i++;
			}
			inf.index = global_index;
		    global_index++;
			add_elem(inf);
			otvet.stroka = "ELEMENT dobavlen";
		}
		else if(one_zapr.comand == "select")
		{
			int i ,q = 0;
			vector<int> ind_for_sel;
			ses.clear();
			ind_for_sel = association(one_zapr);
 			for(i = 0;i < ind_for_sel.size();i++)
			{
				while(ind_for_sel[i] != inf[q].index)
				{
					q++;
				}
				ses.push_back(inf[q]);
			}
			otvet.stroka = "SELECT is OK";
		}
		else if(one_zapr.comand == "reselect")
		{
			int i ,q, status;
			vector<int> ind_for_sel;
			ind_for_sel = association(one_zapr);
 			for(q = 0;q < ses.size();q++)
			{
				status = 0;
				for(i = 0;i < ind_for_sel.size();i++)
				{
					if(ind_for_sel[i] == ses[q].index)
					{
						status = 1;
						break;
					}
				}
				if(status == 0 )
				{
					ses.erase(ses.begin() + q);
					vector<info>(ses).swap(ses);
					q--;
				}
			}
			otvet.stroka = "RESELECT is OK";	
		}
		else if(one_zapr.comand == "print")
		{
			int i = 0, q = 0, keks = 0;
			proverka_insert_and_remove prov;
			prov.date_time = 0;
			prov.group = 0;
			prov.room = 0;
			prov.subject = 0;
			prov.teacher = 0;
			while(i < ses.size())
			{
				otvet.inf.push_back(ses[i]);
				i++;
			}
			while(q < one_zapr.criteri.size())
			{
				if(one_zapr.criteri[q].pole[0] == 't')
				{
					prov.teacher = 1;
					keks = 1;
				}
				else if(one_zapr.criteri[q].pole[0] == 'g')
				{
					prov.group = 1;
					keks = 1;
				}
				else if(one_zapr.criteri[q].pole[0] == 'd')
				{
					prov.date_time = 1;
					keks = 1;
				}
				else if(one_zapr.criteri[q].pole[0] == 's')
				{
					prov.subject = 1;
					keks = 1;
				}
				else if(one_zapr.criteri[q].pole[0] == 'r')
				{
					prov.room = 1;
					keks = 1;
				}
				q++;
			}
			if(keks == 1)
			{
				if(prov.date_time == 0)
				{
					i = 0;
					while(i < ses.size())
					{
						otvet.inf[i].date_time.clear();
						i++;
					}
				}
				if(prov.group == 0)
				{
					i = 0;
					while(i < ses.size())
					{
						otvet.inf[i].group.clear();
						i++;
					}
				}
				if(prov.room == 0)
				{
					i = 0;
					while(i < ses.size())
					{
						otvet.inf[i].room.clear();
						i++;
					}
				}
				if(prov.subject == 0)
				{
					i = 0;
					while(i < ses.size())
					{
						otvet.inf[i].subject.clear();
						i++;
					}
				}
				if(prov.teacher == 0)
				{
					i = 0;
					while(i < ses.size())
					{
						otvet.inf[i].teacher.clear();
						i++;
					}
				}
			}
		}
		return otvet;
	}
	Result process(Query &q)
	{
		Result res;
		resultat one_res;
		condition one_zapr;
		int i = 0;
		while(i < q.sizee())
		{
			one_zapr = q.elem(i);
			one_res = resresult_of_zapros(one_zapr);
			res.add_element(one_res);
			one_res.clear();
			i++;
		}
		return res;
	}
};



int main(void)
{
    string st, base = "Base.txt";
	condition s;
	Result res;
	getline(cin,st);
	try
	{
		Database baza(base);
		Query zapros(st);
		res = baza.process(zapros);
		res.print();
	}
	catch(MyException& my) 
	{
 		cerr << endl << "!!!! " << my.message() << " !!!!" << endl << "Code - " << my.code();
	}

	return 0;
}
