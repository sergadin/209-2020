 #include "struct.h"

int eq(double x)
{
	return fabs(x) < 1e-16;
}

//functions for student

int student :: init(const char * buff, double rt, int gr)
{
	strcpy( name, buff );
	rating = rt;
	group = gr;
    return 0;
}

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

//functions for Session

int Session :: del( student * prime)
{
	char * name = prime->get_name();
	int flag = 0;
	for( int i = 0; i< quan_st; i++)
	{
		if(strcmp(name, data[i]->get_name()) == 0)	flag = 1;
		data[i] = data[i + flag];
	}
	if( flag == 1)	quan_st -= 1;
	return 1 - flag;
}

//functions for comand

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
	
int comand :: analyze()
{
	char buff[100];
	char sign[3];
	while(scanf("%s", buff)!=0)
	{
		if(strcmp(buff, "end")==0)	break;
		else if(strcmp(buff, "group")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			c_gr = find_cond(sign);
			if(c_gr == COND_NONE)	return 1;
			int gr;
			if(scanf("%d", &gr)==0) return 1;
			group = gr;
		}
		else if(strcmp(buff, "rating")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			c_rat = find_cond(sign);
			if(c_rat == COND_NONE)	return 1;
			double rat;
			if(scanf("%lf", &rat)==0) return 1;
			rating = rat;
		}
		else if(strcmp(buff, "name")==0)
		{
			if(scanf("%s", sign)==0) return 1;
			c_name = find_cond(sign);
			if(c_name == COND_NONE)	return 1;
			if(scanf("%s", buff)==0) return 1;
			strcpy(name, buff);
		}
		else	return 1;
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

void comand :: print(FILE * stream)
{
	if(type == SELECT)	printf("select   |");
	else if(type == RESELECT)	printf("reselect |");
	else if(type == DELETE)		printf("delete   |");
	else if(type == UPDATE)		printf("update   |");
	const cond_type c_mass[7] = {EQ, NE, LT, GT, LE, GE, SET};
	const char * mass[7] = {"==","!=","<", ">", "<=", ">=", "set"};
	if(c_gr != COND_NONE)
	{
		fprintf(stream, "group ");
		for(int i = 0 ; i < 7; i++)
			if(c_gr == c_mass[i])	printf("%s ", mass[i]);
		fprintf(stream,"%d |", group);
	}
	if(c_rat != COND_NONE)
	{
		fprintf(stream,"rating ");
		for(int i = 0 ; i < 7; i++)
			if(c_rat == c_mass[i])	printf("%s ", mass[i]);
		fprintf(stream,"%f |", rating);
	}
	if(c_name != COND_NONE)
	{
		fprintf(stream,"name ");
		for(int i = 0 ; i < 7; i++)
			if(c_name == c_mass[i])	printf("%s ", mass[i]);
		fprintf(stream,"%s |", name);
	}
	printf("\n");
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
	
	Session * sess = new Session();
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
						
						sess->add(prime);
						
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
					break;
				}
			case 's':
				{
					if(strstr(com,"select")==com)
					{
						sess->eraze();
						cmd->type = SELECT; 
						int ret = cmd->analyze();
						if( ret == 1 )
						{
							cmd->type = CMD_NONE;
							continue;
						}
						cmd->print();
						do_select(cmd, sess);
					}
					else if(strstr(com, "s_pr"))
					{
						sess->print();
					}
					break;
				}
			case 'u':
				{
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
						sess->reselect(cmd);
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


// functions for select


void Data_base :: do_select(comand * cmd, Session * sess)
{
	hash_node * curr;
	cond_type G = cmd->c_gr;
	int gr = cmd->get_gr();
	int index = my_hash->hash_fun(cmd);
	int i;
	if((G == EQ)||(G == GE)||(G == LE))//Часть для равно
	{
		curr = my_hash->root[index];
		while(curr)
		{
			if(curr->group == gr)
				curr->do_select(cmd, sess);
			curr = curr->next;
		}
	}
	if((G == GT)||(G == GE)||(G == NE))//Часть для больше
	{
		for( i = index + 1; i < HASH; i++)
		{
			curr = my_hash->root[i];
			while(curr)
			{
				curr->do_select(cmd, sess);
				curr = curr->next;
			}
		}
		curr = my_hash->root[index];
		while(curr)
		{
			if(curr->group > gr)
				curr->do_select(cmd, sess);
			curr = curr->next;
		}
	}
	if((G == LT)||(G == LE)||(G == NE))//Часть для меньше
	{
		for( i = 0; i < index; i++)
		{
			curr = my_hash->root[i];
			while(curr)
			{
				curr->do_select(cmd, sess);
				curr = curr->next;
			}
		}
		curr = my_hash->root[index];
		while(curr)
		{
			if(curr->group < gr)
				curr->do_select(cmd, sess);
			curr = curr->next;
		}
	}
	if(G == COND_NONE)
	{
		for( i = 0; i < HASH; i++)
		{
			curr = my_hash->root[i];
			while(curr)
			{
				curr->do_select(cmd, sess);
				curr = curr->next;
			}
		}
	}
}

void hash_node :: do_select(comand * cmd, Session * sess)
{
	if(cmd->c_name != COND_NONE)	my_tree->do_select(cmd, sess);
	else							my_list->do_select(cmd, sess);
}

void tree :: do_select(comand * cmd, Session * sess)
{
	char * tmp = cmd->get_name();
	cond_type N = cmd->c_name;
	if((N == EQ)||(N == GE)||(N == LE))//Часть для равно
	{
		curr = root;
		while(curr)
		{
			if( strcmp( tmp, curr->name) > 0)
			{
				if(curr->right)	curr = curr->right;
				else	break;
			}
			else if( strcmp( tmp, curr->name) < 0)
			{
				if(curr->left)	curr = curr->left;
				else break;
			}
			else
			{
				sel_rat(curr->data, cmd, sess);//Это нам подходит
				break;
			}
		}
	}
	if((N == GT)||(N == GE)||(N == NE))//Часть для больше
	{
		curr = root;
		while(curr)
		{
			if( strcmp( tmp, curr->name) >= 0)
			{
				if(curr->right)	curr = curr->right;
				else	break;
			}
			else if( strcmp( tmp, curr->name) < 0)
			{
				sel_rat(curr->data, cmd, sess); //Это нам подходит
				if(curr->right)	sel_rat_rec(curr->right, cmd, sess);	//Это нам подходит - функция которая перебирает элементы поддерева
				if(curr->left)	curr = curr->left;
				else break;
			}
		}
	}
	if((N == LT)||(N == LE)||(N == NE))//Часть для меньше
	{
		curr = root;
		while(curr)
		{
			if( strcmp( tmp, curr->name) > 0)
			{
				sel_rat(curr->data, cmd, sess); //Это нам подходит
				if(curr->left)	sel_rat_rec(curr->left, cmd, sess);  	//Это нам подходит - функция которая перебирает элементы поддерева
				if(curr->right)	curr = curr->right;
				else	break;
			}
			else if( strcmp( tmp, curr->name) <= 0)
			{
				if(curr->left)	curr = curr->left;
				else break;
			}
		}
	}
}

void tree :: sel_rat_rec(tree_node * node, comand * cmd ,Session * sess)
{
	sel_rat(node->data, cmd, sess);
	if(node->right)	sel_rat_rec(node->right, cmd, sess);
	if(node->left)	sel_rat_rec(node->left, cmd, sess);
}
		
void tree :: sel_rat(student * prime, comand * cmd ,Session * sess)
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

void list :: do_select(comand * cmd, Session * sess)
{
	double rat = cmd->get_rt();
	cond_type R = cmd->c_rat;
	curr = root;
	if((R == LT)||(R == LE)||(R == NE))//Часть для меньше
	{
		while(curr)
		{
			if(curr->rating >= rat)	break;
			else
				sess->add(curr->data);
			curr = curr->next;
		}
	}
	if((R == EQ)||(R == GE)||(R == LE))//Часть для равно
	{
		while(curr)
		{
			if(curr->rating > rat)	break;
			if( eq(curr->rating - rat) )
				sess->add(curr->data);
			curr = curr->next;
		}
	}
	if((R == GT)||(R == GE)||(R == NE))//Часть для больше
	{
		while(curr)
		{
			if(curr->rating > rat)
				sess->add(curr->data);
			curr = curr->next;
		}
	}
	if(R == COND_NONE)
	{
		while(curr)
		{
			sess->add(curr->data);
			curr = curr->next;
		}
	}
}

// functions for reselect

void Session :: reselect(comand * cmd)
{
	int tmp_name = 0;
	int tmp_rt = 0;
	int tmp_gr = 0;
	int t;
	int sh = 0;
	double res;
	char * buff = cmd->get_name();
	double rt = cmd->get_rt();
	int gr = cmd->get_gr();
	
	for(int i = 0; i < quan_st; i++)
	{
		res = strcmp(data[i]->get_name(), buff);
		tmp_name = tmp_correct(res, cmd->c_name);
		
		res = data[i]->get_gr() - gr;
		tmp_gr = tmp_correct(res, cmd->c_gr);
		
		res = data[i]->get_rt() - rt;
		tmp_rt = tmp_correct(res, cmd->c_rat);
		
		t = tmp_name * tmp_rt * tmp_gr;
		if(t == 0)
		{
			data[i] = 0;
			sh += 1;
		}
		else
			if(sh != 0)
			{
				data[i - sh] = data[i];
				data[i] = 0;
			}
	}
	quan_st -= sh;
}
			

int Session :: tmp_correct(double res, cond_type O)
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



