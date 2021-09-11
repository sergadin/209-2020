#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

int main(int argc, char* argv[])
{
	char stroka[1024];
	struct sockaddr_in Socket_s;
	struct  sockaddr_in  Socket_cl;
	Vector_server My_server;
	
	int connection_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(connection_socket == -1)
	{
		perror("Socket creating error");
		return -1;
	}
	
	Socket_s.sin_port = htons(5555);
	Socket_s.sin_family = AF_INET;
	Socket_s.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	int on = 1;
	if(setsockopt(connection_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) 
	{
		perror("Calling setsockopt error");
	}
	
	if(bind(connection_socket, (struct sockaddr*) &Socket_s, sizeof(Socket_s))==-1)
	{
		perror("\nBinding socket error\n");
		return -1;
	}
	
	if(listen(connection_socket, 10) <0)
	{
		perror("\nListnening to socket error\n");
		return -1;
	}
	
	My_server.set_dim(atoi(argv[1]));
	My_server.set_port(Socket_s.sin_port);
	My_server.set_address(ntohl(Socket_s.sin_addr.s_addr));
	
	pollfd  act_set[100];   
    act_set[0].fd = connection_socket;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;
    
	int err=0;
	int i=0;
	int new_sock;
	socklen_t size =0;
	
	while(1)
	{
        if (poll(act_set, num_set, -1) < 0) 
		{
            perror("Server poll  error");
            return -1;
        }
        if (poll (act_set, num_set, -1) > 0) 
		{
           for (i=0; i<num_set; i++) 
		   {
              if (act_set[i].revents & POLLIN) 
			  {
                  printf("get POLLIN at fd %d\n", act_set[i].fd);
                  act_set[i].revents &= ~POLLIN;
                  if (i==0) 
				  {
                      new_sock = accept(act_set[i].fd, (struct sockaddr*)&Socket_cl, &size);
                      printf("\nnew client port %u\n", ntohs(Socket_cl.sin_port));
                      if (num_set < 100) 
					  {
                         act_set[num_set].fd = new_sock;
                         act_set[num_set].events = POLLIN;
                         act_set[num_set].revents = 0;
                         num_set++;
                      } 
					  else 
					  {
                        printf("\nno more sockets for client\n");
                        close(new_sock);
                      }
                   } 
				   else 
				   {
                      err = My_server.parsing(stroka, act_set[i].fd);
                      printf("%d [%s] %p\n", err, stroka, strstr(stroka, "stop"));
                      
					  if ( err<0 || strstr(stroka, "stop") ) 
					  {
                          close (act_set[i].fd);
                          if (i < num_set-1) 
						  {
                             act_set[i] = act_set[num_set - 1];
                             num_set--;
                             i--;
                          }
                      } 
					  else 
					  {  
                          My_server.WriteToClient(stroka, act_set[i].fd);
                      }
                  }  
              }
           }
        }
    }
	
	return 0;
}
