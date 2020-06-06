#include <sstream>
#include <iostream>
#include <map>
#include<math.h>
#include<vector>

#define F_SIZE 10


enum {Play, Quit, Move, ShowMy, ShowOther, Ships};
enum {h, v};
enum {FREE, InGame};  
enum {miss, half, killed};
enum {OK, WS, OpM, SM ,SO, Q, NC, LOSE, MISS, KILL, HALF}; //ws - введите корабли, opm - ход противника, sm- мои шаги, so- шаги соперника, q- выход, nc - не команда, l- game over

using namespace std;

class Player;
class Game;

class Ship {
	private:
		int row_, col_;
		int len_;
		char* ori_;
		int* status_;
	public:
		Ship(int row, int col, string ori, int len)
		{
			if(!(0 <= row && row < F_SIZE) || !(0 <= col && col < F_SIZE) || (strcmp(ori.c_str(),"h") != 0 && strcmp(ori.c_str(),"v") != 0))
				perror("Coordinates is not correct");
			if((strcmp(ori.c_str(),"h") == 0 && row + len >= F_SIZE) || (strcmp(ori.c_str(),"v") == 0 && col + len >= F_SIZE))
				perror("Coordinates is not correct");
			row_ = row;
			col_ = col;
			ori_ = &ori[0];
			len_ = len;
			status_ = new int[len];
			status_ = {0};
		}
		bool wound(int row, int col)
		{
			if(strcmp(ori_,"h") == 0)
			{
				for(int i = 0; i < len_; i++)
				{
					if(row_ == row && i + col_ == col)
					{
						status_[i] = 1;
						return true;
					}
				}
			}
			if(strcmp(ori_,"v") == 0)
			{
				for(int i = 0; i < len_; i++)
				{
					if(col_ == col && i + row_ == row)
					{
						status_[i] = 1;
						return true;
					}
				}
			}
			return false;
		}
		bool kill() const
		{
			int i = 0;
			while(i < len_ && status_[i] == 1)
				i++;
			if(i == len_)
				return true;
			return false;
		}
		
		friend class Player;
};

class Player {
	private:
		Ship* ships_[10] = {NULL};
		vector<int> my_moves;
		int status_;
		int sock_;
		bool queue_;
		int numb_;
		int qs_; //количество оставшихся кораблей
		Game* game_;
	public:
	    
		Player(int sock) 
		{
			sock_ = sock;
			status_ = FREE;
			queue_ = true;
			qs_ = -1;
		}
        int analysis(char* buf, map<int, Player*> &Players);
		vector<int>* get_moves()
		{
			return &my_moves;
		}
		bool lose()
		{
			if(qs_ == 0)
				return true;
			return false;
		}
		void free()
		{
			this->status_ = FREE;
			game_ = NULL;
		}
		Player* search(map<int, Player*> &Players)
		{
			map <int, Player*> :: iterator it = Players.begin();
			while(it != Players.end())
			{
				if(it->second->status_ == FREE)
					return it->second;
				else it++;
			}
			if(it == Players.end())
				return NULL;
			return NULL;
		}
		int hit(int row, int col)
		{
			for(int i = 0; i < 10; i++)
			{
				if(this->ships_[i]->wound(row, col))
				{
					if(this->ships_[i]->kill())
					{
						qs_--;
						return killed;
					}
					else 
						return half;
				}
			}
			return miss;
		}
		bool ready()
		{
			if(ships_[0] != NULL)
				return true;
			return false;
		}
		int get_qs()
		{
			return qs_;
		}
		Game* get_game()
		{
			return (this->game_);
		}
		
	friend class Game;
};

class Game {
	private:
		Player* pl_1;
		Player* pl_2;
	public:
		Game(Player *pl1, Player *pl2)
		{
			pl_1 = pl1;
			pl_2 = pl2;
			pl_1->game_ = this;
			pl_2->game_ = this;
			pl_1->status_ = InGame;
			pl_2->status_ = InGame;
			pl_1->numb_ = 1;
			pl_2->numb_ = 2;
		}
		void change_q()
		{
			this->pl_1->queue_ = !(pl_1->queue_);
			this->pl_2->queue_ = !(pl_2->queue_);
		}
		int who()
		{
			if(this->pl_1->queue_)
				return(pl_1->sock_);
			if(this->pl_2->queue_)
				return(pl_2->sock_);
		}
		int opponent(int sock)
		{
			if(pl_1->sock_ == sock)
				return pl_2->sock_;
			return pl_1->sock_;
		}
		
		friend class Player;
};





int Player::analysis(char* buf, map<int, Player*> &Players)
{
    Player *pl_2;
    string query;
    int row, col;
    string ori;
    string str = buf;
    if(strcmp(buf, "Quit") == 0)
        return Q;
    else if(strcmp(buf, "ShowMy") == 0)
        return SM;
    else if(strcmp(buf, "ShowOther") == 0)
        return SO;
    if(!(this->queue_))
        return OpM;
    else{
        if(strcmp(buf, "Play") == 0)
        {
            pl_2 = search(Players);  //если соперник найден - введите позиции кораблей
            if(pl_2 == NULL)
                return -1;
            else
            {
                game_ = new Game(this, pl_2);
                this->queue_ = true;
                pl_2->queue_ = false;
                return WS;
            }
        }
		else if(strstr(buf, "Ships") != NULL)
		{
			stringstream ss(str);
			getline(ss, query, ':');
			if(strcmp(query.c_str(), "Ships") != 0)
				return NC;
			else
			{
				for(int i = 0; i < 10; i++)
				{
					ss >> row >> col >> ori;
					ss.ignore();
					if(0 <= i && i < 4)
						ships_[i] = new Ship(row, col, ori, 1);
					else if(4 <= i && i< 7)
						ships_[i] = new Ship(row, col, ori, 2);
					else if(7 <= i && i < 9)
						ships_[i] = new Ship(row, col, ori, 3);
					else
						ships_[i] = new Ship(row, col, ori, 4);
				}
				qs_ = 10;
				this->game_->change_q();
				return OK;
			}
		}
        else if(strstr(buf, "Move") != NULL)
        {
            stringstream ss(str);
            getline(ss, query, ':');
            if(strcmp(query.c_str(), "Move") != 0)
                return NC;
            else
            {
                ss >> row >> col;
                if(this->numb_ == 1)
                {
                    if(this->game_->pl_2->hit(row, col) == miss)
                    {
                        this->game_->change_q();
                        this->my_moves.push_back(row*10 + col);
                        if(this->game_->pl_2->lose())
                            return LOSE;
                        return MISS;
                    }
					else if(this->game_->pl_2->hit(row, col) == killed)
						return KILL;
					else 
						return HALF;
                }
                if(this->numb_ == 2)
                {
                    if(this->game_->pl_1->hit(row, col) == miss)
                    {
                        this->game_->change_q();
                        this->my_moves.push_back(row*10 + col);
                        if(this->game_->pl_1->lose())
                            return MISS;
                        else if(this->game_->pl_2->hit(row, col) == killed)
							return KILL;
						else 
							return HALF;
                    }
                }
            }
        }
    }
    return NC;
}

