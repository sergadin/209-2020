/**
 *  Простейший пример TCP клиента и сервера.
 *
 *  Файлы    cli_tcp.c   ser_tcp.c
 *  автор    В.Д.Валединский
 *
 *  Клиент получает с клавиатуры текстовую строку и отсылает
 *  ее на сервер, читает ответ сервера, после чего заканчивает
 *  свою работу. Сервер ждет соединений от клиентов. При установленом
 *  соединении получает строку от клиента, переводит ее в верхний
 *  регистр и отсылает обратно клиенту. Для остановки сервера
 *  можно послать ему строку, состоящую из одного слова STOP.
 *
 *  Для ясности примеры составлены максимально просто и не анализируют
 *  некорректные ответы и запросы клиента и сервера, возможность переполнения
 *  буферов ввода вывода, неожиданное 'падение' сервера и т.п.
 *
 *  Компиляция:
 *      make tcp
 *  или
 *      gcc -W -Wall cli_tcp.c -o cli_tcp
 *      gcc -W -Wall ser_tcp.c -o ser_tcp
 *
 *  Запуск:
 *      В отдельных консолях (терминалах) запускается один сервер
 *          ./ser_tcp
 *      и несколько клиентов
 *          ./cli_tcp
 *
 */


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


// Определимся с номером порта и другими константами.
#define PORT    5555
#define BUFLEN  512

// Две вспомогательные функции для чтения/записи (см. ниже)
int   readFromClient(int fd, char *buf);
void  writeToClient (int fd, char *buf);


int  main (void)
{

    int     i, err, opt=1;
    int     sock, new_sock;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    char    buf[BUFLEN];
    socklen_t  size;

    // Создаем TCP сокет для приема запросов на соединение
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if ( sock<0 ) {
        perror ("Server: cannot create socket");
        exit (EXIT_FAILURE);
    }
    // опция чтобы не ждать при повторном запуске сервера
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void*)&opt,sizeof(opt));

    // Заполняем адресную структуру и
    // связываем сокет с любым адресом
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
    if ( err<0 ) {
        perror ("Server: cannot bind socket");
        exit (EXIT_FAILURE);
    }

    // Создаем очередь на 3 входящих запроса соединения
    err = listen(sock,3);
    if ( err<0 ) {
        perror ("Server: listen queue failure");
        exit(EXIT_FAILURE);
    }

    // Подготавливаем множества дескрипторов каналов ввода-вывода.
    // Для простоты не вычисляем максимальное значение дескриптора,
    // а далее будем проверять все дескрипторы вплоть до максимально
    // возможного значения FD_SETSIZE.
    pollfd  act_set[100];   
    act_set[0].fd = sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;



    // Основной бесконечный цикл проверки состояния сокетов
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
                      // пришли данные в уже существующем соединени
                      err = readFromClient(act_set[i].fd, buf);
                      printf("%d [%s] %p\n",err,buf, strstr(buf,"stop"));
                      if ( err<0 || strstr(buf,"stop") ) {
                          // ошибка или конец данных
                          close (act_set[i].fd);
                          if (i < num_set-1) {
                             act_set[i] = act_set[num_set - 1];
                             num_set--;
                             i--;
                          }
                      } else {
                          // данные прочитаны нормально - отвечаем
                          writeToClient(act_set[i].fd,buf);
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

