#include "server.h"

using namespace std;

void Server::add_vector(int *vect)
{
	vectors.push_back(vect);
}

int** Server::search_for_nearest_vectors(int* vector, int k)
{	
	int** nearest_vectors = (int*)malloc(vectors.size());
	int* distance_arr = (int)malloc(vectors.size());
	
	for(int j=0; j<vectors.size(); j++)
	{
		nearest_vectors[j] = (int)malloc(razmernost_ + 1);
	}
	
	list <int*>::iterator p = vectors.begin();
  	
	for(int i=0; p != vectors.end(); i++) 
	{
		distance_arr[i] = distance(vector, *p);
		nearest_vectors[i] = *p; 
		p++;
  	}
  	
  	int temp;
  	int *tmp = (int*)malloc(razmernost__ + 1);
  	
  	for (int q = 0; q < vectors.size() - 1; i++) 
	  {
        for (int s = 0; s < vectors.size() - q - 1; s++) 
		{
            if (distance_arr[s] > distance_arr[s + 1]) 
			{
                temp = distance_arr[s];
                distance_arr[s] = distance_arr[s + 1];
                distance_arr[s + 1] = temp;
                
                tmp = nearest_vectors[s];
                nearest_vectors[s] = nearest_vectors[s + 1];
                nearest_vectors[s + 1] = tmp;
            }
        }
    }
    
    int** otvet = (int*)malloc(k);
    for(i = 0; i<k; i++)
    {
    	otvet[i] = nearest_vectors[i];
	}
	
	return otvet;
}

int distance(int *vect1, int* vect2)
{
	
	int dist = 0;
	
	for(int i=0; i < razmernost_; i++)
	{
		dist += (vect2[i] - vect1[i])^2;
	}	
	
	return dist;
}

void get_server_info()
{	
	char* Infa = itoa(razmernost__); 
	char* Infa_ = itoa(vectors.size());
	char* Infa__ = itoa(clients_number);
	char* Infa___ = itoa(port_);
	char* Infa____ = itoa(family_);
	char* Infa_____ = itoa(address_);

	strcat(Infa, " ");
	strcat(Infa_, " ");
	strcat(Infa__, " ");
	strcat(Infa___, " ");
	strcat(Infa____, " ");

	strcat(Infa, Infa_);
	strcat(Infa, Infa__);
	strcat(Infa, Infa___);
	strcat(Infa, Infa____);
	strcat(Infa, Infa_____);
	
	WriteToClient(Infa, clients_number);
}

void Server::clear_client_vector(int client_id)
{
list <int*>::iterator p = vectors.begin();
  	
	while(p != vectors.end()) 
	{
		if(*p[razmernost_] == client_id)
		{
			vectors.erase(p);
			free(*p);
		}
		else
		{
			p++;
		}
  	}	
}

int Server::parsing(char* stroka, int aidi_)
{
	char symbol;
	int i=0;
	
	while((stroka[i] != ' ') || (stroka[i] != '\n') || (stroka[i] != '\0'))
	{
		i++;
	}
	
	char* cmd = (char*)malloc(i);
	 
	while (int j=0; j<i; j++)
	{
		cmd[j] = stroka[j];
	}
	
	if(strcmp(cmd, "info")==0)
	{
		get_server_info();
	}
	else if(strcmp(cmd, "add")==0)
	{
		int* massiv = (int)malloc(razmernost__ + 1);
		int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda =(char)malloc(dlina);
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
		
		massiv[razmernost_] = aidi_;
		add_vector(massiv);
	}
	else if(strcmp(cmd, "query")==0)
	{
		int* massiv = (int)malloc(razmernost__ + 1);
		int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda =(char)malloc(dlina);
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
		
		int** neighbours = search_for_nearest_vectors(massiv, massiv[razmernost__]);
		
		char* clientu;
		
		for(int q = 0; q<sizeof(neighbours); q++)
		{
			for(r=0; r < razmernost__ + 1; r++)
			{
				strcat(clientu, ' ');
				strcat(clientu, itoa(neighbours[q][r]));
			}
		}
		
		WriteToClient(clientu);
	}
	else if(strcmp(cmd, "clear")==0)
	{
		clear_client_vector(aidi);
	}
	else
	{
		printf("Ya ne ponimayu tvoy language, try eshe razok\n");
		return -1;
	}
	
	return 0;
}

int  Server::ReadFromClient(char *stroka, int id_)
{
    int  nbytes;

    nbytes = read(id_,stroka,sizeof(stroka));
    if (nbytes<0) 
	{
        perror ("\nReading message error\n");
        return -1;
    } 
	else if(nbytes==0) 
	{
        return -1;
    } 
	else 
	{
        printf("Server got message: %s\n", stroka);
        return 0;
    }
}

void  Server::WriteToClient (char *stroka, int id_)
{
    int  nbytes;
    char *s;

    for (s=stroka; *s; s++) *s = toupper(*s);
    nbytes = write(id_,stroka,strlen(stroka)+1);
    printf("Write back: %s\nnbytes=%d\n", stroka , nbytes);
    
    if ( nbytes<0 ) 
	{
        perror ("\nWriting to server error\n");
    }
}
