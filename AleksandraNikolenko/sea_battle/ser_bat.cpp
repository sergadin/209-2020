#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>

#include "server.h"


#define PORT    8766
#define BUFLEN  1024
#define EXIT_FAILURE 1


int readFrom(int fd, char *buf);
void  writeTo(int fd, const char* buf);
char* mess(const char* ch, int c);
char* str_moves(vector<int> &v);
int coord(char *buf);
void processing(int res, int sock, map<int, Player*> &Players, char *buf);


int  main (void)
{
	int     i, err, opt=1, res;
    int     ser_sock, new_sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    char    buf[BUFLEN];
	Player* player;
    socklen_t  size = 0;
	int num_set = 1;
	map<int, Player*> Players;
	
	
    ser_sock = socket (PF_INET, SOCK_STREAM, 0);
    if (ser_sock < 0) 
	{
        perror ("Server: cannot create socket");
        exit (EXIT_FAILURE);
    }
    setsockopt(ser_sock, SOL_SOCKET, SO_REUSEADDR,(const void*)&opt,sizeof(opt));
	
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
    err = bind(ser_sock,(struct sockaddr*)&addr,sizeof(addr));
    if (err < 0)
	{
        perror ("Server: cannot bind socket");
        exit (EXIT_FAILURE);
    }
    err = listen(ser_sock, 3);
    if (err < 0) 
	{
        perror ("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }
	
    pollfd  act_set[100];   
    act_set[0].fd = ser_sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;

    while (1) 
	{
        int ret = poll(act_set, num_set, -1);
        if (ret < 0) 
		{
            perror("Server: poll  failure");
            exit (EXIT_FAILURE);
        }
        if (ret > 0) 
		{
           for (i=0; i<num_set; i++) 
		   {
              if (act_set[i].revents & POLLIN) 
			  {
                  printf("get POLLIN at fd %d\n", act_set[i].fd);
                  act_set[i].revents &= ~POLLIN;
                  if (i == 0) 
				  {
                      new_sock = accept(act_set[i].fd,(struct sockaddr*)&client,&size);
                      printf("new client at port %u\n", ntohs(client.sin_port));
                      if (num_set < 100) {
                         act_set[num_set].fd = new_sock;
                         act_set[num_set].events = POLLIN;
                         act_set[num_set].revents = 0;
                         num_set++;
						 player = new Player(new_sock);
						 Players.insert(pair<int, Player*> (new_sock, player));
                      } 
					  else 
					  {
                         printf("no more sockets for client");
                         close(new_sock);
                      }
                   } 
				   else 
				   {
                      err = readFrom(act_set[i].fd, buf);
                      if (err < 0) 
					  {          
                          close (act_set[i].fd);
                          if (i < num_set - 1) {
                             act_set[i] = act_set[num_set - 1];
                             num_set--;
                             i--;
                          }
                      } 
					  else 
					  {
                          res = Players[act_set[i].fd]->analysis(buf, Players);
						  processing(res, act_set[i].fd, &Players, buf);
						  
                      }
                  }  
              }
           }
        }
    }
} 


void processing(int res, int sock, map<int, Player*> &Players, char *buf)
{
	int k = Players[sock]->get_game()->opponent(sock);
	if(res == OK)
	{
		if(Players[k]->get_qs() == -1)
			writeTo(k, "Your ships");
		else writeTo(k, "Your move");
	}
	if(res == WS)
	{
		writeTo(sock, "Your ships");
	}
	if(res == OpM)
	{
		writeTo(sock, "Not your move");
	}
	if(res == SM)
	{
		char* str;
		vector<int>* moves = Players[sock]->get_moves();
		str = str_moves(&moves);
		writeTo(sock, str);
	}
	if(res == SO)
	{
		char* str;
		vector<int>* moves = Players[k]->get_moves();
		str = str_moves(&moves);
		writeTo(sock, str);
	}
	if(res == Q)
	{
		writeTo(sock, "Game over");
		writeTo(k, "Opponent is out of the game");
		Players[sock]->free();
		Players[k]->free;
	}
	if(res == NC)
	{
		writeTo(sock, "Not command");
	}
	if(res == LOSE)
	{
		writeTo(sock, "You lose");
		writeTo(k, "You win");
	}
	if(res == MISS)
	{
		int c = coord(buf);
		writeTo(sock, mess("Miss:", c));
		writeTo(k, mess("Miss:", c)); //сказать чей ход
	}
	if(res == KILL)
	{
		int c = coord(buf);
		mess("Kill:", c);
		writeTo(sock, mess("Kill:", c));
		writeTo(k, mess("Kill:", c)); //сказать чей ход
	}
	if(res == HALF)
	{
		int c = coord(buf);
		writeTo(sock, mess("Half:", c));
		writeTo(k, mess("Half:", c)); //сказать чей ход
	}
}

int  readFrom(int fd, char *buf)
{
    int  nbytes;

    nbytes = read(fd,buf,BUFLEN);
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("Server: read failure");
        return -1;
    } else if ( nbytes==0 ) {
        // больше нет данных
        return -1;
    } else {
        // есть данные
        fprintf(stdout,"Server got message: %s\n",buf);
        return 0;
    }
}



void  writeTo(int fd, const char* buf)
{
    int  nbytes;
    nbytes = write(fd, buf, strlen(buf)+1);
    if (nbytes < 0) {
        perror ("Server: write failure");
    }
}

int coord(char *buf)
{
	string str = buf;
	int a, b;
	stringstream ss(str);
	ss.ignore(4, ':');
	ss >> a >> b;
	return (a * 10 + b); 
}



char* str_moves(vector<int> &v)
{
	int a, b, i;
	string str;
	std::ostringstream ss;
	for(i = 0; i < v.size(); i++)
	{
		a = v[i]/10;
		b = v[i]%10;
		ss << a << b;
	}
	str = ss.str();
	return str.c_str();
}

char* mess(const char* ch, int c)
{
	string str;
	int a = c/10;
	int b = c%10;
	std::ostringstream ss;
	ss << ch;
	ss << a;
	ss << b;
	str = ss.str();
	return str.c_str();
}











