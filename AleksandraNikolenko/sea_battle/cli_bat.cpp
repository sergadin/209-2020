#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#define  SERVER_PORT     7766
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

    pollfd  act_set[100];
    act_set[0].fd = 0; // stdin
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    act_set[1].fd = sock; // connection to server
    act_set[1].events = POLLIN;
    act_set[1].revents = 0;
    int num_set = 2;

    while(1) {
        int ret = poll(act_set, num_set, -1);
        if (ret < 0)
		{
            perror("poll  failure");
            exit (EXIT_FAILURE);
        }
        if (ret > 0)
		{
            if (act_set[0].revents & POLLIN) {
                // stdin
                act_set[0].revents &= ~POLLIN;
                if (writeTo(sock) < 0) break;
            }
            if (act_set[1].revents & POLLIN) {
                act_set[1].revents &= ~POLLIN;
                read_data = readFrom(sock);
                process(sock, read_data);
            }
        }
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
	printf("!!!writeTo!!!\n");
    return 0;
}

void process(int fd, char* buf)
{
	int row, col;
	if(isdigit(buf[0]))
	{
        printf("Got: %s\n", buf);
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
		printf("!!!process_q!!!\n");
		close(fd);
	}
	else if(strstr(buf, "Kill") || strstr(buf, "Half"))
	{
		printf("%s\n", buf);
		// printf("!!!process_K_H!!!\n");
		// process(fd, readFrom(fd));
	}
	else printf("%s\n", buf);	
}

char* readFrom(int fd)
{
    int   nbytes;
    int   message_length;
    char  buf[BUFLEN];
	char* ch;

    nbytes = read(fd, &message_length, sizeof(int));
    nbytes = read(fd, buf, message_length);
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("read");
        return NULL;
    } else if ( nbytes==0 ) {
        fprintf (stderr,"Client: no message\n");
    } else {
        // ответ успешно прочитан
        fprintf(stderr, "Client got: '%s'\n", buf);
		ch = buf;
        return ch;
    }
    return NULL;
}


