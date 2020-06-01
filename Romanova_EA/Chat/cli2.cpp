#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SV_PORT 25400

using namespace std;

int CreateSocket(int);
int Sock = 0;

bool alive = true; // true если программа работает

int Port;

#include "server.h"

void * TODO (void *)
{
    while (alive & Sock != 0)
    {
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = Port; // server port
	string Buf = ""; 

	    string UserName;
	    string Msg;
	    int MT = ExtractMsg(&Buf, &UserName, &Msg);
	    switch (MT)
	    {
		case MT_BROADCAST:
		    cout << "Message from " << UserName << ": " << Msg << "\n";
		    break;
		case MT_QUIT_FS:
		    cout << "User " << UserName << " had left the chat.\n";
		    break;
		case MT_KILL_FS:
		    cout << "Angry user " << UserName << " killed the server. Press ENTER to exit.\n";
		    alive = false;
		    break;
	    }
    }
    return 0;    
}

string getString(const string& s)
{
    printf("%s", s.c_str());
    string m;
    while (alive)
    {
        char c = fgetc(stdin);
	if (c == '\n') break;
	m += c;
    }
    return m;
}

int main()
{
    Port = rand() & 0xFFFF;
    Sock = CreateSocket(Port);
    string name = getString("Enter nickname:\n");
    pthread_t tid; // многопоточность
    pthread_create(&tid, 0, &TODO, 0); //указатель на поток и функция которая бдет выполняться в новом потоке
    while (alive)
    {	
	bool exitflag = false;
	string msg = getString("");
	
	string SendBuf = "M" + name + "#" + msg;
	if (msg == "kill") { SendBuf[0] = 'K'; exitflag = true; }
	if (msg == "quit") { SendBuf[0] = 'Q'; exitflag = true; }
	
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = SV_PORT; // server port
	string Buf; 
	
	if (exitflag) break;
    }
    close(Sock);
    Sock = 0;
    alive = false;
    
    return 0;
}

int CreateSocket(int port)
{
    sockaddr_in address;
    int i = 1;
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == -1) printf("I'm sorry, it's unable to create UDP socket\n");
	
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = port; // our port

    bind(sock, (sockaddr*)&address, sizeof(address));
    return sock;
}

