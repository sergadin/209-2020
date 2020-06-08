#pragma once

#include <string>
#include <vector>

namespace R2 
{
	using namespace std;

	class RecordSet
	{
	public:
		typedef vector<string> Row;

	private:
		mutable vector<Row> rows;

		inline const Row& _row_ref(size_t i) const
		{
			return rows[i];
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

		inline void parse(const string& text) const
		{
			auto lines = split(text, '\n');
			for (const auto& line : lines)
			{
				rows.push_back(split(line, '\t'));
			}
		}

	public:
		inline RecordSet()
		{
		}

		inline RecordSet(const RecordSet& other)
		{
			rows = other.rows;
		}

		inline RecordSet(const Message& msg)
		{
			string text((const char*)msg.binary());
			parse(text);
		}

		inline RecordSet(const string& text)
		{
			parse(text);
		}

		inline size_t length() const 
		{
			auto val = rows.size();
			return val;
		}

		inline Row row(size_t i) const
		{
			return Row(_row_ref(i));
		}

		inline string getItemStr(size_t i, size_t j) const
		{
			return _row_ref(i)[j];
		}

		inline string serialize() const
		{
			string s = "";
			for (int i = 0; i < length(); ++i)
			{
				if (i > 0)
					s += "\n";
				string line = "";
				for (int j = 0; j < _row_ref(i).size(); ++j)
				{
					if (j > 0)
						line += "\t";
					line += _row_ref(i)[j];
				}
				s += line;
			}
			return s;
		}

		inline const RecordSet& pushRow(const Row& row) const
		{
			rows.push_back(row);
			return *this;
		}

		inline const RecordSet& pushValue(const string& v1) const
		{
			Row row;
			row.push_back(v1);
			rows.push_back(row);
			return *this;
		}

		inline const RecordSet& pushValues(const string& v1, const string& v2) const
		{
			Row row;
			row.push_back(v1);
			row.push_back(v2);
			rows.push_back(row);
			return *this;
		}

		inline const RecordSet& pushValues(const string& v1, const string& v2, const string& v3) const
		{
			Row row;
			row.push_back(v1);
			row.push_back(v2);
			row.push_back(v3);
			rows.push_back(row);
			return *this;
		}

		inline const RecordSet& pushValues(const string& v1, const string& v2, const string& v3, const string& v4) const
		{
			Row row;
			row.push_back(v1);
			row.push_back(v2);
			row.push_back(v3);
			row.push_back(v4);
			rows.push_back(row);
			return *this;
		}

		inline const RecordSet& pushValues(const string& v1, const string& v2, const string& v3, 
			const string& v4, const string& v5) const
		{
			Row row;
			row.push_back(v1);
			row.push_back(v2);
			row.push_back(v3);
			row.push_back(v4);
			row.push_back(v5);
			rows.push_back(row);
			return *this;
		}
	};
}