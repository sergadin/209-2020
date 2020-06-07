#include <string>
#include <iostream>
#include "Client.h"

void initialUpload(R2::Client& client)
{
	if (true)
	{
		client.query("insert teacher name=Ivanov title=prof");
		client.query("insert teacher name=Petrov title=prof");
		client.query("insert teacher name=Stucer title=docent");
		client.query("insert teacher name=Morskaja title=docent");
		client.query("insert course title=Calculus");
		client.query("insert course title=Geometry");
		client.query("insert course title=ODE");
		client.query("insert course title=Python");
		//
		client.query("insert group id=1 year=1");
		client.query("insert group id=2 year=1");
		client.query("insert group id=3 year=1");
		client.query("insert group id=4 year=2");
		client.query("insert group id=5 year=2");
		client.query("insert group id=6 year=2");
		client.query("insert group id=7 year=3");
		client.query("insert group id=8 year=3");
		client.query("insert group id=9 year=3");
		//
		// test
		//
		client.query("insert lecture room=1 day=MON t0=4 len=1 teacher=Ivanov group=7 course=Geometry");
		client.query("insert lecture room=1 day=THU t0=4 len=1 teacher=Ivanov group=8 course=Geometry");
		client.query("insert lecture room=1 day=FRI t0=5 len=2 teacher=Ivanov group=9 course=Geometry");
		client.query("insert lecture room=2 day=MON t0=0 len=2 teacher=Stucer group=1 course=Calculus");
		client.query("insert lecture room=2 day=TUE t0=1 len=2 teacher=Stucer group=2 course=Calculus");
		client.query("insert lecture room=2 day=THU t0=1 len=1 teacher=Stucer group=3 course=Calculus");
		client.query("insert lecture room=2 day=SAT t0=3 len=1 teacher=Stucer group=3 course=Calculus");
		client.query("insert lecture room=3 day=MON t0=2 len=1 teacher=Petrov group=4 course=ODE");
		client.query("insert lecture room=3 day=MON t0=3 len=1 teacher=Petrov group=5 course=ODE");
		client.query("insert lecture room=2 day=MON t0=4 len=1 teacher=Morskaja group=1 course=Python");
		client.query("insert lecture room=2 day=MON t0=6 len=1 teacher=Morskaja group=2 course=Python");
		client.query("insert lecture room=2 day=MON t0=7 len=1 teacher=Morskaja group=3 course=Python");
		client.query("insert lecture room=2 day=FRI t0=1 len=1 teacher=Morskaja group=4 course=Python");
		client.query("insert lecture room=2 day=FRI t0=2 len=1 teacher=Morskaja group=5 course=Python");
		client.query("insert lecture room=2 day=FRI t0=7 len=1 teacher=Morskaja group=6 course=Python");
		//
		client.query("save");
		//
		client.query("remove lecture day=MON t0=7 room=2");
	}
	else
	{
		client.query("read");
	}
	//
	// selecting sample data
	//
	client.query("select day day=MON room=2");
	client.query("select course_lectures course=Python");
	client.query("select group_lectures group=1");
	client.query("select group_lectures group=3");
	client.query("select teacher_lectures teacher=Stucer");
}

int main()
{
	using namespace std;
	using namespace R2;
	//
	try
	{
		Client client("127.0.0.1", 8888);
		//
		initialUpload(client);
		//
		while (true)
		{
			string q;
			cout << "query: ";
			getline(cin, q);
			if (q == "exit")
				break;
			// q = "insert lecture 1 0 4 1 0 0 0";
			auto rows = client.query(q);		
			//break;
		}
	}
	catch (Exception& e1)
	{
		cout << e1.text() << endl;
	}
	catch(...)
	{
		cout << "Unexpected error!" << endl;
	}
	//
	return 0;
}