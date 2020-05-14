#pragma once

#include <string>
#include<vector>

namespace R2
{
	using namespace std;
	class RecordSet
	{
	public:
		typedef vector<string> Row;
	private:
		vector<Row> rows;
		inline const vector<Row>& _row_ref(size_t i) const
		{
			return rows[i];
		}
	public:
		inline size_t length() const
		{
			return rows.size();
		}
		inline vector<Row> row(size_t i) const
		{
			return vector<Row>(_row_ref(i));
		}
		inline string getItemStr(size_t i, size_t j) const
		{
			return _row_ref(i)[j];
		}
	};

	class Client
	{
	public:
		RecordSet query() const;
	};
}