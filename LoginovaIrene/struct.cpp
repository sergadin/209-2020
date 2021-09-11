 #include "struct.h"
#include "head.h"

int eq(double x)
{
	return fabs(x) < 1e-16;
}

int tmp_correct(double res, cond_type O)//Проверка на корректность операции
{
	if(O == COND_NONE)
		return 1;
	if((O == LT)||(O == LE)||(O == NE))//Часть для меньше
		if(res < 0)		return 1;
	if((O == EQ)||(O == GE)||(O == LE))//Часть для равно
		if( eq(res) )	return 1;
	if((O == GT)||(O == GE)||(O == NE))//Часть для больше
		if(res > 0)		return 1;
	return 0;
}

//functions for student

void student :: print( FILE * fp)
{	fprintf( fp ,"%s \t%1.2f \t%d\n", name, rating, group );}

void student :: write( int sock)
{	
	char pr[LEN];
	sprintf( pr ,"%s \t%1.2f \t%d", name, rating, group );
	write_socket(sock, pr);
}


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

void list :: write( int sock)
{
	char pr[LEN];
    sprintf( pr, "List :\n");
	write_socket(sock, pr);
    curr = root;
    while( curr )
    {
        curr->data->write(sock);
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
		else if( strcmp( node->name, curr->name) < 0)
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

void tree :: write( int sock)
{
	char pr[LEN];
	sprintf( pr, "Tree :\n");
	write_socket(sock, pr);
	if(root)	rec_write(root, 0, sock);
	//fprintf( stream, "End of the tree.\n");
	return;
}

void tree :: rec_write( tree_node * node, int tab,  int sock)
{
	//for(int i = 0 ;  i < tab; i++)	sprintf(pr + 2*i, "  ");
	node->data->write(sock);
	if(node->right)	rec_write( node->right, tab+1, sock);
	if(node->left)	rec_write( node->left, tab+1, sock);
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
			if((curr->my_tree->is_empty())||(curr->my_list->is_empty()))
			{
				if((curr->my_tree->is_empty())&&(curr->my_list->is_empty()))
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

void hash :: write( int sock)
{
	char pr[LEN];
	int count = 0;
	for( int i = 0 ; i < HASH; i++)
	{
		if(!root[i])	continue;
		count ++;
		curr = root[i];
		//printf("\nHash[%d] starts here :", i);
		while(curr)
		{
			write_socket(sock, " ");
			sprintf(pr, "Group = %d\n", curr->group);
			write_socket(sock, pr);
			curr->my_list->write(sock);
			write_socket(sock, " ");
			curr->my_tree->write(sock);
			curr = curr->next;
		}
		write_socket(sock, " ");
	}
	if(count == 0)
		write_socket(sock, "Database is empty!\n");
}

int hash :: fill(int n, FILE * in)
{
	int gr;
    double rt;
    char name[64];
    student * prime;
    for( int i = 0; i < n; i++)
    {
        fscanf(in, "%s %lf %d", name, &rt, &gr);
        prime = new student( name, rt, gr);
		add( prime );
		prime = 0;
    }
}

//functions for comand

void comand :: clear()
{	
	type = CMD_NONE;
	if(name_root)	name_root->clear();
	name_root = 0;
	if(rat_root)	rat_root->clear();
	rat_root  = 0;
	if(gr_root)		gr_root->clear();
	gr_root   = 0;
}

void comand :: print(FILE * stream)
{
	if(type == SELECT)	printf("select   |");
	else if(type == RESELECT)	printf("reselect |");
	else if(type == DELETE)		printf("delete   |");
	else if(type == UPDATE)		printf("update   |");
	const cond_type c_mass[7] = {EQ, NE, LT, GT, LE, GE, SET};
	const char * mass[7] = {"==","!=","<", ">", "<=", ">=", "set"};
	if(gr_root)
	{
		fprintf(stream, "group ");
		cond_for_gr * curr = gr_root;
		while(curr)
		{
			for(int i = 0 ; i < 7; i++)
				if(curr->c_gr == c_mass[i])	printf("( %s ", mass[i]);
			fprintf(stream,"%d )", curr->group);
			curr = curr->next;
		}
		fprintf(stream," | ");
	}
	if(rat_root)
	{
		fprintf(stream,"rating ");
		cond_for_rat * curr = rat_root;
		while(curr)
		{
			for(int i = 0 ; i < 7; i++)
				if(curr->c_rat == c_mass[i])	printf("( %s ", mass[i]);
			fprintf(stream,"%1.2f )", curr->rating);
			curr = curr->next;
		}
		fprintf(stream," | ");
	}
	if(name_root)
	{
		fprintf(stream,"name ");
		cond_for_name * curr = name_root;
		while(curr)
		{
			for(int i = 0 ; i < 7; i++)
				if(curr->c_name == c_mass[i])	printf("( %s ", mass[i]);
			fprintf(stream,"%s )", curr->name);
			curr = curr->next;
		}
		fprintf(stream," | ");
	}
	printf("\n");
}

int comand :: analyze(char * com)
{
	char buff[100];
	char sign[3];
	cond_type C;
	while(sscanf(com, "%s", buff)!=0)
	{
		if(strcmp(buff, "end")==0)	break;
		else if(strcmp(buff, "group")==0)
		{
			com = strstr(com + 1 , " ");
			if(sscanf(com, "%s", sign)==0) return 1;
			com = strstr(com + 1 , " ");
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			int gr;
			if(sscanf(com, "%d", &gr)==0) return 1;
			com = strstr(com + 1 , " ");
			cond_for_gr * node = new cond_for_gr(C, gr);
			cond_for_gr * next = gr_root;
			gr_root = node;
			node->next = next;
		}
		else if(strcmp(buff, "rating")==0)
		{
			com = strstr(com + 1 , " ");
			if(sscanf(com, "%s", sign)==0) return 1;
			com = strstr(com + 1 , " ");
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			double rat;
			if(sscanf(com, "%lf", &rat)==0) return 1;
			com = strstr(com + 1 , " ");
			cond_for_rat * node = new cond_for_rat(C, rat);
			cond_for_rat * next = rat_root;
			rat_root = node;
			node->next = next;
		}
		else if(strcmp(buff, "name")==0)
		{
			com = strstr(com + 1 , " ");
			if(sscanf(com, "%s", sign)==0) return 1;
			com = strstr(com + 1 , " ");
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			if(sscanf(com, "%s", buff)==0) return 1;
			com = strstr(com + 1 , " ");
			cond_for_name * node = new cond_for_name(C, buff);
			cond_for_name * next = name_root;
			name_root = node;
			node->next = next;
		}
		else	return 1;
	}
	return gr_sort()*100 + rat_sort()*10 + name_sort();
}

int comand :: gr_sort()
{
	int Eq_max = 1;
	int Eq_min = 1;
	cond_for_gr * curr = gr_root;
	if(!gr_root)	return 0;
	cond_for_gr * next;
	cond_for_gr * node;
	gr_root = 0;
	int curr_gr;
	int gr_min = 0;
	int gr_max = 1000;
	cond_for_gr * Equal = 0;
	cond_for_gr * Not_equal = 0;
	cond_for_gr * Less = 0;
	cond_for_gr * Great = 0;
	while(curr)
	{
		next = curr->next;
		curr->next = 0;
		curr_gr = curr->group;
		if(curr->c_gr == LT)
		{
			if( curr_gr <= gr_max )
			{
				gr_max = curr_gr;
				Eq_max = 0;
				Less = curr;
			}
		}
		else if(curr->c_gr == LE)
		{
			if( curr_gr < gr_max )
			{
				gr_max = curr_gr;
				Eq_max = 1;
				Less = curr;
			}
		}
		else if(curr->c_gr == GT)
		{
			if( curr_gr >= gr_min )
			{
				gr_min = curr_gr;
				Eq_min = 0;
				Great = curr;
			}
		}
		else if(curr->c_gr == GE)
		{
			if( curr_gr > gr_min )
			{
				gr_min = curr_gr;
				Eq_min = 1;
				Great = curr;
			}
		}
		else if(curr->c_gr == NE)
		{
			node = Not_equal;
			if(node)
			{
				if( node->group > curr_gr )
				{
					Not_equal = 0;
					curr->next = node;
					Not_equal = curr;
				}
				else if( node->group < curr_gr )
				{
					cond_for_gr * prev = node;
					
					while(prev->next)
					{
						node = prev->next;
						if(node->group > curr_gr)
						{
							prev->next = curr;
							curr->next = node;
							break;
						}
						if(node->group == curr_gr)
							break;
						prev = node;
					}
					if((!prev->next)&&(prev->group != curr_gr))
						prev->next = curr;
				}
			}
			else	Not_equal = curr;
		}
		else if(curr->c_gr == EQ)
		{
			if(Equal)
			{
				if(Equal->group != curr_gr)
					return -1;
			}
			else	Equal = curr;
		}
		curr = next;
	}
	if(gr_min > gr_max)
		return -1;
	if(Not_equal)
	{
		while((Not_equal)&&(Not_equal->group < gr_min))
		{
			next = Not_equal->next;
			Not_equal->next = 0;
			Not_equal = next;
		}
		node = Not_equal;
		if(node&&(node->group > gr_max))
		{
			Not_equal->clear();
			Not_equal = 0;
		}
		while(node && (node->next))
		{
			next = node->next;
			if(next->group > gr_max)
			{
				next->clear();
				next = 0;
				node->next = 0;
				break;
			}
			node = next;
		}
		if(Not_equal && Not_equal->group == gr_min)
		{
			Eq_min = 0;
			if(Great)	Great->c_gr = GT;
		}
		if(node && node->group == gr_max)
		{
			Eq_max = 0;
			if(Less)	Less->c_gr = LT;
		}
	}	
	if(gr_min == gr_max)
	{
		if(Eq_min*Eq_max == 0)	return -1;
		else
		{
			Less->c_gr = EQ;
			if(Equal)
			{
				if(Equal->group != gr_min)
					return -1;
			}
			else	Equal = Less;
		}
	}
	if(Equal)
	{
		curr_gr = Equal->group;
		if((curr_gr >= gr_max)&&(!Eq_max || (curr_gr != gr_max)))
			return -1;
		if((curr_gr <= gr_min)&&(!Eq_min || (curr_gr != gr_min)))
			return -1;
		gr_root = Equal;
		while(Not_equal && Not_equal->group != curr_gr)
			Not_equal = Not_equal->next;
		if(Not_equal) return -1;
		else return 0;
	}
	gr_root = Not_equal;
	if(Less)
	{
		next = gr_root;
		gr_root = Less;
		Less->next = next;
	}
	if(Great)
	{
		next = gr_root;
		gr_root = Great;
		Great->next = next;
	}
	return 0;
}

int comand :: rat_sort()
{
	int Eq_max = 1;
	int Eq_min = 1;
	cond_for_rat * curr = rat_root;
	if(!rat_root)	return 0;
	cond_for_rat * next;
	cond_for_rat * node;
	rat_root = 0;
	double curr_rat;
	double rat_min = 0;
	double rat_max = 1000;
	cond_for_rat * Equal = 0;
	cond_for_rat * Not_equal = 0;
	cond_for_rat * Less = 0;
	cond_for_rat * Great = 0;
	while(curr)
	{
		next = curr->next;
		curr->next = 0;
		curr_rat = curr->rating;
		if(curr->c_rat == LT)
		{
			if( curr_rat <= rat_max )
			{
				rat_max = curr_rat;
				Eq_max = 0;
				Less = curr;
			}
		}
		else if(curr->c_rat == LE)
		{
			if( curr_rat < rat_max )
			{
				rat_max = curr_rat;
				Eq_max = 1;
				Less = curr;
			}
		}
		else if(curr->c_rat == GT)
		{
			if( curr_rat >= rat_min )
			{
				rat_min = curr_rat;
				Eq_min = 0;
				Great = curr;
			}
		}
		else if(curr->c_rat == GE)
		{
			if( curr_rat > rat_min )
			{
				rat_min = curr_rat;
				Eq_min = 1;
				Great = curr;
			}
		}
		else if(curr->c_rat == NE)
		{
			node = Not_equal;
			if(node)
			{
				if( node->rating > curr_rat )
				{
					Not_equal = 0;
					curr->next = node;
					Not_equal = curr;
				}
				else if( node->rating < curr_rat )
				{
					cond_for_rat * prev = node;
					
					while(prev->next)
					{
						node = prev->next;
						if(node->rating > curr_rat)
						{
							prev->next = curr;
							curr->next = node;
							break;
						}
						if(eq(node->rating - curr_rat))
							break;
						prev = node;
					}
					if((!prev->next)&&(prev->rating != curr_rat))
						prev->next = curr;
				}
			}
			else	Not_equal = curr;
		}
		else if(curr->c_rat == EQ)
		{
			if(Equal)
			{
				if(!eq(Equal->rating - curr_rat))
					return -1;
			}
			else	Equal = curr;
		}
		curr = next;
	}
	if(rat_min > rat_max)	return -1;
	if(Not_equal)
	{
		while((Not_equal)&&(Not_equal->rating < rat_min))
		{
			next = Not_equal->next;
			Not_equal->next = 0;
			Not_equal = next;
		}
		node = Not_equal;
		if(node&&(node->rating > rat_max))
		{
			Not_equal->clear();
			Not_equal = 0;
		}
		while(node && (node->next))
		{
			next = node->next;
			if(next->rating > rat_max)
			{
				next->clear();
				next = 0;
				node->next = 0;
				break;
			}
			node = next;
		}
		if(Not_equal && Not_equal->rating == rat_min)
		{
			Eq_min = 0;
			if(Great)	Great->c_rat = GT;
		}
		if(node && node->rating == rat_max)
		{
			Eq_max = 0;
			if(Less)	Less->c_rat = LT;
		}
	}	
	if(eq(rat_min - rat_max))
	{
		if(Eq_min*Eq_max == 0)	return -1;
		else
		{
			Less->c_rat = EQ;
			if(Equal)
				if(!eq(Equal->rating - rat_min))	return -1;
			else	Equal = Less;
		}
	}
	if(Equal)
	{
		curr_rat = Equal->rating;
		if((curr_rat >= rat_max)&&(!Eq_max || !eq(curr_rat - rat_max)))
			return -1;
		if((curr_rat <= rat_min)&&(!Eq_min || !eq(curr_rat - rat_min)))
			return -1;
		rat_root = Equal;
		while(Not_equal && !eq(Not_equal->rating - curr_rat))
			Not_equal = Not_equal->next;
		if(Not_equal) return -1;
		else return 0;
	}
	rat_root = Not_equal;
	if(Less)
	{
		next = rat_root;
		rat_root = Less;
		Less->next = next;
	}
	if(Great)
	{
		next = rat_root;
		rat_root = Great;
		Great->next = next;
	}
	return 0;
}

int comand :: name_sort()
{
	int Eq_max = 1;
	int Eq_min = 1;
	cond_for_name * curr = name_root;
	if(!name_root)	return 0;
	cond_for_name * next;
	cond_for_name * node;
	name_root = 0;
	char * curr_name;
	char min_char = ' ';
	char max_char = '~';
	char * name_min = &min_char;
	char * name_max = &max_char;
	cond_for_name * Equal = 0;
	cond_for_name * Not_equal = 0;
	cond_for_name * Less = 0;
	cond_for_name * Great = 0;
	while(curr)
	{
		next = curr->next;
		curr->next = 0;
		curr_name = curr->name;
		if(curr->c_name == LT)
		{
			if( strcmp(curr_name, name_max) <= 0 )
			{
				name_max = curr_name;
				Eq_max = 0;
				Less = curr;
			}
		}
		else if(curr->c_name == LE)
		{
			if( strcmp(curr_name, name_max) < 0 )
			{
				name_max = curr_name;
				Eq_max = 1;
				Less = curr;
			}
		}
		else if(curr->c_name == GT)
		{
			if( strcmp(curr_name, name_min ) >= 0)
			{
				name_min = curr_name;
				Eq_min = 0;
				Great = curr;
			}
		}
		else if(curr->c_name == GE)
		{
			if( strcmp(curr_name, name_min) > 0)
			{
				name_min = curr_name;
				Eq_min = 1;
				Great = curr;
			}
		}
		else if(curr->c_name == NE)
		{
			node = Not_equal;
			if(node)
			{
				if( strcmp(node->name, curr_name ) > 0)
				{
					Not_equal = 0;
					curr->next = node;
					Not_equal = curr;
				}
				else if( strcmp( node->name, curr_name ) < 0)
				{
					cond_for_name * prev = node;
					
					while(prev->next)
					{
						node = prev->next;
						if(strcmp(node->name, curr_name) > 0)
						{
							prev->next = curr;
							curr->next = node;
							break;
						}
						if( strcmp(node->name, curr_name) == 0)
							break;
						prev = node;
					}
					if((!prev->next)&&( strcmp(prev->name, curr_name) != 0 ))
						prev->next = curr;
				}
			}
			else	Not_equal = curr;
		}
		else if(curr->c_name == EQ)
		{
			if(Equal)
			{
				if(strcmp(Equal->name, curr_name) != 0)
					return -1;
			}
			else	Equal = curr;
		}
		curr = next;
	}
	if( strcmp(name_min, name_max) > 0)		return -1;
	if(Not_equal)
	{
		while((Not_equal)&&( strcmp(Not_equal->name, name_min) < 0))
		{
			next = Not_equal->next;
			Not_equal->next = 0;
			Not_equal = next;
		}
		node = Not_equal;
		if(node&&( strcmp(node->name, name_max) > 0))
		{
			Not_equal->clear();
			Not_equal = 0;
		}
		while(node && (node->next))
		{
			next = node->next;
			if( strcmp(next->name, name_max) > 0)
			{
				next->clear();
				next = 0;
				node->next = 0;
				break;
			}
			node = next;
		}
		if( Not_equal && strcmp( Not_equal->name,  name_min) != 0)
		{
			Eq_min = 0;
			if(Great)	Great->c_name = GT;
		}
		if(node && strcmp( node->name, name_max) != 0)
		{
			Eq_max = 0;
			if(Less)	Less->c_name = LT;
		}
	}
	if( strcmp(name_min, name_max) == 0)
	{
		if(Eq_min*Eq_max == 0)	return -1;
		else
		{
			if(!Less)	Less = Great;
			Less->c_name = EQ;
			if(Equal)
			{
				if( strcmp(Equal->name, name_min) != 0)
					return -1;
			}
			else	Equal = Less;
		}
	}
	if(Equal)
	{
		curr_name = Equal->name;
		if(( strcmp(curr_name , name_max) >= 0)&&(!Eq_max || ( strcmp(curr_name, name_max) != 0)))
			return -1;
		if(( strcmp(curr_name , name_min) <= 0)&&(!Eq_min || ( strcmp(curr_name, name_min) != 0)))
			return -1;
		name_root = Equal;
		while(Not_equal && strcmp( Not_equal->name, curr_name) != 0)
			Not_equal = Not_equal->next;
		if(Not_equal) return -1;
		else return 0;
	}
	name_root = Not_equal;
	if(Less)
	{
		next = name_root;
		name_root = Less;
		Less->next = next;
	}
	if(Great)
	{
		next = name_root;
		name_root = Great;
		Great->next = next;
	}
	return 0;
}
		   
		   
cond_type comand :: find_cond(char * cond)
{
	const char * mass[6] = {"==","!=","<", ">", "<=", ">="};
	const cond_type c_mass[6] = {EQ, NE, LT, GT, LE, GE};
	for(int i = 0 ; i < 6; i++)
	{
		if(strcmp(cond, mass[i])==0)	return c_mass[i];
	}
	return COND_NONE;
}



//functions for Session

int Session :: add( student * prime )
{
    sub_sess * node = new sub_sess( prime);
    if( !root){   root = node;    return 0;}
    curr = root;
    while( curr->next )	curr = curr->next;
    curr->next = node;
	curr = 0;
    return 0;
}

int Session :: del_it( student * prime )
{
    if( root->data == prime ){	root = root->next;	return 0;}
    curr = root;
    while( curr->next )
    {
        sub_sess * tmp = curr->next;
		if( tmp->data == prime )
		{
			curr->next = tmp->next;
			tmp->next = 0;
			return 0;
		}
        curr = tmp;
    }
	curr = 0;
    return 1;
}

void Session :: print( FILE * stream )
{
	if(!root)
	{
		fprintf( stream, "Session is empty\n");
		return ;
	}
    fprintf( stream, "Session :\n");
    curr = root;
    while( curr )
    {
        curr->data->print(stream);
        curr = curr->next;
    }
	curr = 0;
    //fprintf( stream, "End of the list.\n");
}

void Session :: write( int sock )
{
	if(!root)
	{
		write_socket(sock, "Session is empty\n");
		return ;
	}
    write_socket(sock, "Session :\n");
    curr = root;
    while( curr )
    {
        curr->data->write(sock);
        curr = curr->next;
    }
	curr = 0;
    //fprintf( stream, "End of the list.\n");
}

int Session :: inside(student * elem)
{
	double res;
	char * 	my_name = elem->get_name();
	double 	rt = elem->get_rt();
	int 	gr = elem->get_gr();
	
	for(curr = root; curr != 0; curr = curr->next)
	{
		student * dat = curr->data;
		if(strcmp(dat->get_name(), my_name))
			continue;
		if(dat->get_gr() != gr)
			continue;
		if(!eq(dat->get_rt() - rt))
			continue;
		return 1;
	}
	return 0;
}

//int main

int main(int argc, char *argv[])
{
	fd_set active_set, read_set;
    int i, opt, sock;
    struct sockaddr_in addr;
    char end[4];
    strcpy(end, "end");
    opt = 1;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {   perror ("Cannot create socket\n");   return -1;   }
    puts("Socket created\n");
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {   perror("Cannot bind socket\n");   return -2;}
    puts("Bind done\n");
    if (listen(sock, 3) < 0) {   perror("Cannot be listened\n");   return -3;}
    FD_ZERO(&active_set);
    FD_SET(sock, &active_set);
	
	int n=atoi(argv[1]);
	if((argc < 2)||n < 1)	return 0;
	FILE * in = fopen(argv[2], "r");
	hash * my_hash = new hash();
	printf("Database in progress...\n");
	my_hash->fill(n, in);
	printf("Database has %d elements\n", n);
	fclose(in);
	Database * Base = new Database(my_hash);
	my_hash = 0;
	comand * cmd = new comand();
	char * tmp;
    char * com;
    char * buf_f;
    char buff[LEN];
    int quit=1;
    puts("Listen\n");
    int st_sel=0;
    while (true) {
        read_set = active_set;
        if (select(FD_SETSIZE, &read_set, 0, 0, 0) < 0) {
            perror("Cannot select\n");
            return -4;
        }
        if(!st_sel)  puts("Select comands in process");
        st_sel++;
        for (i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &read_set))
            {
                if (i == sock)
                {
                    struct sockaddr_in client;
                    socklen_t size = sizeof(client);
                    int new_sock = accept(sock, (struct sockaddr *)&client, &size);
                    if (new_sock < 0) {   perror ("cannot accept\n");   return -5;   }
                    FD_SET(new_sock, &active_set);
                }
                else
                {
					
					Base->set_socket(i);
					/*
					sprintf(buff, "i = %d, sock = %d\n", i, sock);
					
					write_socket(i, buff);
					write_socket(i, buff);
					*/
                    	char buf[LEN];
                    	if(read_socket(i, buf) < 0) {   close(i);   FD_CLR(i, &active_set);}
                    	else
                    	{
							write_socket(i, buf);
                        strcpy(buff,buf);
                        buf_f  = buff;
                        quit = 1;
                        while((quit==1)&&buf_f)
                        {
                            tmp=0;
                            com = buf_f;
                            buf_f = tmp;
                            switch(com[0])
							{
								case 'p':
									{
										//Base->base_print(stdout);
										Base->base_write();
										break;
									}
								case 'i':
									{
										if(strstr(com,"insert")==com)
										{
											com = com + 7;
											int gr;
											double rt;
											char name[64];
											sscanf(com, "%s %lf %d", name, &rt, &gr);
											student * prime = new student( name, rt, gr);

											int ret = Base->add(prime);

											if(ret == 1)	write_socket(i,"Already in base!\n");
											else
											{
												write_socket(i,"inserted : ");
												prime->write(i);
											}
											prime = 0;
										}
										break;
									}
								case 's':
									{
										if(strstr(com,"select")==com)
										{
											Base->clear_sess();

											cmd->type = SELECT; 
											int ret = cmd->analyze(com + 7);
											if( ret == 1 )
											{
												cmd->type = CMD_NONE;
												continue;
											}
											char pr[LEN];
											if( ret < 0 )
											{
												sprintf(pr, "We have conter conditions! Error %3d\n", -ret);
												write_socket(i,pr);
												cmd->type = CMD_NONE;
												continue;
											}
											//cmd->print();
											Base->do_select( cmd);
											sprintf(pr,"Selected %d elements\n", Base->sess_size());
											write_socket(i,pr);
										}
										else if(strstr(com, "s_pr")==com)
										{
											Base->sess_write();
										}
										else if(strstr(com, "stop")==com)
										{
											quit = 2;
										}
										break;
									}
								case 'r':
									{
										if(strstr(com,"reselect")==com)
										{
											cmd->type = RESELECT;
											int ret = cmd->analyze(com + 9);
											if( ret == 1 )
											{
												cmd->type = CMD_NONE;
												continue;
											}
											char pr[LEN];
											if( ret < 0 )
											{
												sprintf(pr, "We have conter conditions! Error %3d\n", -ret);
												write_socket(i,pr);
												cmd->type = CMD_NONE;
												continue;
											}
											//cmd->print();
											Base->reselect(cmd);
											sprintf(pr,"Reselected %d elements\n", Base->sess_size());
											write_socket(i,pr);
										}
										break;
									}
								case 'q':
									{
										if(strstr(com,"quit")==com)	quit = 0;
										break;
									}
								default:
									continue;
							}
   						}
                        if (quit == 0)
                        {
                            strcpy(buf,"quit;");
                            strcpy(end,"ent");
                            write_socket(i, end);
                            puts("Select comands ends with success\n");
                            puts("Listen\n");
                            st_sel=0;
                            close(i);
                            FD_CLR(i, &active_set);
                        }
                        else if (quit == 2)
                        {
                            strcpy(buf,"stop;");
                            strcpy(end,"ent");
                            write_socket(i, end);
                            puts("Select comands ends with success\n");
                            puts("Clossing server\n");
                            puts("Please wait some moments\n");
                            close(i);
                            FD_CLR(i, &active_set);
                            return 0;
                        }
                        else
                        {
                            strcpy(end,"end");
                            write_socket(i, end);
                        }
						}
                }
            }
        }
    }
    return 0;
}


// functions for select

void Database :: do_select( comand * cmd)
{
	hash_node * curr;
	int gr_max = 699;
	int gr_min = 100;
	int sign_max = 0;
	int sign_min = 0;
	cond_for_gr * node = cmd->gr_root;
	cond_for_gr * n_eq;
	if(node)
	{
		if(node->get_cond() == EQ)
		{
			int gr = node->get_gr();
			curr = my_hash->get_elem( my_hash->get_index( gr ));
			while(curr)
			{
				if(curr->get_group() == gr)
					//printf("Subselect in group %d\n", curr->get_group());
					sub_select_group(curr, cmd);
				curr = curr->get_next();
			}
		 	return;
		}
		if((node->get_cond() == GT)||(node->get_cond() == GE))
		{
			if(node->get_cond() == GE)	sign_min = 1;
			gr_min = node->get_gr();
			node = node->get_next();
		}
		if((node)&&((node->get_cond() == LT)||(node->get_cond() == LE)))
		{	
			if(node->get_cond() == LE)	sign_max = 1;
			gr_max = node->get_gr();
			node = node->get_next();
		}
	}
	n_eq = node;
	for(int i = my_hash->get_index(gr_min); i <= my_hash->get_index(gr_max); i++)
	{
		curr = my_hash->get_elem( i );
		while(curr)
		{
			int gr = curr->get_group();
			if(( gr > gr_min || (sign_min && gr == gr_min )) &&  ((gr < gr_max) || (sign_max && gr == gr_max )))
			{
				if(n_eq)
				{
					node = n_eq;
					while(node && (gr > node->get_gr()))	node = node->get_next();
					if(node && (gr == node->get_gr()))
					{
						curr = curr->get_next();
						continue;
					}
				}
				sub_select_group(curr, cmd);
			}
			curr = curr->get_next();
		}
	}
}

void Database :: sub_select_group(hash_node * head, comand * cmd)
{
	if(cmd->name_root != 0)			sub_select_tree(head->get_tree(), cmd);
	else							sub_select_list(head->get_list(), cmd);
}

void Database :: sub_select_tree( tree * head, comand * cmd)
{
	tree_node * curr;
	char min_char = ' ';
	char max_char = '~';
	char * name_min = &min_char;
	char * name_max = &max_char;
	char * tmp;
	int sign_max = 0;
	int sign_min = 0;
	cond_for_name * node = cmd->name_root;
	cond_for_name * n_eq;
	if(node->get_cond() == EQ)//Часть для равно
	{
		tmp = node->get_name();
		curr = head->get_root();
		while(curr)
		{
			if( strcmp( tmp, curr->get_name()) > 0)
			{
				if(curr->get_right())	curr = curr->get_right();
				else	break;
			}
			else if( strcmp( tmp, curr->get_name()) < 0)
			{
				if(curr->get_left())	curr = curr->get_left();
				else break;
			}
			else
			{
				if(sel_rat(curr->get_data(), cmd))
					sess[sock]->add(curr->get_data());//Это нам подходит
				break;
			}
		}
		return;
	}
	if((node->get_cond() == GT)||(node->get_cond() == GE))
	{
		if(node->get_cond() == GE)	sign_min = 1;
		name_min = node->get_name();
		node = node->get_next();
	}
	if((node)&&((node->get_cond() == LT)||(node->get_cond() == LE)))
	{	
		if(node->get_cond() == LE)	sign_max = 1;
		name_max = node->get_name();
		node = node->get_next();
	}
	n_eq = node;
	rec_sel_tree(head->get_root(), n_eq, name_min, sign_min, name_max, sign_max, cmd);
	name_min = 0;
	name_max = 0;
}

void Database :: rec_sel_tree(tree_node * curr, cond_for_name * n_eq, char * min, int smn, char * max, int smx, comand * cmd)
{
	cond_for_name * node;
	char * tmp = curr->get_name();
	if( strcmp(tmp, min) < 0)
	{	if(curr->get_right())	rec_sel_tree(curr->get_right(), n_eq, min, smn, max, smx, cmd);
	}
	else if( strcmp(tmp, max) > 0)
	{	if(curr->get_left())	rec_sel_tree(curr->get_left(), n_eq, min, smn, max, smx, cmd);
	}
	else if(( strcmp(tmp, min) == 0) && !smn)
	{
		if(curr->get_right())	rec_sel_tree(curr->get_right(), n_eq, min, smn, max, smx, cmd);
	}
	else if(( strcmp(tmp, max) == 0) && !smx)
	{
		if(curr->get_left())	rec_sel_tree(curr->get_left(), n_eq, min, smn, max, smx, cmd);
	}
	else
	{
		node = n_eq;
		while(node && strcmp(tmp , node->get_name()) > 0)	node = node->get_next();
		if((!node) || strcmp(tmp , node->get_name()) < 0)
			if(sel_rat(curr->get_data(), cmd))
				sess[sock]->add(curr->get_data());//Это нам подходит
		if(curr->get_left())	rec_sel_tree(curr->get_left(), n_eq, min, smn, max, smx, cmd);
		if(curr->get_right())	rec_sel_tree(curr->get_right(), n_eq, min, smn, max, smx, cmd);
	}
}
	
void Database :: sub_select_list(list * head, comand * cmd)
{
	list_node * curr = head->get_root();
	double rat_min = 0;
	double rat_max = 1000;
	int sign_max = 0;
	int sign_min = 0;
	cond_for_rat * node = cmd->rat_root;
	if(node && (node->get_cond() == EQ))
	{
		double rt = node->get_rt();
		while(curr)
		{
			if(curr->get_rt() > rt)	return;
			if(eq(curr->get_rt() - rt))
				sess[sock]->add(curr->get_data());
			curr = curr->get_next();
		}
		return;
	}
	if(node && ((node->get_cond() == GT)||(node->get_cond() == GE)))
	{
		if(node->get_cond() == GE)	sign_min = 1;
		rat_min = node->get_rt();
		node = node->get_next();
	}
	if(node && ((node->get_cond() == LT)||(node->get_cond() == LE)))
	{	
		if(node->get_cond() == LE)	sign_max = 1;
		rat_max = node->get_rt();
		node = node->get_next();
	}
	while(curr)
	{
		double rt = curr->get_rt();
		if(( rt < rat_min ) || (eq(rt - rat_min ) && !sign_min))
		{
			curr = curr->get_next();
			continue;
		}
		else if(( rt > rat_max ) || (eq(rt - rat_max ) && !sign_max))
		{
			return;
		}
		while(node && (rt > node->get_rt()))	node = node->get_next();
		if(node && eq(rt - node->get_rt()))
		{
			curr = curr->get_next();
			continue;
		}
		sess[sock]->add(curr->get_data());
		
		curr = curr->get_next();
	}
}

// functions for reselect

void Database :: reselect(comand * cmd)
{
	sub_sess * curr = sess[sock]->get_root();
	int t = 0;
	sub_sess * prev = 0;
	while(curr)
	{
		t = sel_gr(curr->get_data(), cmd);
		if(t == 1)
			t += sel_rat(curr->get_data(), cmd);
		if(t == 2)
			t += sel_name(curr->get_data(), cmd);
		if(t != 3)
		{
			if(prev)
			{
				sess[sock]->set_next(prev, curr->get_next());
				sess[sock]->set_next(curr, 0);
				curr = prev;
			}
			else
			{
				sess[sock]->set_root(curr->get_next());
				sess[sock]->set_next(curr, 0);
				curr = sess[sock]->get_root();
				continue;
			}
		}
		prev = curr;
		curr = curr->get_next();
	}
}

// functions for update

void Database :: update(comand * cmd)
{
	sub_sess * curr = sess[sock]->get_root();
	int t = 0;
	while(curr)
	{
		student * prime = curr->get_data();
		if(cmd->gr_root)
			prime->set_group((cmd->gr_root)->get_gr());
		if(cmd->rat_root)
			prime->set_rating((cmd->rat_root)->get_rt());
		if(cmd->name_root)
			prime->set_name((cmd->name_root)->get_name());
		curr = curr->get_next();
	}
}

// student подходит под условия cmd

int Database :: sel_gr(student * prime, comand * cmd)
{
	int gr = prime->get_gr();
	cond_for_gr * node = cmd->gr_root;
	int yes = 1;
	if(node && (node->get_cond() == EQ))
	{
		if(node->get_gr() == gr)	return 1;
		else	return 0;
	}
	if(node && (node->get_cond() == GT))
	{
		if(node->get_gr() >= gr)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == GE))
	{
		if(node->get_gr() > gr)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LT))
	{
		if(node->get_gr() <= gr)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LE))
	{
		if(node->get_gr() < gr)	return 0;
		node = node->get_next();
	}
	while(node && (gr > node->get_gr()))	node = node->get_next();
	if(node && (gr == node->get_gr()))	return 0;
	return 1;
}

int Database :: sel_rat(student * prime, comand * cmd)
{
	double rt = prime->get_rt();
	cond_for_rat * node = cmd->rat_root;
	int yes = 1;
	if(node && (node->get_cond() == EQ))
	{
		if(eq(node->get_rt() - rt))	return 1;
		else	return 0;
	}
	if(node && (node->get_cond() == GT))
	{
		if(node->get_rt() >= rt)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == GE))
	{
		if(node->get_rt() > rt)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LT))
	{
		if(node->get_rt() <= rt)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LE))
	{
		if(node->get_rt() < rt)	return 0;
		node = node->get_next();
	}
	while(node && (rt > node->get_rt()))	node = node->get_next();
	if(node && eq( rt - node->get_rt()))	return 0;
	return 1;
}

int Database :: sel_name(student * prime, comand * cmd)
{
	char * name = prime->get_name();
	cond_for_name * node = cmd->name_root;
	int yes = 1;
	if(node && (node->get_cond() == EQ))
	{
		if(strcmp(node->get_name(),name) == 0 )	return 1;
		else	return 0;
	}
	if(node && (node->get_cond() == GT))
	{
		if( strcmp(node->get_name(), name) >= 0)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == GE))
	{
		if(strcmp(node->get_name(), name) > 0)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LT))
	{
		if(strcmp(node->get_name(), name) <= 0)	return 0;
		node = node->get_next();
	}
	if(node && (node->get_cond() == LE))
	{
		if(strcmp(node->get_name(), name) < 0)	return 0;
		node = node->get_next();
	}
	while(node && (strcmp(node->get_name(), name) < 0))	node = node->get_next();
	if(node && (strcmp(node->get_name(), name) == 0))	return 0;
	return 1;
}
