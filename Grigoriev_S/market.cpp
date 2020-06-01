/*
 * market.cpp
 *
 *  Created on: Mar 15, 2020
 *      Author: CerchilXIII
 */

#include "market.h"
#include <stdio.h>
#include <string>
#include <cstdio>

//order

order::order()
{

}

order::order(unsigned int o_id, unsigned int p_id, ORDER_TYPE t, unsigned int rem, unsigned int total, unsigned int p)
{
	order_id = o_id;
	player_id = p_id;
	type = t;
	remaining_amount = rem;
	total_amount = total;
	price = p;
}

order::order(const order& o)
{
	order_id = o.order_id;
	player_id = o.player_id;
	type = o.type;
	remaining_amount = o.remaining_amount;
	total_amount = o.total_amount;
	price = o.price;
}

order& order::operator=(const order& o)
{
	order_id = o.order_id;
	player_id = o.player_id;
	type = o.type;
	remaining_amount = o.remaining_amount;
	total_amount = o.total_amount;
	price = o.price;
	return *this;
}

//player

player::player()
{

}

player::player(const player& cpy)
{

}

const player& player::operator=(const player& cpy)
{
	return *this;
}

int player::playerSetup(int id, market_sf* strfrnt)
{
	_id = id;
	if(!strfrnt)
		throw sEx(-2, "player setup failed");
	else
	_strfrnt = strfrnt;
	return 0;
}

player::~player()
{

}

//market

market::market()
{
	int ret, code = 1;
	sqlite3_stmt* stmt, *rostmt;

	char* err;
	remove("mm.db");
	ret = sqlite3_open("mm.db", &_db);
	if(SQLITE_OK == ret)
	{
		ret = (sqlite3_open_v2("mm.db", &_rodb, SQLITE_OPEN_READONLY, nullptr));
		code = 2;
	}
	logfile << "database open: " << sqlite3_errmsg(_db) << std::endl << "readonly database open:  " << sqlite3_errmsg(_rodb)<< std::endl;
	if(SQLITE_OK != ret)
		throw sEx(code, "can't connect to a database");
	ret = sqlite3_exec(_db, "CREATE TABLE orderbook (order_id UNSIGNED INTEGER, player_id UNSIGNED INTEGER, order_type INTEGER, amount_remaining UNSIGNED INTEGER, amount_total UNSIGNED INTEGER, price UNSIGNED INTEGER)", nullptr, nullptr, &err);
	logfile << "Create table \"orderbook\": " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1,"Failed to execute a statement" );
	ret = sqlite3_exec(_db, "CREATE TABLE playerinfo (player_id UNSIGNED INTEGER, money_available UNSIGNED INTEGER, money_total UNSIGNED INTEGER, shares_available UNSIGNED INTEGER, shares_total UNSIGNED INTEGER)", nullptr, nullptr, &err);
	logfile << "Create table \"playerinfo\": " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1,"Failed to execute a statement" );
	ret = sqlite3_exec(_db, "CREATE TABLE price_graph (min_price UNSIGNED INTEGER, max_price UNSIGNED INTEGER, last_price UNSIGNED INTEGER)", nullptr, nullptr, &err);
	logfile << "Create table \"price_graph\": " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1,"Failed to execute a statement" );
	//
	//ret = sqlite3_exec(_db, "INSERT INTO playerinfo (player_id, money_available, money_total, shares_available, shares_total) VALUES (0,0,0,0,0)", nullptr, nullptr, &err);
	//logfile << "AAAAA: " << (err ? err : "no errors") << std::endl;
	//if(SQLITE_OK != ret)
	//	throw sEx(-1,"Failed to execute a statement" );
	p_id.addHead({0, -1u});
	std::cout << (*(p_id[0]))->_second << std::endl;
	o_id.addHead({0, -1u});
}

market::market(const market& cpy)
{

}

const market& market::operator=(const market& cpy)
{
	return *this;
}

int market::addPlayer(player* p, unsigned int sshares, unsigned int smoney)
{
	int ret;
	char* err;
	std::string expr;
	unsigned int id = generatePlayerId();
	market_sf* temp = new market_sf(this, p, id);
	_playersTree.put(id, {p, temp});
	p->playerSetup(id, temp);
	expr = "INSERT INTO playerinfo (player_id, money_available, money_total, shares_available, shares_total) VALUES (";
	expr += std::to_string(id);
	expr += ", ";
	expr += std::to_string(smoney);
	expr += ", ";
	expr += std::to_string(smoney);
	expr += ", ";
	expr += std::to_string(sshares);
	expr += ", ";
	expr += std::to_string(sshares);
	expr += ")";
	ret = sqlite3_exec(_db, expr.c_str() , nullptr, nullptr, &err);
	logfile << "Addplayer id = "<< id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	_playercount++;
	return id;
}

int market::deletePlayer(unsigned int id)
{
	int ret;
	char* err;
	unsigned int* arr = nullptr;
	std::string expr = "DELETE FROM playerinfo WHERE (\"player_id\" = ";
	expr += std::to_string(id);
	expr += ")";
	_playersTree.remove(id);
	_playercount--;
	ret = sqlite3_exec(_db, expr.c_str() , nullptr, nullptr, &err);
	logfile << "Delete player id = "<< id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	returnPlayerId(id);
	expr = "SELECT \"order_id\" FROM orderbook WHERE (\"player_id\" = ";
	expr += std::to_string(id);
	expr += ")";
	ret = sqlite3_exec(_db, expr.c_str(), callback_array_int, &arr, &err);
	logfile << "Selecting orders of players id = "<< id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	for(int i = 1; i < arr[0]; i++)
	{
		returnOrderId(arr[i]);
	}
	expr = "DELETE FROM orderbook WHERE (\"player_id\" = ";
	expr += std::to_string(id);
	expr += ")";
	ret = sqlite3_exec(_db, expr.c_str() , nullptr, nullptr, &err);
	logfile << "Delete orders of player id = "<< id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	return 0;
}

int market::morning()
{
	for(pair<player*, market_sf*>* i : _playersTree)
	{
		(i->_first)->wakeUp();
	}
	return 0;
}

int market::evening()
{
	for(pair<player*, market_sf*>* i : _playersTree)
	{
		(i->_second)->flush();
	}
	makeTrades();
	return 0;
}


market::~market()
{
	sqlite3_close(_db);
	logfile << "Close connection: " << sqlite3_errmsg(_db) << std::endl;
	sqlite3_close_v2(_rodb);
	logfile << "Close readonly connection: " << sqlite3_errmsg(_rodb) << std::endl;
}

sqlite3* market::rodb()
{
	return _rodb;
}

int market::makeTrades()
{
	return 0;
}

unsigned int market::generatePlayerId()
{
	unsigned int ret;
	listlim<pair<unsigned int, unsigned int>>::iterator i = p_id[0];
	if(!(*i))
		throw sEx(-78, "Out of player id's");
	ret = ((*i)->_first)++;
	if(((*i)->_second < -1u) && ((*i)->_first > (*i)->_second))
		throw sEx(-101, "Unexpected error");
	else if((*i)->_first == (*i)->_second)
		p_id.remove(i);
	return ret;
}

int market::returnPlayerId(unsigned int id)
{
	listlim<pair<unsigned int, unsigned int>>::iterator i = p_id[0];
	if(((*i)->_first > 0) && (id == (*i)->_first - 1))
	{
		(*i)->_first--;
		return 0;
	}
	else if(((*i)->_second < -1u) && (id == (*i)->_second + 1))
	{
		(*i)->_second++;
		return 0;
	}
	else if (((*i)->_first < id) && ((*i)->_second > id))
		throw sEx(-102, "Unexpected error");
	else
	{
		p_id.addHead({id, id});
		return 0;
	}
}

unsigned int market::generateOrderId()
{
	unsigned int ret;
	listlim<pair<unsigned int, unsigned int>>::iterator i = o_id[0];
	if(!(*i))
		throw sEx(-79, "Out of order id's");
	ret = ((*i)->_first)++;
	if(((*i)->_second < -1u) && ((*i)->_first > (*i)->_second + 1))
		throw sEx(-103, "Unexpected error");
	else if((*i)->_first == (*i)->_second + 1)
		o_id.remove(i);
	return ret;
}

int market::returnOrderId(unsigned int id)
{
	listlim<pair<unsigned int, unsigned int>>::iterator i = o_id[0];
	if(((*i)->_first > 0) && (id == (*i)->_first - 1))
	{
		(*i)->_first--;
		return 0;
	}
	else if(((*i)->_second < -1u) && (id == (*i)->_second + 1))
	{
		(*i)->_second++;
		return 0;
	}
	else if (((*i)->_first < id) && ((*i)->_second > id))
		throw sEx(-104, "Unexpected error");
	else
	{
		o_id.addHead({id, id});
		return 0;
	}
}

//market sf

market_sf::market_sf (market* m, player* p, unsigned int player_id)
{
	_mkt = m;
	_player = p;
	_id = player_id;
	if(!(_queue = new queuelim<order>))
		throw sEx(-3, "out of memory");
}

market_sf::market_sf(const market_sf& cpy)
{

}

const unsigned int* market_sf::accStatus()
{
	updateStatus();
	return _account_status;
}

const market_sf& market_sf::operator=(const market_sf& cpy)
{
	return *this;
}

int market_sf::putInQueue(ORDER_TYPE type, unsigned int total_amount, unsigned int price)
{
	order temp = order(_mkt->generateOrderId(), _id, type, total_amount, total_amount, price);
	int ret;
	updateStatus();
	switch(type)
	{
		case ORDER_TYPE::REMOVE_TYPE:
			return -1;
			break;
		case ORDER_TYPE::BUY_MARKET:

			break;
		case ORDER_TYPE::BUY_BOUNDED:
			if(total_amount * price > _account_status[0])
				ret = -1;
			else
			{
				_account_status[0] -= price * total_amount;
				ret = 0;
			}
			break;
		case ORDER_TYPE::SELL_MARKET:

			break;
		case ORDER_TYPE::SELL_BOUNDED:
			if(total_amount > _account_status[1])
				ret = -1;
			else
			{
				_account_status[1] -= temp.remaining_amount;
				ret = 0;
			}
			break;
		default:
			throw sEx(-99, "unexpected error");
			break;
	}
	if(!ret)
		_queue->push(temp);
	return ret;
}

int market_sf::removeOrder(unsigned int order_id)
{
	int ret;
	char* err;
	unsigned int* arr = nullptr;
	std::string expr = "SELECT \"player_id\", \"order_type\", \"amount_remaining\", \"amount_total\", \"price\" FROM orderbook WHERE (\"order_id\" = ";
	expr += std::to_string(order_id);
	expr += ")";
	ret = sqlite3_exec(_mkt->_rodb, expr.c_str(), callback_array_int, &arr, &err);
	logfile << "Player attempts to remove order id = "<< order_id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	if(arr[0] > 1)
		throw sEx(-9, "Multiple orders with the same id encountered");
	if((arr[0] == 0) || (arr[1] != _id))
		return -1;
	_queue->push(order(order_id, _id, ORDER_TYPE::REMOVE_TYPE));
	updateStatus();
	switch(arr[2])
	{
		case ORDER_TYPE::REMOVE_TYPE:
			throw sEx(-88, "REMOVE_TYPE order found in orderbook");
			break;
		case ORDER_TYPE::BUY_MARKET:

			break;
		case ORDER_TYPE::BUY_BOUNDED:
			_account_status[0] += arr[5] * arr[3];
			break;
		case ORDER_TYPE::SELL_MARKET:

			break;
		case ORDER_TYPE::SELL_BOUNDED:
			_account_status[1] += arr[3];
			break;
		default:
			throw sEx(-100, "unexpected error");
			break;
	}
	return 0;
}

market_sf::~market_sf()
{

}

sqlite3* market_sf::rodb()
{
	return _mkt->_rodb;
}

void market_sf::updateStatus()
{
	if(!_account_status)
	{
		_account_status = get_account_status(_mkt, _id);
	}
}

void market_sf::clearStatus()
{
	if(_account_status)
	{
		free(_account_status);
		_account_status = nullptr;
	}
}

int market_sf::flush()
{
	order otemp;
	std::string expr;
	int ret;
	char* err;
	updateStatus();
	for( ; 0 < _queue->size(); )
	{
		otemp = _queue->pop();
		if(otemp.type != ORDER_TYPE::REMOVE_TYPE)
		{
			expr = "INSERT INTO orderbook (order_id, player_id, order_type, amount_remaining, amount_total, price) VALUES (";
			expr += std::to_string(otemp.order_id);
			expr += ", ";
			expr += std::to_string(otemp.player_id);
			expr += ", ";
			expr += std::to_string(otemp.type);
			expr += ", ";
			expr += std::to_string(otemp.remaining_amount);
			expr += ", ";
			expr += std::to_string(otemp.total_amount);
			expr += ", ";
			expr += std::to_string(otemp.price);
			expr += ")";
			ret = sqlite3_exec(_mkt->_db, expr.c_str(), nullptr, nullptr, &err);
			logfile << "Order placed: id = "<< otemp.player_id <<" : "<< order::stringType(otemp.type)<< ", amount= "<< otemp.total_amount << ", price= " << otemp.price <<" : " << (err ? err : "no errors") << std::endl;
			if(SQLITE_OK != ret)
				throw sEx(-1, "Failed to execute a statement");
		}
		else
		{
			expr = "DELETE FROM orderbook WHERE (\"order_id\" = ";
			expr += std::to_string(otemp.order_id);
			expr += ")";
			ret = sqlite3_exec(_mkt->_db, expr.c_str() , nullptr, nullptr, &err);
			logfile << "Remove order id = "<< otemp.order_id <<" : " << (err ? err : "no errors") << std::endl;
			if(SQLITE_OK != ret)
				throw sEx(-1, "Failed to execute a statement");
		}
	}
	expr = "UPDATE playerinfo SET money_available = ";
	expr += std::to_string(_account_status[0]);
	expr += ", shares_available = ";
	expr += std::to_string(_account_status[1]);
	expr += " WHERE player_id = ";
	expr += std::to_string(_id);
	ret = sqlite3_exec(_mkt->_db, expr.c_str(), nullptr, nullptr, &err);
	logfile << "Playerinfo update: id = "<< otemp.player_id <<" : "<< order::stringType(otemp.type)<< ", amount= "<< otemp.total_amount << ", price= " << otemp.price <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	clearStatus();
	return 0;
}

//the matrix

the_matrix::the_matrix()
{
	_market = new market();
	_rodb = _market->rodb();
}

the_matrix::the_matrix(const the_matrix& cpy)
{

}

const the_matrix& the_matrix::operator=(const the_matrix& cpy)
{
	return *this;
}

int the_matrix::addPlayer(player* p, int sshares, int smoney)
{
	return _market->addPlayer(p,sshares, smoney);
}

int the_matrix::step()
{
	_market->morning();
	_market->evening();
	return 0;
}

the_matrix::~the_matrix()
{
	delete _market;
}

sqlite3* the_matrix::rodb()
{
	return _rodb;
}

//hobo

int callback_array_int(void* v, int colc, char** col, char** coln)
{
	unsigned int** array = (unsigned int**) v;
	if(*array == nullptr)
	{
		*array = (unsigned int*) malloc(sizeof(unsigned int) * (colc + 1));
		(*array)[0] = 0;
	}
	else
		*array = (unsigned int*) realloc(*array, sizeof(unsigned int) * (((*array)[0] + 1) * colc + 1));
	if(!(*array))
		throw sEx(-3, "out of memory");
	for (int i = 0; i < colc; i++)
	{
		(*array)[colc * (*array)[0] + 1 + i] = std::stoi(col[i]);
	}
	(*array)[0]++;
	return 0;
}

int callback_print(void* v, int colc, char** col, char** coln)
{
	for (int i = 0; i < colc; i++)
	{
		std::cout << col[i] << " || ";
	}
	std::cout << std::endl;
	return 0;
}

unsigned int* get_account_status(market* mkt, int id)
{
	int ret;
	char* err;
	unsigned int* state = (unsigned int*) malloc(sizeof(unsigned int) * 2), *tstatus = nullptr;
	std::string expr("SELECT \"money_available\", \"shares_available\" FROM playerinfo WHERE (\"player_id\" = ");
	expr += std::to_string(id);
	expr += " )";
	ret = sqlite3_exec(mkt->rodb(), expr.c_str(), callback_array_int, &tstatus, &err);
	logfile << "Player request available assets id = "<< id <<" : " << (err ? err : "no errors") << std::endl;
	if(SQLITE_OK != ret)
		throw sEx(-1, "Failed to execute a statement");
	if((!tstatus)||(tstatus[0] == 0))
		throw sEx(-10, ("No player with id = " + std::to_string(id)).c_str());
	if(tstatus[0] > 1)
		throw sEx(-11, "Multiple players with the same id encountered");
	state[0] = tstatus[1];
	state[1] = tstatus[2];
	free(tstatus);
	return state;
}


