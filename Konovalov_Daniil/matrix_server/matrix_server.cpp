#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <netdb.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "Matrix_.h"

using namespace std;

int parsing(char* stroka, int id, vector <Matrix> Mat_vect);
int message_to_client(char* stroka, int id);


int main()
{
	vector <Matrix> Matrici;
	char stroka[1024];
	struct sockaddr_in Socket_s;
	struct  sockaddr_in  Socket_cl;
	
	int connection_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(connection_socket == -1)
	{
		perror("Socket creating error");
		return -1;
	}
	
	Socket_s.sin_port = htons(5555);
	Socket_s.sin_family = AF_INET;
	Socket_s.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int on = 1;
	if(setsockopt(connection_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1) 
	{
		perror("Calling setsockopt error");
	}
	
	if(bind(connection_socket, (struct sockaddr*) &Socket_s, sizeof(Socket_s))==-1)
	{
		perror("\nBinding socket error\n");
		return -1;
	}
	
	if(listen(connection_socket, 10) <0)
	{
		perror("\nListnening to socket error\n");
		return -1;
	}
	
	pollfd  act_set[100];   
    act_set[0].fd = connection_socket;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;
    
	int err=0;
	int i=0;
	int new_sock;
	socklen_t size=0;
	
	while(1)
	{
        if (poll(act_set, num_set, -1) < 0) 
		{
            perror("Server poll  error");
            return -1;
        }
        if (poll (act_set, num_set, -1) > 0) 
		{
           for (i=0; i<num_set; i++) 
		   {
              if (act_set[i].revents & POLLIN) 
			  {
                  printf("get POLLIN at fd %d\n", act_set[i].fd);
                  act_set[i].revents &= ~POLLIN;
                  if (i==0) 
				  {
                      new_sock = accept(act_set[i].fd, (struct sockaddr*)&Socket_cl, &size);
                      printf("\nnew client port %u\n", ntohs(Socket_cl.sin_port));
                      if (num_set < 100) 
					  {
                         act_set[num_set].fd = new_sock;
                         act_set[num_set].events = POLLIN;
                         act_set[num_set].revents = 0;
                         num_set++;
                      } 
					  else 
					  {
                        printf("\nno more sockets for client\n");
                        close(new_sock);
                      }
                   } 
				   else 
				   {
                      err = parsing(stroka, act_set[i].fd, Matrici); 
                      printf("%d [%s] %p\n", err, stroka, strstr(stroka,"stop"));
                      
					  if ( err<0 || strstr(stroka,"stop") ) 
					  {
                          close (act_set[i].fd);
                          if (i < num_set-1) 
						  {
                             act_set[i] = act_set[num_set - 1];
                             num_set--;
                             i--;
                          }
                      } 
                  }  
              }
           }
        }
    }
}

void  message_to_client(int id, char *stroka)
{
	int  bytes_sent;

	bytes_sent = write(id, stroka, strlen(stroka));
	if (bytes_sent < 0)
	{
		perror("\nMessaging to client error\n");
	}
}

int parsing(char* stroka, int id, vector <Matrix> Mat_vect)
{
	if(strlen(stroka)<0)
	{
		perror("\nString reading error\n");
	}
	
	vector <Matrix> Good_matrix;
	vector <Matrix> Matrix_product;
	Matrix matrica(stroka);
	
	int i=0;
	int k=0;
	
	for(i=0; i< Mat_vect.size(); i++)
	{
		bool mozhno = can_i_multiply(Mat_vect.at(i), matrica);
		if(mozhno == 1)
		{
			Good_matrix.push_back(Mat_vect.at(i));
			Matrix* mat_product = new Matrix(Mat_vect.at(i).get_mat_row(), matrica.get_mat_col());
			mat_product = mat_mult(Mat_vect.at(i), matrica);
			Matrix_product.push_back(*mat_product);
			k++;
			
			/*double* massiv = new double[matrica.get_mat_col()*matrica.get_mat_row()];
			twoD_to_oneD(matrica, matrica.get_mat_row(), matrica.get_mat_col(), massiv);
			char* massivich =" ";
			for(int j=0; j<Mat_vect.at(i).get_mat_row()*Mat_vect.at(i).get_mat_col(); j++)
			{
			 tut ya pitalsa srazu prislat matricu klientu
			}*/
		}
	}
	
	if(k==0) 
	{
		Mat_vect.push_back(matrica);
		char* stringg = new char[50];
		sprintf(stringg, "\nSended matrix was added on server\n");
		message_to_client(id, stringg);
		free(stringg);
	}
	else
	{
		for(i=0; i<k; i++)
		{
			char* string = new char[60];
			sprintf(string, "\n %d matrix on server was multiplied on yours\n", i);
			message_to_client(id, string);
			message_to_client(id, matrix_to_string(Matrix_product.at(i)));
			free(string);
		}
	}
		
	
	return 0;
}
