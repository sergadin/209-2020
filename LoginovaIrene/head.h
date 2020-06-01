#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LEN 1234
#define PORT 4321
#define SERVER "127.0.0.1"

int read_socket(int fd, char *buf){
    int i, len, n;

    if (read(fd, &len, sizeof(int)) != (int)sizeof(int)){
        perror("cannot read len");
        return -1;
    }

    for (i = 0; len > 0; i += n, len -= n){
        n = read(fd, buf+i, len);
        if (n < 0){
            perror("reading error");
            return -1;
        }
        else if(n == 0){
            perror("read truncated");
            return -2;
        }
    }

    if (i == 0)
        return -1;

    return 0;
}

int write_socket(int fd, char *buf){
    int len, i, n;

    len = strlen(buf) + 1;
    if (write(fd, &len, sizeof(int)) != (int)sizeof(int)){
        perror("Cannot send len\n");
        return -1;
    }

    for (i = 0; len > 0; i += n, len -= n){
        n = write(fd, buf+i, len);
        if (n < 0){
            perror("write error");
            return -1;
        }
        else if(n == 0){
            perror("write truncated\n");
            return -2;
        }
    }

    return 0;
}
