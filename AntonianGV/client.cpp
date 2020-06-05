#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define  SERVER_PORT     5555
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          1024


int  writeToServer  (int fd);
int  readFromServer (int fd);


int  main (void)
{
//    int i;
    int err;
    int sock;
    struct sockaddr_in server_addr;
    struct hostent    *hostinfo;

    hostinfo = gethostbyname(SERVER_NAME);
    if ( hostinfo==NULL ) {
        fprintf (stderr, "Unknown host %s.\n",SERVER_NAME);
        exit (EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if ( sock<0 ) {
        perror ("Client: socket was not created");
        exit (EXIT_FAILURE);
    }


    err = connect (sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if ( err<0 ) {
        perror ("Client:  connect failure");
        exit (EXIT_FAILURE);
    }
    fprintf (stdout,"Connection is ready\n");

	fprintf(stdout,"Write info to see the list of possible requests\n");
    while(1) {
        if (writeToServer(sock)<0) break;
        if (readFromServer(sock)<0) break;
    }
    fprintf (stdout,"The end\n");



    close (sock);
    exit (EXIT_SUCCESS);
}



int  writeToServer (int fd)
{
    int   nbytes;
    char  buf[BUFLEN];

    fprintf(stdout,"Send to server > ");
	
    if (fgets(buf,BUFLEN,stdin)==nullptr) {
	printf("error\n");
    }
    buf[strlen(buf)-1] = 0;

    nbytes = write (fd,buf,strlen(buf)+1);
    if ( nbytes<0 ) { perror("write"); return -1; }
    if (strstr(buf,"stop")) return -1;
    return 0;
}


int  readFromServer (int fd)
{
    int   nbytes;
    char  *buf;
	read(fd, &nbytes, 4);
	
	buf = (char*)malloc(nbytes+2);
    nbytes = read(fd,buf,nbytes+2);
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("read"); 
        return -1;
    } else if ( nbytes==0 ) {
        // нет данных для чтения
        fprintf (stderr,"Client: no message\n");
    } else {
        // ответ успешно прочитан
        fprintf (stdout,"Server's replay:\n%s\n",buf);
		//fprintf (stdout,"%d\n",nbytes);
    }
	if (strncmp(buf,"Quit",4) == 0) {
		free (buf);
		return -1;
	}
	free (buf);
    return 0;
}

