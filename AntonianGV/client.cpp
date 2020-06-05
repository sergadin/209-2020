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
//int flagg (int, int);
struct table {
	int paragraph;
	int numbers;
	int number;
	char* result;
};
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
	struct table * tabliza;
	int size;
	int i;
	int depth;
    fprintf(stdout,"Send to server > ");
	
    if (fgets(buf,BUFLEN,stdin)==nullptr) {
	printf("error\n");
    }
    buf[strlen(buf)-1] = 0;

    nbytes = write (fd,buf,strlen(buf)+1);
    if ( nbytes<0 ) { perror("write"); return -1; }
    if (strncmp(buf,"stop",4) == 0) return -1;
	if (strncmp(buf,"search",6) == 0) {
		read (fd, &size, 4);
		if (size == 0) return 0;
		read (fd, &depth, 4);
		if (depth > 1024) depth = 1024;
		tabliza = (struct table *)malloc(sizeof(struct table));
		for (i = 0; i < size; i++) {
			tabliza[i].result = (char*)malloc(depth+1);
			read (fd, &((tabliza[i]).paragraph), 4);
			read (fd, &((tabliza[i]).numbers), 4);
			read (fd, &((tabliza[i]).number), 4);
			read (fd, (tabliza[i]).result, depth+1);
			fprintf (stdout, "%d %d %d %s\n",(tabliza[i]).paragraph, (tabliza[i]).numbers, (tabliza[i]).number, (tabliza[i]).result);
		}
		return 0;
	}
    return 0;
}
/*int flagg (int i, int N) {
	if (i==N) return 0;
	return 1;
}*/

int  readFromServer (int fd)
{
    int   nbytes;
	int blocks;
	int i;
    char  buf[1025];
	char *ans;
	int proverka;
	read(fd, &blocks, 4);
	proverka = blocks;
	ans = (char*)malloc(1024*blocks);
	for (i = 0; i < blocks; i++) {
		nbytes = read(fd,buf,1025);
		strcpy (&ans[1024*i],buf);
	}
    if ( nbytes<0 ) {
        // ошибка чтения
        perror ("read"); 
        return -1;
    } else if ( nbytes==0 ) {
        // нет данных для чтения
        fprintf (stderr,"Client: no message\n");
    } else {
        // ответ успешно прочитан
        fprintf (stdout,"Server's replay:\n%s\n",ans);
		//fprintf (stdout, "Proverka = %d\n",proverka);
		//fprintf (stdout,"%d\n",nbytes);
    }
	if (strncmp(ans,"Quit",4) == 0) {
		free (ans);
		return -1;
	}
	bzero (buf, sizeof(buf));
	bzero (ans, sizeof(ans));
	free (ans);
    return 0;
}

