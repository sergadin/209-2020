#include <iostream>
#include <map>
typedef enum {Play, Quit, Move, ShowMy, ShowOther} CommandType;
enum {FREE, InGame, LOSE} STATUS;
enum {miss, half, kill} HITS;
enum {WS, OpM, SM,SO, Q} RES; //ws - введите корабли, opm - ход противника, sm- мои шаги, so- шаги соперника, q- выход





class Player {
	private:
		Ship[10] ships_ = {NULL};
		vector<int> my_moves;
		int status_;
		int sock_;
		bool queue_;
		int numb_;
	public:
	    Game* game_;
		Player(int sock) 
		{
			sock_ = sock;
			status_ = FREE;
			queue_ = True;
		}
		int analisys(char* buf, map<int, Player*> &Players)
		{
			Player *pl_2;
			string query;
			int row, col;
			string str(buf);
			if(strcmp(buf, "Quit") == 0)
				return Q;
			else if((strcmp(buf, "ShowMy") == 0)
				return SM;
			else if((strcmp(buf, "ShowOther") == 0)
				return SO;
			if(!(this->queue))
				return OpM;
			else{
				if(strcmp(buf, "Play") == 0)
				{
					pl_2 = search(map<int, Player*> &Players);  //если соперник найден - введите позиции кораблей
					if(pl_2 == NULL)
						return -1;
					else
					{
						game_ = new Game(this, pl_2);
						this->queue_ = True;
						pl_2->queue_ = False;
						return WS;
					}
				}
				
				else if(strstr(buf, "Move") != NULL)
				{
					stringstream ss(str);
					getline(ss, query, ':');
					if(strcmp(query, "Move") != 0)
							return -1;
					else
					{
						ss >> row >> col;
						if(this->numb_ = 1)
						{
							if(this->game->pl_2->hit(row, col) == miss)
							{
								this->game->change_q();
								this->my_moves.push_back(row*10 + col);
								return 0;
							}
							
						}
						if(this->numb_ = 2)
						{
							if(this->game->pl_1->hit(row, col) == miss)
							{
								this->game->change_q();
								this->my_moves.push_back(row*10 + col);
								return 0;
							}
						}
					}
				}
			}
			return -1;
		}
		&vector<int> get_moves()
		{
			return &my_moves;
		}
		&Player search(map<int, Player*> &Players)
		{
			map <int, Player*> :: iterator it = Players->begin();
			while(it != Players->end())
			{
				if(map[it]->status_ == FREE)
					return map[it];
				else it++;
			}
			if(it == Players->end())
				return NULL;
		}
		move()
		{
		}
}

class Game {
	private:
		Player *pl_1;
		Player *pl_2;
	public:
		Game(Player *pl1; Player *pl2)
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
			pl_1->queue_ = !(pl_1->queue_);
			pl_2->queue_ = !(pl_2->queue_);
		}
}































