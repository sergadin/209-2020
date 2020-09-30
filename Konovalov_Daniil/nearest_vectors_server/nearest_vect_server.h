#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <list>

using namespace std;

class Server
{
	public:
		int ReadFromClient(char* stroka, int id_);
		void WriteToClient(char* stroka, int id_);
		int parsing(char* stroka, int aidi_);	
	
	private:
		list <int*> vectors; 
		int razmernost__;
		int clients_number;
		int port_;
		int family_;
		int address_;
		
		void add_vector(int* vect);
		int** search_for_nearest_vectors(int *vector, int k);
		void get_server_info();
		void clear_client_vector(int client_id);
}

