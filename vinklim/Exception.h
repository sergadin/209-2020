#pragma once

#include <stdio.h>
#include <iostream>
#include <string>

namespace R2
{
	using namespace std;

	class Exception
	{
		string msg, source;
		int line;

	public:
		Exception(const string& msg_)
		{
			msg = msg_, line = -1;
		}

		Exception(const string& msg_, const string& source_, int line_)
		{
			msg = msg_, source = source_, line = line_;
		}

		Exception(const Exception& other)
		{
			msg = other.msg, source = other.source, line = other.line;
		}

		friend ostream& operator << (ostream& O, Exception& e1);

		inline string text() const
		{
			if (line == -1)
			{
				return "Error: " + msg;
			}
			else
			{
				char buf[64];
				sprintf(buf, "%d", line);
				return "Error: " + msg + " @ " + source + ":" + string(buf);
			}
		}
	};

	ostream& operator << (ostream& O, Exception& e1)
	{
		O << e1.text();
	}
}