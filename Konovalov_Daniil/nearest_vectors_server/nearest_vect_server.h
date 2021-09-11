#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <list>

using namespace std;

class Vector_server
{
	public:
		
		void WriteToClient(char* stroka, int id_);
		int parsing(char* stroka, int aidi_);	
		void set_port(int port);
		void set_address(int addr);
		void set_dim(int dim);
	
	private:
		list <int*> vectors; 
		int razmernost__;
		int clients_number;
		int port_;
		int address_;
	
		int** search_for_nearest_vectors(int *vector, int k);
		int get_dimension();	
		void add_vector(int* vect);
		void get_server_info();
		void clear_client_vector(int client_id);
};
