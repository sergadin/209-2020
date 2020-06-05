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


#define PORT    8766
#define BUFLEN  1024
#define EXIT_FAILURE 1
#define (i min) i * 60 000

int   readFromClient(int fd, char *buf);
void  writeToClient (int fd, char *buf);


int  main (void)
{
	int     i, err, err2, opt = 1;
    int     ser_sock, sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client, client2;
    char    buf[BUFLEN], buf2[BUFLEN];
	socklen_t  size = 0, size2 = 0;
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
						 Players.insert(pair<int, Player*> (new_sock, &player));
                      } 
					  else 
					  {
                         printf("no more sockets for client");
                         close(new_sock);
                      }
                   } 
				   else 
				   {
                      err = readFromClient(act_set[i].fd, buf);
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
                          Players[act_set[i]]->analysis(buf, &Players);
                      }
                  }  
              }
           }
        }
    }
}



int  readFromClient (int fd, char *buf)
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



void  writeToClient (int fd, char *buf)
{
    int  nbytes;
    unsigned char *s;

    for (s=(unsigned char*)buf; *s; s++) *s = toupper(*s);
    nbytes = write(fd,buf,strlen(buf)+1);
    fprintf(stdout,"Write back: %s\nnbytes=%d\n",buf,nbytes);
    
    if ( nbytes<0 ) {
        perror ("Server: write failure");
    }
}


void game(int fd, chsr *buf)





















/*
if(err == 0 && strcmp(buf, "Play") == 0)   // если от клиента пришло сообщение: play, то называем его 1 игроком и ищем противника
					  {
						  player_1 = Player(act_set[i].fd];
						  ret = poll(act_set, num_set, (10 min));
						  if(ret < 0)
						  {
							  perror("Error: opponent is not founded"); //ошибка поиска противника
							  exit (EXIT_FAILURE);
						  }
						  else if(ret == 0)  //time out
						  {
							  writeToClient(player_1.sock_.fd, "error");
							  // xnj-nj tot
						  }
						  else(ret > 0)
						  {
							  if (act_set[0].revents & POLLIN) 
							  {
									printf("get POLLIN at fd %d\n", act_set[i].fd);
									act_set[0].revents &= ~POLLIN;  
									player_2 = Player(accept(act_set[0].fd,(struct sockaddr*)&client2,&size2)); //противник найден
									writeToClient(player_1.sock_.fd, "found");
									writeToClient(player_1.
									if (num_set < 100) 
									{
										 act_set[num_set].fd = player_2.sock_;
										 act_set[num_set].events = POLLIN;
										 act_set[num_set].revents = 0;
										 num_set++;
										
									} 
									else 
									{
										printf("no more sockets for client");
										close(player_2.sock_);
										writeToClient(player_1.sock_.fd, "error");
									}
							   }
						  }
					  }
	*/