 #include "struct.h"

//functions for student

int student :: init(const char * buff, double rt, int gr)
{
	strcpy( name, buff );
	rating = rt;
	group = gr;
    return 0;
}

void student :: print( FILE * fp)
{	fprintf( fp ,"%s    %f    %d\n", name, rating, group );}

student & student ::operator = (student &&x)//move
{
    strcpy( name , x.name);
    rating = x.rating;
    x.rating = 0;
    group = x.group;
    x.group = 0;
    return * this;
}

//functions for list

int list :: add( student * prime )
{
    list_node *node = new list_node( prime);
    if( !root)
    {
        root = node;
        return 0;
    }
    curr = root;
    if(node->rating <= root->rating)
    {
        node->next = root;
        root = node;
        return 0;
    }
    while( curr->next )
    {
        list_node *tmp = curr->next;
        if( tmp->rating > node->rating )
        {
            curr->next = node;
            node->next = tmp;
            return 0;
        }
        curr = tmp;
    }
    curr->next = node;
    return 0;
}

int list :: del_it( student * prime )
{
    double rat = prime -> get_rt();
    if( root -> rating >rat)	return 1;
	else if( root->rating >=rat && strcmp(root->data->get_name(), prime->get_name()) == 0)
	{
		curr = root->next;
		root->next = 0;
		root = curr;
		return 0;
	}	
    curr = root;
    while( curr->next )
    {
        list_node * tmp = curr->next;
        if( tmp->rating < rat )	return 1;
		else if( tmp->rating <= rat && strcmp(tmp->data->get_name(), prime->get_name()) == 0)
		{
			curr->next = tmp->next;
			tmp->next = 0;
			return 0;
		}
        curr = tmp;
    }
    return 1;
}

void list :: print( FILE * stream )
{
    fprintf( stream, "List :\n");
    curr = root;
    while( curr )
    {
        curr->data->print(stream);
        curr = curr->next;
    }
    //fprintf( stream, "End of the list.\n");
}

//functions for tree

int tree :: add( student * prime )
{
	tree_node * node = new tree_node( prime);
	curr = root;
	while(curr)
	{
		if( strcmp( node->name, curr->name) > 0)
		{
			if(curr->right)	curr = curr->right;
			else
			{
				curr->right = node;
				return 0;
			}
		}
		else if( strcmp( node->name, curr->name) > 0)
		{
			if(curr->left)	curr = curr->left;
			else
			{
				curr->left = node;
				return 0;
			}
		}
		else	return 1;
	}
	root = node;
	return 0;
}


int tree :: del_it( student * prime )
{
	
	char * str = prime->get_name();
	
	if(strcmp(str, root->name) == 0)
	{
		root = parent_ret(root);
		return 0;
	}
	curr = root;
	while(curr)
	{
		if(strcmp(str, curr->name) > 0)
		{
			tree_node * tmp = curr->right;
			if(!tmp)	return 1;
			if(strcmp(str, tmp->name) == 0)
			{
				curr->right = parent_ret(tmp);
				return 0;
			}
			curr = tmp;
		}
		else if(strcmp(str, curr->name) < 0)
		{
			tree_node * tmp = curr->left;
			if(!tmp)	return 1;
			if(strcmp(str, tmp->name) == 0)
			{
				curr->left = parent_ret(tmp);
				return 0;
			}
			curr = tmp;
		}
	}
	return 0;
}

tree_node * tree :: parent_ret(tree_node * node)
{
	curr = node->right;
	if(!curr)
	{
		curr = node->left;
		node->left = 0;
		return curr;
	}
	tree_node * prev = node;
	if(!curr->left)
	{
		node->right = 0;
		curr->left = node->left;
		return curr;
	}
	while(curr)
	{
		if(!curr->left)
		{
			tree_node * tmp = curr->right;
			prev->left = tmp;
			curr->right = 0;
			curr->right = node->right;
			node->right = 0;
			curr->left = node -> left;
			node->left = 0;
			return curr;
		}
		prev = curr;
		curr = prev->left;
	}
}
	
void tree :: print ( FILE * stream )
{
	fprintf( stream, "Tree :\n");
	if(root)	rec_print(root, 0, stream);
	//fprintf( stream, "End of the tree.\n");
	return;
}

void tree :: rec_print( tree_node * node, int tab,  FILE * stream)
{
	for(int i = 0 ;  i < tab; i++)	printf("  ");
	node->data->print(stream);
	if(node->right)	rec_print( node->right, tab+1, stream);
	if(node->left)	rec_print( node->left, tab+1, stream);
}
//functions for hash

int hash :: add( student * prime )
{
	int ret;
	int ind = hash_fun(prime);
	if(!root[ind])
	{
		root[ind] = new hash_node(prime);
		return 0;
	}
	curr = root[ind];
	if(curr->group == prime->get_gr())
	{
		ret = curr->my_tree->add( prime);
		if(ret == 1)	return 1;
		curr->my_list->add( prime);
		return 0;
	}
	hash_node * tmp;
	while(curr->next)
	{
		tmp = curr->next;
		if(tmp->group == prime->get_gr())
		{
			ret = tmp->my_tree->add( prime);
			if(ret == 1)	return 1;
			tmp->my_list->add( prime);
			return 0;
		}
		curr = tmp;
	}
	tmp = new hash_node(prime);
	curr->next = tmp;
	return 0;
}

int hash :: del_it( student * prime )
{
	int ind = hash_fun(prime);
	if(!root[ind])	return 1;
	curr = root[ind];
	hash_node * prev = 0;
	int ret;
	while(curr)
	{
		if(curr->group == prime->get_gr())
		{
			ret = curr->my_tree->del_it( prime);
			curr->my_tree->print();
			if(ret == 1)	return 1;
			curr->my_list->del_it( prime);
			curr->my_list->print();
			if((curr->my_tree->root==0)||(curr->my_tree->root==0))
			{
				if((curr->my_tree->root==0)&&(curr->my_tree->root==0))
				{
					if(prev)	prev = curr->next;
					else		root[ind] = curr->next;
					curr->next = 0;
					return 0;
				}
				printf("Something wrong!!\n");
				return 1;
			}	
			return 0;
		}
		prev = curr;
		curr = curr->next;
	}
	return 1;
}

void hash :: print ( FILE * stream )
{
	int count = 0;
	for( int i = 0 ; i < HASH; i++)
	{
		if(!root[i])	continue;
		count ++;
		curr = root[i];
		//printf("\nHash[%d] starts here :", i);
		while(curr)
		{
			printf("\ngroup = %d\n\n", curr->group);
			curr->my_list->print(stream);
			fprintf(stream, "\n");
			curr->my_tree->print(stream);
			curr = curr->next;
		}
		printf("\n");
	}
	if(count == 0)
		printf("Database is empty!\n");
}

//int main
int main(int argc, char *argv[])
{
    Data_base Object;
	Object.run(argc, argv);
	return 0;
}

int Data_base :: run(int argc, char **argv)
{
	int n=atoi(argv[1]);
	if((argc < 2)||n < 1)	return 0;
	FILE * in = fopen(argv[2], "r");
	printf("Database in progress...\n");
	fill(n, in);
	printf("Database has %d elements\n", n);
	fclose(in);
	char com[100];
	char *tmp;
	int quit = 1;
	while(quit)
	{
		printf("Input comand : ");
		if(!scanf("%s", com))	continue;
		switch(com[0])
		{
			case 'p':
				{
					my_hash->print(stdout);
					break;
				}
			case 'i':
				{
					if(strstr(com,"insert")==com)
					{
						int gr;
						double rt;
						char name[64];
						scanf("%s %lf %d", name, &rt, &gr);
						student * prime = new student( name, rt, gr);
						int ret = my_hash->add(prime);
						if(ret == 1)	printf("Already in base!\n");
						else
						{
							printf("inserted - ");
							prime->print();
						}
						prime = 0;
					}
					break;
				}
			case 'd':
				{
					if(strstr(com,"del_it")==com)
					{
						int gr;
						double rt;
						char name[64];
						scanf("%s %lf %d", name, &rt, &gr);
						student * prime = new student( name, rt, gr);
						int ret = my_hash->del_it(prime);
						if(ret == 1)	printf("Not in base!\n");
						else
						{
							printf("del_ited - ");
							prime->print();
						}
						prime = 0;
					}
					else if(strstr(com,"delete")==com)
					{
						printf("Comand delete\n");
					}
					break;
				}
			case 's':
				{
					if(strstr(com,"select")==com)
					{
						printf("Comand select\n");
					}
					break;
				}
			case 'r':
				{
					
					if(strstr(com,"reselect")==com)
					{
						printf("Comand reselect\n");
					}
					break;
				}
			case 'q':
				{
					if(strcmp(com,"quit")==0)	quit = 0;
					break;
				}
			default:
				continue;
		}
	}
    return 0;
}

int Data_base :: fill( int n, FILE * in)
{
	int gr;
    double rt;
    char name[64];
	student * prime;
    fscanf(in, "%s %lf %d", name, &rt, &gr);
    prime = new student( name, rt, gr);
	my_hash = new hash( prime);
	prime = 0;
    for( int i = 0; i < n-1; i++)
    {
        fscanf(in, "%s %lf %d", name, &rt, &gr);
        prime = new student( name, rt, gr);
		my_hash->add( prime );
		prime = 0;
    }
}
