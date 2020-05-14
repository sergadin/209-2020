 #include "struct.h"

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
{	fprintf( fp ,"%s \t%f \t%d\n", name, rating, group );}

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
		fprintf(stream, "group : ");
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
			fprintf(stream,"%f )", curr->rating);
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

/*
int comand :: analyze_upd(comand * cmd)
{
	char buff[100];
	while(scanf("%s", buff)!=0)
	{
		if(strcmp(buff, "end")==0)	break;
		else if(strcmp(buff, "group")==0)
		{
			int gr;
			if(scanf(" = %d", &gr)==0) return 1;
			c_gr = SET;
			group = gr;
		}
		else if(strcmp(buff, "rating")==0)
		{
			double rat;
			if(scanf(" = %lf", &rat)==0) return 1;
			c_rat = SET;
			rating = rat;
		}
		else if(strcmp(buff, "name")==0)
		{
			if(scanf(" = %s", buff)==0) return 1;
			c_name = SET;
			strcpy(name, buff);
		}
		else if(strcmp(buff, "where")==0)	return cmd->analyze();
		else	return 1;
	}
	return 0;
}
*/

int comand :: analyze()
{
	char buff[100];
	char sign[3];
	cond_type C;
	while(scanf("%s", buff)!=0)
	{
		if(strcmp(buff, "end")==0)	break;
		else if(strcmp(buff, "group")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			int gr;
			if(scanf("%d", &gr)==0) return 1;
			cond_for_gr * node = new cond_for_gr(C, gr);
			cond_for_gr * next = gr_root;
			gr_root = node;
			node->next = next;
		}
		else if(strcmp(buff, "rating")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			double rat;
			if(scanf("%lf", &rat)==0) return 1;
			cond_for_rat * node = new cond_for_rat(C, rat);
			cond_for_rat * next = rat_root;
			rat_root = node;
			node->next = next;
		}
		else if(strcmp(buff, "name")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			C = find_cond(sign);
			if(C == COND_NONE)	return 1;
			if(scanf("%s", buff)==0) return 1;
			cond_for_name * node = new cond_for_name(C, buff);
			cond_for_name * next = name_root;
			name_root = node;
			node->next = next;
		}
		else	return 1;
	}
	return comand_sorting();
}

int comand :: comand_sorting()
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
					Not_equal = curr;
					curr->next = node;
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
					if(!node->next && (node->group != curr_gr))
						node->next = curr;
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
		Equal->next = Not_equal;
		return 0;
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
	char com[100];
	char *tmp;
	int quit = 1;
	while(quit)
	{
		cmd->clear();
		printf("Input comand : ");
		if(!scanf("%s", com))	continue;
		switch(com[0])
		{
			case 'p':
				{
					Base->base_print(stdout);
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
						
						int ret = Base->add(prime);
						
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
					/*
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
						sess->eraze();
						cmd->type = DELETE; 
						int ret = cmd->analyze();
						if( ret == 1 )
						{
							cmd->type = CMD_NONE;
							continue;
						}
						cmd->print();
					}
					*/
					break;
				}
			case 's':
				{
					if(strstr(com,"select")==com)
					{
						Base->clear_sess();
						
						cmd->type = SELECT; 
						int ret = cmd->analyze();
						if( ret == 1 )
						{
							cmd->type = CMD_NONE;
							continue;
						}
						if( ret == -1 )
						{
							printf("We have conter conditions!\n");
							continue;
						}
						cmd->print();
						//Base->do_select( cmd);
					}
					else if(strstr(com, "s_pr"))
					{
						Base->sess_print();
					}
					break;
				}
			case 'u':
				{
					/*
					if(strstr(com,"update")==com)
					{
						comand * set = new comand();
						sess->eraze();
						set->type = UPDATE;
						cmd->type = SELECT;
						int ret = set->analyze_upd( cmd );
						if( ret == 1 )
						{
							cmd->type = CMD_NONE;
							continue;
						}
						set->print();
						cmd->print();
					}
					*/
					break;
				}
			case 'r':
				{
					if(strstr(com,"reselect")==com)
					{
						cmd->type = RESELECT;
						int ret = cmd->analyze();
						if( ret == 1 )
						{
							cmd->type = CMD_NONE;
							continue;
						}
						cmd->print();
						//Base->reselect(cmd);
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

/*

// functions for select

void Database :: do_select( comand * cmd)
{
	hash_node * curr;
	cond_type G = cmd->c_gr;
	int gr = cmd->get_gr();
	int index = my_hash->get_index(cmd->get_gr());
	int i;
	if((G == EQ)||(G == GE)||(G == LE))//Часть для равно
	{
		curr = my_hash->get_elem( index );
		while(curr)
		{
			if(curr->get_group() == gr)
				sub_select_group(curr, cmd);
			curr = curr->get_next();
		}
	}
	if((G == GT)||(G == GE)||(G == NE))//Часть для больше
	{
		for( i = index + 1; i < HASH; i++)
		{
			curr = my_hash->get_elem( i );
			while(curr)
			{
				sub_select_group(curr, cmd);
				curr = curr->get_next();
			}
		}
		curr = my_hash->get_elem( index );
		while(curr)
		{
			if(curr->get_group() > gr)
				sub_select_group(curr, cmd);
			curr = curr->get_next();
		}
	}
	if((G == LT)||(G == LE)||(G == NE))//Часть для меньше
	{
		for( i = 0; i < index; i++)
		{
			curr = my_hash->get_elem( i );
			while(curr)
			{
				sub_select_group(curr, cmd);
				curr = curr->get_next();
			}
		}
		curr = my_hash->get_elem( index );
		while(curr)
		{
			if(curr->get_group() < gr)
				sub_select_group(curr, cmd);
			curr = curr->get_next();
		}
	}
	if(G == COND_NONE)
	{
		for( i = 0; i < HASH; i++)
		{
			curr = my_hash->get_elem( i );
			while(curr)
			{
				sub_select_group(curr, cmd);
				curr = curr->get_next();
			}
		}
	}
}

void Database :: sub_select_group(hash_node * head, comand * cmd)
{
	if(cmd->c_name != COND_NONE)	sub_select_tree(head->get_tree(), cmd);
	else							sub_select_list(head->get_list(), cmd);
}

void Database :: sub_select_tree( tree * head, comand * cmd)
{
	char * tmp = cmd->get_name();
	cond_type N = cmd->c_name;
	tree_node * curr;
	if((N == EQ)||(N == GE)||(N == LE))//Часть для равно
	{
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
				sel_rat(curr->get_data(), cmd);//Это нам подходит
				break;
			}
		}
	}
	if((N == GT)||(N == GE)||(N == NE))//Часть для больше
	{
		curr = head->get_root();
		while(curr)
		{
			if( strcmp( tmp, curr->get_name()) >= 0)
			{
				if(curr->get_right())	curr = curr->get_right();
				else	break;
			}
			else if( strcmp( tmp, curr->get_name()) < 0)
			{
				sel_rat(curr->get_data(), cmd); //Это нам подходит
				if(curr->get_right())	sub_select_subtree(curr->get_right(), cmd);	//Это нам подходит - функция которая перебирает элементы поддерева
				if(curr->get_left())	curr = curr->get_left();
				else break;
			}
		}
	}
	if((N == LT)||(N == LE)||(N == NE))//Часть для меньше
	{
		curr = head->get_root();
		while(curr)
		{
			if( strcmp( tmp, curr->get_name()) > 0)
			{
				sel_rat(curr->get_data(), cmd); //Это нам подходит
				if(curr->get_left())	sub_select_subtree(curr->get_left(), cmd);  	//Это нам подходит - функция которая перебирает элементы поддерева
				if(curr->get_right())	curr = curr->get_right();
				else	break;
			}
			else if( strcmp( tmp, curr->get_name()) <= 0)
			{
				if(curr->get_left())	curr = curr->get_left();
				else break;
			}
		}
	}
}

void Database :: sub_select_subtree(tree_node * node, comand * cmd)
{
	sel_rat(node->get_data(), cmd);
	if(node->get_right())	sub_select_subtree(node->get_right(), cmd);
	if(node->get_left())	sub_select_subtree(node->get_left(), cmd);
}
		
void Database :: sel_rat(student * prime, comand * cmd)
{
	double rt = cmd->get_rt();
	double rt_pr = prime->get_rt();
	cond_type R = cmd->c_rat;
	int yes = 0;
	if((R == EQ)||(R == GE)||(R == LE))//Часть для равно
		if(rt_pr == rt)	yes = 1;
	if((R == GT)||(R == GE)||(R == NE))//Часть для больше
		if(rt_pr > rt)	yes = 1;
	if((R == LT)||(R == LE)||(R == NE))//Часть для меньше
		if(rt_pr < rt)	yes = 1;
	if(R == COND_NONE)	yes = 1;
	if(yes)	sess->add(prime);
}

void Database :: sub_select_list(list * head, comand * cmd)
{
	double rat = cmd->get_rt();
	cond_type R = cmd->c_rat;
	list_node * curr = head->get_root();
	if((R == LT)||(R == LE)||(R == NE))//Часть для меньше
	{
		while(curr)
		{
			if(curr->get_rt() >= rat)	break;
			else
				sess->add(curr->get_data());
			curr = curr->get_next();
		}
	}
	if((R == EQ)||(R == GE)||(R == LE))//Часть для равно
	{
		while(curr)
		{
			if(curr->get_rt() > rat)	break;
			if( eq(curr->get_rt() - rat) )
				sess->add(curr->get_data());
			curr = curr->get_next();
		}
	}
	if((R == GT)||(R == GE)||(R == NE))//Часть для больше
	{
		while(curr)
		{
			if(curr->get_rt() > rat)
				sess->add(curr->get_data());
			curr = curr->get_next();
		}
	}
	if(R == COND_NONE)
	{
		while(curr)
		{
			sess->add(curr->get_data());
			curr = curr->get_next();
		}
	}
}

// functions for reselect

void Database :: reselect(comand * cmd)
{
	int tmp_name = 0;
	int tmp_rt = 0;
	int tmp_gr = 0;
	int t = 0;
	int sh = 0;
	double res;
	char * buff = cmd->get_name();
	double rt = cmd->get_rt();
	int gr = cmd->get_gr();
	sub_sess * prev;
	sub_sess * curr = sess->get_root();
	while(curr)
	{
		student * dat = curr->get_data();
		res = strcmp(dat->get_name(), buff);
		tmp_name = tmp_correct(res, cmd->c_name);
		
		res = dat->get_gr() - gr;
		tmp_gr = tmp_correct(res, cmd->c_gr);
		
		res = dat->get_rt() - rt;
		tmp_rt = tmp_correct(res, cmd->c_rat);
		
		t = tmp_name * tmp_rt * tmp_gr;
		if(!t &&(curr == sess->get_root()))
		{
			curr = curr->get_next();
			sess->set_root( curr );
		}
		else if(!t)
		{
			curr = curr->get_next();
			sess->set_next( prev, curr );
		}
		else
		{
			prev = curr;
			curr = prev->get_next();
		}
	}
}

*/

