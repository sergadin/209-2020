#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define  SERVER_PORT     8766
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          1024
#define EXIT_FAILURE 1


int  writeTo(int fd);
char* readFrom(int fd);
void process(int fd, char* buf);

int  main (void)
{
    int err;
    int sock;
	char* read_data = NULL;
    struct sockaddr_in server_addr;
    struct hostent  *hostinfo;

    // Получаем информацию о сервере по его DNS имени
    // или точечной нотации IP адреса.
    hostinfo = gethostbyname(SERVER_NAME);
    if(hostinfo == NULL) 
	{
        fprintf (stderr, "Unknown host %s.\n",SERVER_NAME);
        exit (EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;
//for (i=0;i<100000;i++) {
    // Создаем TCP сокет.
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
	{
        perror ("Client: socket was not created");
        exit (EXIT_FAILURE);
    }
    err = connect (sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err < 0) 
	{
        perror ("Client:  connect failure");
        exit (EXIT_FAILURE);
    }
    fprintf(stdout,"Connection is ready\n");
    while(1) {
		process(sock, read_data);
        if (writeTo(sock) < 0) break;
		read_data = readFrom(sock);
        if (read_data == NULL) break;
    }
    fprintf (stdout,"The end\n");
    close (sock);
    exit (EXIT_SUCCESS);
}



int  writeTo(int fd)
{
    int   nbytes;
    char  buf[BUFLEN];
    if (fgets(buf,BUFLEN,stdin)==nullptr) {
	printf("error\n");
    }
    buf[strlen(buf)-1] = 0;

    nbytes = write (fd,buf,strlen(buf)+1);
    if ( nbytes<0 ) { perror("write"); return -1; }
    return 0;
}

void process(int fd, char* buf)
{
	int row, col;
	if(buf == NULL)
		printf("Enter a command");
	else if(isdigit(buf[0]) == 0)
	{
		for(int i =  0; i < (int)(strlen(buf)) + 1; i++)
		{
			row = buf[i]/10;
			col = buf[i]%10;
			printf("%d, %d\n", row, col);
		}
	}
	else if(strcmp(buf, "Quit") == 0)
	{
		printf("%s\n", buf);
		close(fd);
	}
	else if(strstr(buf, "Kill") || strstr(buf, "Half"))
	{
		printf("%s\n", buf);
		process(fd, readFrom(fd));
	} 
	else printf("%s\n", buf);	
}

char* readFrom(int fd)
{
    int   nbytes;
    char  buf[BUFLEN];
	char* ch;

    nbytes = read(fd, buf, BUFLEN);
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("read"); 
        return NULL;
    } else if ( nbytes==0 ) {
        fprintf (stderr,"Client: no message\n");
    } else {
        // ответ успешно прочитан
		ch = buf;
        return ch;
    }
    return NULL;
}


