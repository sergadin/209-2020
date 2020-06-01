#include "head.h"

int main() {
    struct hostent *hostinfo;
    hostinfo = gethostbyname(SERVER);
    char buf[LEN], buf2[LEN];
    FILE *in = stdin;
/*
    if(argc!=2)
    {
        perror("Wrong input!!!\n");
        return 1;
    }

    in = fopen(argv[1], "r");
    if (!in) {
        perror("Cannot read file\n");
    }
*/
    if (!hostinfo) {
        perror("Cannot get host\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *(struct in_addr *)hostinfo->h_addr;

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror ("Cannot create socket\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect error\n");
        return -1;
    }
    fprintf(stdout, "Connected\n");

    int c;
    while (fgets(buf, LEN, in))
    {
        c=0;
        if (write_socket(sock, buf) < 0)   return -1;
        while(1)
        {
            read_socket(sock, buf2);
            if (strcmp(buf2, "end")== 0) break;
            else if (strcmp(buf2, "ent")==0){close(sock);fclose(in);return 0;}
            if(c)
            {
                if(c==1)printf("_\n");
                printf("%s\n",buf2);
            }
            c++;
        }
    }
    close(sock);
    return 0;
}

