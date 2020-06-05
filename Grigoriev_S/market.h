

#ifndef MARKET_H_
#define MARKET_H_

#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include "utility.h"
#include "RBTree.h"

extern std::ofstream logfile;

enum ORDER_TYPE
{
	REMOVE_TYPE = 0,
	BUY_MARKET = 1,
	BUY_BOUNDED = 2,
	SELL_MARKET = -1,
	SELL_BOUNDED = -2
};

struct order
{
	unsigned int order_id;
	unsigned int player_id;
	ORDER_TYPE type;
	unsigned int remaining_amount;
	unsigned int total_amount;
	unsigned int price;
	order();
	order(unsigned int o_id, unsigned int p_id, ORDER_TYPE t, unsigned int rem = 0, unsigned int total = 0, unsigned int p = 0);
	order(const order& o);
	order& operator=(const order& o);
	static const char* stringType(ORDER_TYPE t)
	{
		switch(t)
		{
			case 0: return "REMOVE_TYPE";
			case 1: return "BUY_MARKET";
			case 2: return "BUY_BOUNDED";
			case -1: return "SELL_MARKET";
			case -2: return "SELL_BOUNDED";
			default: return "NOT_A_TYPE";
		}
	}
};

class market;
class market_sf;

class player
{
public:
	player();
	virtual int playerSetup(int id, market_sf* strfrnt);
	virtual int wakeUp() = 0;
	virtual ~player();
protected:
	int _id;
	market_sf* _strfrnt;

	player(const player& cpy);
	const player& operator=(const player& cpy);
};



class market
{

friend market_sf;

public:
	market();
	int addPlayer(player* p, unsigned int sshares, unsigned int smoney);
	int deletePlayer(unsigned int id);
	int morning();
	int evening();
	~market();

	sqlite3* rodb();
private:
	int makeTrades();
	unsigned int generatePlayerId();
	int returnPlayerId(unsigned int id);
	unsigned int generateOrderId();
	int returnOrderId(unsigned int id);
	market(const market& cpy);
	const market& operator=(const market& cpy);
	sqlite3* _db;
	sqlite3* _rodb;
	RBTree<unsigned int, pair<player*, market_sf*>> _playersTree;
	listlim<pair<unsigned int, unsigned int>> p_id;
	listlim<pair<unsigned int, unsigned int>> o_id;
	unsigned int _playercount = 0;
};



class market_sf
{

friend class market;

public:
	market_sf(market* m, player* p, unsigned int player_id);
	const unsigned int* accStatus();
	int putInQueue(ORDER_TYPE type, unsigned int total_amount, unsigned int price);
	int removeOrder(unsigned int order_id);
	~market_sf();

	sqlite3* rodb();
private:
	player* _player;
	market* _mkt;
	unsigned int* _account_status = nullptr;
	unsigned int _id;
	queuelim<order>* _queue;
	void updateStatus();
	void clearStatus();
	int flush();
	market_sf(const market_sf& cpy);
	const market_sf& operator=(const market_sf& cpy);
};



class the_matrix
{
public:
	the_matrix();
	int addPlayer(player* p, int sshares, int smoney);
	int step();
	~the_matrix();

	sqlite3* rodb();
	//const price_graph_node* const _mprice_graph;
private:
	market* _market;
	sqlite3* _rodb;
	the_matrix(const the_matrix& cpy);
	const the_matrix& operator=(const the_matrix& cpy);
};



int callback_array_int(void* v, int colc, char** col, char** coln);

int callback_print(void* v, int colc, char** col, char** coln);

unsigned int* get_account_status(market* mkt, int id);


#endif /* MARKET_H_ */
