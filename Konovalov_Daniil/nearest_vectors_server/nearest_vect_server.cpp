#include "server.h"

using namespace std;

void Vector_server::add_vector(int *vect)
{
	vectors.push_back(vect);
}

int Vector_server::get_dimension()
{
	return this->razmernost__;
}
void Vector_server::set_port(int port)
{
	this->port_ = port;
}

void Vector_server::set_address(int addr)
{
	this->address_ = addr;
}

void Vector_server::set_dim(int dim)
{
	this->razmernost__ = dim;
}

int** Vector_server::search_for_nearest_vectors(int* vector, int k)
{	
	int** nearest_vectors = new int*[vectors.size()];
	int* distance_arr = new int[vectors.size()];
	
	for(int j=0; j<vectors.size(); j++)
	{
		nearest_vectors[j] = new int[sizeof(vector)];
	}
	
	list <int*>::iterator p = vectors.begin();
  	
	for(int i=0; p != vectors.end(); i++) 
	{
		distance_arr[i] = distance(vector, *p);
		nearest_vectors[i] = *p; 
		p++;
  	}
  	
  	int temp;
  	int *tmp = new int[this->get_dimension() + 1];
  	
  	for (int q = 0; q < vectors.size() - 1; q++) 
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
    
    int** otvet = new int*[k];
    for(int i = 0; i<k; i++)
    {
    	otvet[i] = nearest_vectors[i];
	}
	
	return otvet;
}

int distance(int *vect1, int* vect2)
{
	if(sizeof(vect1)!=sizeof(vect2))
	{
		printf("\n Cant find the distance between R^k and R^n vectors\n");
		return -1;
	}
	int dist = 0;
	
	for(int i=0; i < sizeof(vect1); i++)
	{
		dist += (vect2[i] - vect1[i])^2;
	}	
	
	return dist;
}

void Vector_server::get_server_info()
{	
	char* Infa = new char[100]; 
	sprintf(Infa, "Dimension:%d\nNumber of vectors:%d\nActive clients:%d\nServer port:%d\nServer address:%d", this->razmernost__, (int)this->vectors.size(), this->clients_number, this->port_, this->address_);
	
	
	WriteToClient(Infa, clients_number);
}

void Vector_server::clear_client_vector(int client_id)
{
list <int*>::iterator p = vectors.begin();
  	

	while(p != vectors.end()) 
	{
		int* item = new int[razmernost__+1];
		item = *p;
		if(item[razmernost__] == client_id)
		{
			vectors.erase(p);
			free(*p);
		}
		else
		{
			p++;
		}
		free(item);
  	}	
}

int Vector_server::parsing(char* stroka, int aidi_)
{
	char symbol;
	int i=0;
	
	while((stroka[i] != ' ') || (stroka[i] != '\n') || (stroka[i] != '\0'))
	{
		i++;
	}
	
	char* cmd = (char*)malloc(i);
	 
	for (int j=0; j<i; j++)
	{
		cmd[j] = stroka[j];
	}
	
	if(strcmp(cmd, "info")==0)
	{
		get_server_info();
	}
	else if(strcmp(cmd, "add")==0)
	{
		int* massiv = new int[razmernost__ + 1];
		int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			int dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda =new char[dlina];
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++;
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
		
		massiv[razmernost__] = aidi_;
		add_vector(massiv);
	}
	else if(strcmp(cmd, "query")==0)
	{
		int* massiv = new int [razmernost__ + 1];
		int p = 0;
		
		while(i < sizeof(stroka))
		{	
			int k = i;
			int dlina = 0;
			
			while(stroka[k] != ' ')
			{
				dlina++;
				k++;
			}
			
			k=i;
			
			char* suda = new char[dlina];
			int m = 0;
			
			while(m < dlina)
			{
				suda[m] = stroka[k];
				m++;
				k++;
			}
			
			massiv[p] = atoi(suda);
			free(suda);
			p++;
		}
		
		int** neighbours = search_for_nearest_vectors(massiv, massiv[razmernost__]);
		
		char* clientu;
		
		for(int q = 0; q<sizeof(neighbours); q++)
		{
			for(int r=0; r < razmernost__ + 1; r++)
			{
				char* prosto_tak = new char[razmernost__+2];
				sprintf(prosto_tak, " %d", neighbours[q][r]); 
				strcat(clientu, prosto_tak);
				free(prosto_tak);
			}
		}
		
		WriteToClient(clientu, aidi_);
	}
	else if(strcmp(cmd, "clear")==0)
	{
		clear_client_vector(aidi_);
	}
	else
	{
		printf("Ya ne ponimayu tvoy language, try eshe razok\n");
		return -1;
	}
	
	return 0;
}


void  Vector_server::WriteToClient (char *stroka, int id_)
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
