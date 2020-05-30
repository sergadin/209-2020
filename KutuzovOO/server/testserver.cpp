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
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#define PORT    5555
#define BUFLEN  512
using namespace std;

int  readFromClient (int fd, string &str)
{
    int  nbytes;
    int bufsize;
    recv(fd,&bufsize,4,MSG_WAITALL);
    char buf[bufsize];
    nbytes = recv(fd,buf,bufsize,MSG_WAITALL);
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
        string s(buf, sizeof(buf));
        str = s;
        return 0;
    }
}




int  main (void)
{

    int     i, err, opt=1;
    int     sock, new_sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    char    buf[BUFLEN];
    socklen_t  size;
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if ( sock<0 ) {
        perror ("Server: cannot create socket");
        exit (EXIT_FAILURE);
    }
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if ( err<0 ) {
        perror ("Server: cannot bind socket");
        exit (EXIT_FAILURE);
    }
    err = listen(sock,3);
    if ( err<0 ) {
        perror ("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }
    pollfd  act_set[100];
    act_set[0].fd = sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;
    while (1) {
        // Проверим, не появились ли данные в каком-либо сокете.
        // В нашем варианте ждем до фактического появления данных.
        int ret = poll (act_set, num_set, -1);
        if (ret < 0) {
            perror("Server: poll  failure");
            exit (EXIT_FAILURE);
        }
        if (ret > 0) {
           for (i=0; i<num_set; i++) {
              if (act_set[i].revents & POLLIN) {
                  printf("get POLLIN at fd %d\n", act_set[i].fd);
                  act_set[i].revents &= ~POLLIN;
                  if (i==0) {
                      new_sock = accept(act_set[i].fd,(struct sockaddr*)&client,&size);
                      printf("new client at port %u\n", ntohs(client.sin_port));
                      if (num_set < 100) {
                         act_set[num_set].fd = new_sock;
                         act_set[num_set].events = POLLIN;
                         act_set[num_set].revents = 0;
                         num_set++;
                      } else {
                         printf("no more sockets for client");
                         close(new_sock);
                      }
                   } else {
                     string str;
                     err = readFromClient(act_set[i].fd,str);
                     cout << "im get :" << str<< endl;
                     if ( err<0 || str == "stop")  {
                         printf("get stop\n");
                         close (act_set[i].fd);
                         if (i < num_set-1) {
                            act_set[i] = act_set[num_set - 1];
                            num_set--;
                            i--;
                         }
                     } else {
                       char *chrstr = new char[str.length() + 1];
                       int  zplen  =str.length();
                       strcpy(chrstr, str.c_str());
                       send(act_set[i].fd, &zplen,4,0);
                       send(act_set[i].fd, chrstr, str.length() + 1,0);
                       delete [] chrstr;
                     }

                 }
             }
          }
       }
   }
}
