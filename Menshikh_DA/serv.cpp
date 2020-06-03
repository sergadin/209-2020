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
#include <vector>
#include "dataGenerator.h"
#include "database.h"
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
   cout<<bufsize<<endl;
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
      //  fprintf(stdout,"Server got message: %s\n",buf);
      string s(buf, sizeof(buf));
      str = s;
      cout<<"Server got message:"<<s<<endl;
      //cout << str.size();
      return 0;
   }
}

void  writeToClient (int fd, char *buf)
{
   int  nbytes;
   //unsigned char *s;

   // for (s=(unsigned char*)buf; *s; s++) *s = toupper(*s);
   nbytes = write(fd,buf,strlen(buf));
   fprintf(stdout,"Write back: %s\nnbytes=%d\n",buf,nbytes);

   if ( nbytes<0 ) {
      perror ("Server: write failure");
   }
}

// Определимся с номером порта и другими константами.
#define PORT    5555
#define BUFLEN  512
#define ITEM "ii.txt"
#define RECIPE "rr.txt"

// Две вспомогательные функции для чтения/записи (см. ниже)

void writeClient(int fd, string str)
{
   char *chrstr = new char[str.length() + 1];
   int  zplen  =str.length();
   strcpy(chrstr, str.c_str());

   send(fd, &zplen,sizeof(int),MSG_WAITALL);
   send(fd, chrstr, str.length(), MSG_WAITALL);
   fprintf(stdout,"Write back: %s\nnbytes=%d\n",chrstr,zplen);
   delete [] chrstr;
}

void writeClientData(int fd,/* Database x,*/int flag){
   //x.Database_to_file("_helpI.txt","_helpR.txt");
   vector<string> s;
   if(flag==0){
      ifstream fan;
      fan.open("_helpI.txt");
      string str;
      s.push_back("*********************************");
      s.push_back("*             ITEMS             *");
      s.push_back("*********************************");
      while(getline(fan,str))
	 s.push_back(str);

      int a=s.size();
      send(fd,&a,4,MSG_WAITALL);
      for(int i=0;i<s.size();i++)
	 writeClient(fd,s[i]);
      fan.close();
   }
   if(flag==1){
      ifstream fan;
      fan.open("_helpR.txt");
      string str;
      while(getline(fan,str))
	 s.push_back(str);
      s.push_back("*********************************");
      s.push_back("*            RECIPES            *");
      s.push_back("*********************************");
      int a=s.size();
      send(fd,&a,4,MSG_WAITALL);
      for(int i=0;i<s.size();i++)
	 writeClient(fd,s[i]);
      fan.close();
   }
   if(flag==2){
      ifstream fan;
      fan.open("_helpI.txt");
      string str;
      s.push_back("*********************************");
      s.push_back("*             ITEMS             *");
      s.push_back("*********************************");
      while(getline(fan,str))
	 s.push_back(str);
      fan.close();
      s.push_back("*********************************");
      s.push_back("*            RECIPES            *");
      s.push_back("*********************************");
      fan.open("_helpR.txt");
      while(getline(fan,str))
	 s.push_back(str);
      int a=s.size();
      cout<<a<<endl;
      // x.print();
      send(fd,&a,4,MSG_WAITALL);
      for(int i=0;i<s.size();i++)
	 writeClient(fd,s[i]);
      fan.close();
   }
}

bool isInt(string s){
   auto it = s.begin();
   while (it!=s.end() && isdigit(*it)) ++it;
   return !s.empty() && it == s.end();
}

int  main (void)
{
   Database x(ITEM,RECIPE);
   int     i, err, opt=1;
   int     sock, new_sock;
   struct  sockaddr_in  addr;
   struct  sockaddr_in  client;
   string str;
   char    buf[BUFLEN];
   socklen_t  size=0;

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
   std::cout<<addr.sin_port <<std::endl;
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
		  err = readFromClient(act_set[i].fd, str);
		  cout<<err<<" "<<str<<endl;
		  if ( err<0 ) {
		     // ошибка или конец данных
		     printf("get stop\n");
		     close (act_set[i].fd);
		     if (i < num_set-1) {
			act_set[i] = act_set[num_set - 1];
			num_set--;
			i--;
		     }
		  } else {
		     // данные прочитаны нормально - отвечаем
		     stringstream ss(str);
		     string zapros;
		     ss >> zapros;
		     std::transform(zapros.begin(), zapros.end(), zapros.begin(), ::tolower);
		      if(zapros=="canmake"){//запрос canmake
			string name;
			string b;
			ss>>name>>b;
			if(b.size()==0 ||  !isInt(b))
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			int kolv=atoi(b.c_str());
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			auto ans=x.can_make(name,kolv);
			if(ans["status"]==-1){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"I can't find this detail");
			   cout<<"at fd"<<act_set[i].fd<<"no such detail"<<endl;
			}else if(ans["status"]==-2){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"It's simple detail");
			   cout<<"at fd"<<act_set[i].fd<<"simple detail"<<endl;
			}else if(ans["status"]==1){
			   int k=ans.size();
			   cout<<k<<endl;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"It can't be made");
			   for(auto j:ans)
			      if(j.first!="status"){
				 stringstream s;
				 s<<j.first<<", "<<j.second;
				 writeClient(act_set[i].fd,s.str());
			      }
			}else if(ans["status"]==0){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"It can be made");
			   cout<<"at fd"<<act_set[i].fd<<"all right"<<endl;
			}
		     }
		     else if(zapros=="hmake"){
			string name,b;
			ss>>name>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			auto ans=x.hm_make(name);
			if(ans["status"]==-1){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"I can't find this detail");
			   cout<<"at fd"<<act_set[i].fd<<"no such detail"<<endl;
			}else if(ans["status"]==-2){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"It's simple detail");
			   cout<<"at fd"<<act_set[i].fd<<"simple detail"<<endl;
			}else if(ans["status"]==1){
			   int k=ans.size()-1;			   
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   stringstream ss;
			   ss<<ans["*ans*"]<<" item's can be made";
			   writeClient(act_set[i].fd,ss.str());
			   for(auto j:ans)
			      if(j.first!="status" && j.first!="*ans*"){
				 stringstream s;
				 s<<j.first<<", "<<j.second;
				 writeClient(act_set[i].fd,s.str());
			      }
			}

		     }else if(zapros=="recipes"){
			string b;
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			   x.Database_to_file("_helpI.txt","_helpR.txt");
			   writeClientData(act_set[i].fd,1);
		     }else if(zapros=="items"){
			string b;
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			   x.Database_to_file("_helpI.txt","_helpR.txt");
			   writeClientData(act_set[i].fd,0);
			}		     
		    else if(zapros=="show"){
			string b;
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			   x.Database_to_file("_helpI.txt","_helpR.txt");
			   writeClientData(act_set[i].fd,2);

		     }else if(zapros=="make"){//запрос canmake
			string name;
			string b;
			ss>>name>>b;
			if(b.size()==0 ||  !isInt(b))
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			int kolv=atoi(b.c_str());
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			auto ans=x.can_make(name,kolv);
			if(ans["status"]==-1){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"I can't find this detail");
			   cout<<"at fd"<<act_set[i].fd<<"no such detail"<<endl;
			}else if(ans["status"]==-2){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"It's simple detail");
			   cout<<"at fd"<<act_set[i].fd<<"simple detail"<<endl;
			}else if(ans["status"]==1){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"I can't make it");
			   cout<<"at fd"<<act_set[i].fd<<"cant make"<<endl;
			}
			else{
			x.MakeDetail(name,kolv);
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Made");
			   cout<<"at fd"<<act_set[i].fd<<"can make"<<endl;
			}
		     }else if(zapros=="additem"){//запрос canmake
			string name;
			string b;
			ss>>name>>b;
			if(b.size()==0 ||  !isInt(b))
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			int kolv=atoi(b.c_str());
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			if(kolv<0){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			x.add_detail(name,kolv);
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Detail added");
		   }else if(zapros=="save"){
		           string b;
			   ss>>b;
			   if(b!="end")
			   {
			      int k=1;
			      send(act_set[i].fd, &k,4,MSG_WAITALL);
			      writeClient(act_set[i].fd,"Invalid sintaxis");
			      continue;
			   }
			   x.Database_to_file(ITEM,RECIPE);
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Data saved");
			}
			else if(zapros == "addrecipe")
                          {
			      LinkedList A;
                              string f;
                              ss>>f;
                              string dname = f;
cout<<f<<endl;
                              int flag=0;
                              while(ss>>f)
                              {
                                  if(f == "end")
                                  {
                                      flag = 1;
                                      break;
                                  }
                                  string a;
                                  int quantt;
                                  ss>>a;
                                  if(!isInt(a))
                                  {
                                      break;
                                  }
                                  quantt =  atoi(a.c_str());
				  line r;
				  r.name=f;
				  x.add_detail(f,0);
				  r.quant=quantt;
				  A.insert(r);
                              }
                              if(flag == 1)
                              {
                                  x.add_recipe(dname,A);
                                  int k=1;
                                  send(act_set[i].fd, &k,4,MSG_WAITALL);
                                  writeClient(act_set[i].fd,"The recipe was added successfully");
                                  continue;
                              }
                              int k=1;
                              send(act_set[i].fd, &k,4,MSG_WAITALL);
                              writeClient(act_set[i].fd,"Input error");
                              continue;
                          }
			else if(zapros=="deleteitem"){//запрос canmake
			string name;
			string b;
			ss>>name>>b;
			if(b.size()==0 ||  !isInt(b))
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			int kolv=atoi(b.c_str());
			ss>>b;
			if(b!="end")
			{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			if(kolv<0){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   continue;
			}
			auto ans=x.delete_detail(name,kolv);
			  if(ans["status"]==-1){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"I can't find this detail");
			   cout<<"at fd"<<act_set[i].fd<<"no such detail"<<endl;
			}else if(ans["status"]==-3){
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Invalid sintaxis");
			   cout<<"at fd"<<act_set[i].fd<<"simple detail"<<endl;
			}else{
			   int k=1;
			   send(act_set[i].fd, &k,4,MSG_WAITALL);
			   writeClient(act_set[i].fd,"Detail deleted");
			}
		   }
		     else if(zapros=="stop"){    // ошибка или конец данных
                              string check;
                              ss >> check;
                              if(check != "end")
                              {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Input error");
                                continue;
                              }                      
			printf("get stop\n");
			close (act_set[i].fd);
			if (i < num_set-1) {
			   act_set[i] = act_set[num_set - 1];
			   
			}
			num_set--;
			i--;
		     } 
		                  
			  else if(zapros == "exit1979")
                          {
                              string check;
                              ss >> check;
                              if(check != "end")
                              {
                                int k=1;
                                send(act_set[i].fd, &k,4,MSG_WAITALL);
                                writeClient(act_set[i].fd,"Input error");
                                continue;
                              }
                              cout << "Good, bye\n";
                              for (i=0; i<num_set; i++)
                              {
                                  close (act_set[i].fd);
                              }
                              close(sock);
                              return 0;
                          } 
                          else{
                          int k=1;
                          send(act_set[i].fd, &k,4,MSG_WAITALL);
                          writeClient(act_set[i].fd,"Input error");
                        }
         
	       }    
}
	    }  
	 }
      }
   }
}




