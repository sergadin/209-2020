#include "sqlite3.h"
#include "market.h"
#include <stdio.h>
#include <limits.h>

#define _DEBUG_
#include "RBTree.h"

std::ofstream logfile;

class testDummy: public player
{
	int wakeUp() override
	{
		int ret;
		char* err;
		static unsigned int count = 0;
		unsigned int *tstatus = nullptr;
		if(count > 0)
		{
			std::string expr("SELECT \"order_id\" FROM orderbook WHERE (\"player_id\" = ");
			expr += std::to_string(_id);
			expr += " )";
			ret = sqlite3_exec(_strfrnt->rodb(), expr.c_str(), callback_array_int, &tstatus, &err);
			logfile << "Player request order list id = "<< _id <<" : " << (err ? err : "no errors") << std::endl;
			if(SQLITE_OK != ret)
				throw sEx(-1, "Failed to execute a statement");
			if(tstatus)
			{
				_strfrnt->removeOrder(tstatus[1]);
				free(tstatus);
			}
			count--;
		}
		else if((_strfrnt->accStatus())[0]>=20)
			_strfrnt->putInQueue(ORDER_TYPE::BUY_BOUNDED, 10, 2);
		else
			count = 7;
		return 0;
	}
};

int isSqare(int n)
{
	for(int i = 0; i < n; i++)
	{
		if(i*i == n)
			return i;
		else if(i*i > n)
			return 0;
	}
	return 0;
}

int main()
{
	int a;
	RBTree<int, int> tree;
	try{
		for(int i = 0; i < 100; i++)
		{
			a = isSqare(i);
			if(a == 0)
				tree.put(i,i);
			else
				tree.remove(a);
			tree.debug();
			std::cout << std::endl << std::endl;
		}
		std::cout << std::endl;
	}
	catch(int e)
	{
		std::cout << e << std::endl;
	}
}

/*int main()
{
	char c;
	char* err;
	try
	{
		logfile.open("mmlog.txt");
		the_matrix* m = new the_matrix();
		m->addPlayer(new testDummy(), 100, 100);
		for ( ; c != 'q' ; )
		{
			m->step();
			sqlite3_exec(m->rodb(), "SELECT \"player_id\", \"money_available\", \"money_total\", \"shares_available\", \"shares_total\" FROM playerinfo", callback_print, nullptr, &err);
			logfile << "SELECT execution: " << (err ? err : "no errors") << std::endl;
			sqlite3_exec(m->rodb(), "SELECT \"player_id\", \"order_type\", \"amount_remaining\", \"amount_total\", \"price\" FROM orderbook", callback_print, nullptr, &err);
			logfile << "SELECT execution: " << (err ? err : "no errors") << std::endl;
			std::cin >> c;
		}
		delete m;
		logfile.close();
	}
	catch(sEx e)
	{
		std::cout << e.code << " " << e.msg << std::endl;
	}
	catch(char const* e)
	{
		std::cout << e << std::endl;
	}
	return 0;
}*/

