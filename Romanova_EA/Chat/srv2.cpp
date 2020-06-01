#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <list>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SV_PORT 25400

using namespace std;

int CreateSocket(int);
int Sock = 0;

#include "server.h"

class CUser
{
public:
    string Name;
    sockaddr_in IP;
};

std::list<CUser> Users;

sockaddr_in cli_addr;

#define IT list<CUser>::iterator

int main()
{
    printf("This is a udp chat server.\n");
    if ((Sock = CreateSocket(SV_PORT)) == -1) return 0; // создали новый сокет
    
    while (true)
    {	
	bool exitflag = false; // не хотим выйти
	string Buf;
	sockaddr_in address; //Заполняем адресную структуру и связываем сокет с любым адресом
	address.sin_family = AF_INET;//
	address.sin_addr.s_addr = INADDR_ANY;//
	address.sin_port = SV_PORT; 
	string Msg;
	string UserName;
	int MT = ExtractMsg(&Buf, &UserName, &Msg); //извлечь сообщение
	bool found = false; // не нашли
	IT it;
	for (it = Users.begin(); it != Users.end(); it++) // идем по всем юзерам
	    if (it->Name == UserName) 
                {   
                    found = true; 
                    break; 
                }
	if (!found) // если пришел новый
	{
	    cout << "New user came: " << UserName << "\n";
	    CUser u;
	    u.Name = UserName;
	    memmove(&u.IP, &address, sizeof(sockaddr_in)); // куда откуда сколько
	    Users.push_back(u);
	}
	string BBuf = " " + UserName + "#" + Msg; // то что он пишет
	
	switch (MT) // проверяем извлеченное сообщение на то какое оно
	{
	    case MT_KILL_TS: BBuf[0] = 'k'; exitflag = true; break; //если такое
	    case MT_QUIT_TS:
		BBuf[0] = 'q';
		for (it = Users.begin(); it != Users.end(); it++)
		{
		    if (it->Name == UserName)
		    {
			cout << "User " << UserName << "died.\n";
			Users.erase(it);
			break;
		    }
		}
		break;
	    case MT_MESSAGE: BBuf[0] = 'B'; break;
	}
	
	cli_addr.sin_family = AF_INET;
	
	for (it = Users.begin(); it != Users.end(); it++)
	{
	    cli_addr.sin_addr.s_addr = it->IP.sin_addr.s_addr;
	    cli_addr.sin_port = it->IP.sin_port;
	    cout << "Track: bbuf = " << BBuf << "\n"; //отслеживаем кто что делает
	}
	if (exitflag) break;
    }
    close(Sock);
    return 0;    
}

int CreateSocket(int port) // создает сокет
{
	sockaddr_in address; // структура с тремя параметрами(номер порта, IP адрес и вот это константное)
	int i = 1;
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
        if (sock == -1)              printf("I'm sorry, it's unable to create socket\n");
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = port; // наш порт
if (bind(sock, (sockaddr*)&address, sizeof(address)) == -1)
 { 
cout << "Error!!!!!\n";
 return -1; 
}

	return sock;
}

