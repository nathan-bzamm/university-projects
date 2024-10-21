#include "main.h"

typedef struct _var
{
	char* name;
	char* value;
} var;

#define g_var_count 1024	// space for 1024 variables
var g_vars[g_var_count];

extern char** environ;

typedef struct _node
{
	char* value;
	char is_pipe;
	struct _node* next;
} node;

node* add_node(node** first, char* str, char is_pipe)	// function to add the nodes
{
	// nn = new node
	node* nn = (node*)malloc(sizeof(node));	// creates a node
	if (is_pipe)
	{
		nn->value = 0;		// if it is a pipe then the content of the node is null
		nn->is_pipe = 1;	// notified that it is a pipe
	}
	else
	{
		nn->value = strdup(str);	// string is copied into the node
		nn->is_pipe = 0;			// it is not a pipe
	}

	nn->next = 0;	// the latest node is added to the end, therefore next is set to null

	if (*first == 0)	// if the first node is null, then the new node is set as the first node
	{
		*first = nn;
	}
	else
	{
		// find the last node
		node* ln = *first;
		while (ln->next != 0)
		{
			ln = ln->next;	// move to the next node
		}
		ln->next = nn;	// add the new node to the end of the list of tokens
	}
	return *first;
}

void print_nodes(node* first)	// function to print all the nodes
{
	int i = 0;
	node* n = first;
	while (n != 0)
	{
		printf(
			"[%3d]: %-30s [pipe?: %d]\n", i++, n->value, n->is_pipe);	// to left align the items
		n = n->next;
	}
	printf("\n\n\n");
}

void free_nodes(node* first)
{
	node* n = first;
	while (n != 0)
	{
		if (n->value != 0)
		{
			free(n->value);
		}
		node* t = n;
		n = n->next;
		free(t);
	}
}

char is_spaces_or_null(char* buf)	// function to check whether there is a space or a null
{
	if (buf == 0 || buf[0] == 0)	// if the contents/the pointer itself is 0
	{
		return 1;
	}
	for (char* p = buf; p != 0 && *p != 0; ++p)
	{
		if (*p != ' ')
		{
			return 0;
		}
	}
	return 1;
}

char is_separator(const char c)	// function to check whether char is a seperator or not
{
	switch (c)
	{
	case ' ':
	case '\t':
	case '\n':
	case '|':
	case ';':
	case '<':
	case '>':
		return (char)1;

	default:
		return (char)0;
	}
}

void add_var(char* name, char* value)
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)
		{
			// empty slot found
			int sz = strlen(name);
			g_vars[i].name = (char*)malloc(sz + 1);
			strcpy(g_vars[i].name, name);
			g_vars[i].name[sz] = 0;	// terminate string
			sz = strlen(value);
			g_vars[i].value = (char*)malloc(sz + 1);
			strcpy(g_vars[i].value, value);
			g_vars[i].value[sz] = 0;	// terminate string
			return;
		}
	}
	printf("Variable space is full");
	exit(1);
}

void unset_var(char* name)
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)
		{
			continue;
		}
		if (strcmp(g_vars[i].name, name) == 0)
		{
			// free memory
			free(g_vars[i].name);
			free(g_vars[i].value);

			// mark slots as free/empty
			g_vars[i].name = 0;
			g_vars[i].value = 0;
		}
	}
}

char* get_var(char* name)
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)
		{
			continue;
		}
		if (strcmp(g_vars[i].name, name) == 0)
		{
			return g_vars[i].value;
		}
	}
	return 0;
}

void print_vars()
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)
		{
			break;
		}
		printf("%s=%s\n", g_vars[i].name, g_vars[i].value);
	}
}

void process_nodes(node* first)	// function used to process the node
{
	node* curr_node = first;
	while (curr_node != 0)
	{
		if (curr_node->value != 0 && curr_node->value[0] != 0)
		{
			if (strcmp(curr_node->value, "echo") == 0) //echo - internal command
			{
				node* nx = curr_node->next;
				if (nx != 0 && nx->value[0] != 0)
				{
					printf("%s\n", nx->value);
				}
			}
			else if (strcmp(curr_node->value, "cd") == 0)
			{
				node* nx = curr_node->next;
				if (nx != 0 && nx->value[0] != 0)
				{
					int rc = chdir(nx->value);
					if (rc == 0)
					{
						// success
						char newdir[2048];
						memset(newdir, 0, sizeof(newdir));
						printf("new dir: %s\n", getcwd(newdir, sizeof(newdir) - 1));
					}
					else
					{
						fprintf(stderr, "%s: ", nx->value);
						perror("");
					}
				}
			}
			else if (strcmp(curr_node->value, "set") == 0)
			{
				// set a=b
				node* n1 = curr_node->next;	// n1 would be the 'a'
				node* n2 = n1->next;		   // n2 would be the '='
				node* n3 = n2->next;		   // n3 would be the 'b'
				if (n1 != 0 && n2 != 0 && n3 != 0)
				{
					char* v1 = n1->value;
					char* v2 = n2->value;
					char* v3 = n3->value;
					if ((v1 != 0 && v1[0] != 0) && (v2 != 0 && v2[0] != 0)
						&& (v3 != 0 && v3[0] != 0))
					{
						if (strcmp(v2, "=") == 0)
						{
							add_var(v1, v3);
						}
					}
					// move past the 3 nodes just processed
					curr_node = n3;
				}
			}
			else if (strcmp(curr_node->value, "unset") == 0)
			{
				node* nx = curr_node->next;	// n1 would be the 'a'
				if (nx != 0 && nx->value[0] != 0)
				{
					unset_var(nx->value);
				}
			}
			else if (strcmp(curr_node->value, "showvar") == 0)
			{
				// showvar cmd expects another entry after it;
				// get it
				node* next_node = curr_node->next;
				if (next_node != 0)
				{
					if (next_node->value != 0 && next_node->value[0] != 0)
					{
						printf(
							"command is: %s and value for the command is: %s\n",
							curr_node->value,
							next_node->value);
					}
					curr_node = next_node->next;
				}
				else
					print_all_vars();
			}
		}
		if (curr_node != 0)
		{
			curr_node = curr_node->next;
		}
	}
}
int main(int argc, char* argv[])
{
	memset(g_vars, 0, sizeof(g_vars));
	vars_on_start();

	// run the shell
	while (1)
	{
		printf("prompt> ");
		fflush(stdout);
		char input_buf[4096];						// creates 4096 characters
		memset(input_buf, 0, sizeof(input_buf));	// sets all characters in the buffer to 0/null
		const int rc = read(
			0,
			input_buf,
			sizeof(input_buf)
				- 1);	// reads the input from the keyboard, with a limit of 4095 characters

		if (rc == -1)
		{
			printf("Exiting...\n");
			exit(0);
		}
		input_buf[rc - 1] = 0;	// terminate input which is the "\n"
		if (strcmp(input_buf, "exit") == 0 || strcmp(input_buf, "quit") == 0)	// to exit the
																				 // shell
		{
			printf("Exiting...\n");
			exit(0);
		}

		node* first = 0;				// first node is created and set to null
		char buf[4096];					// buffer of size 4096 is created
		memset(buf, 0, sizeof(buf));	// buffer is cleared

		char esc_dblqte = 0;	// variable to indicate if we are in the middle of quoting (")
		char esc_slash = 0;		// variable to indicate if we are in the middle of escaping (\)
		char* vp = 0;			// variable pointer
		const int len = strlen(input_buf);

		char* d = buf;					 // destination pointer
		for (int i = 0; i < len; ++i)	// going through each character
		{
			char c = input_buf[i];

			if (esc_slash)
			{
				esc_slash = 0;	// reset '\' flag
				*d++ = c;		  // indiscriminately take the escaped character
				continue;
			}

			if (c == '|')
			{
				add_node(&first, 0, 1);
				continue;
			}

			if (c == '\\')
			{
				esc_slash = 1;
				continue;
			}

			if (c == '$' && vp == 0)
			{
				vp = d;
				// is_variable = 1;
				continue;
			}

			if (c == '"')
			{
				if (esc_dblqte)	// if double quote is already set
				{
					// closing quote
					esc_dblqte = 0;
				}
				else
				{
					// opening quote
					esc_dblqte = 1;
				}
				continue;
			}

			if (c == '{' && vp != 0)
			{
				// skip the opening { in case of variable
				continue;
			}

			if (c == '}' && vp != 0)	// variable expansion occurs
			{
				// take the variable value
				char* env = get_var(vp);
				if (env != 0 && env[0] != 0)
				{
					const int sz = sizeof(buf) - (vp - buf);
					strncpy(vp, env, sz - 1);
					d = buf + strlen(env);
				}
				else
				{
					// clear/reset the buffer
					memset(buf, 0, sizeof(buf));
					d = buf;
				}
				vp = 0;
				continue;
			}

			if (is_separator(c))
			{
				if ((esc_slash || esc_dblqte))
				{
					// if in escape mode, take this character
					*d++ = c;	// take the character
					continue;
				}

				if (is_spaces_or_null(buf))
				{
					// skip white space
					continue;
				}

				if (vp != 0)	// this is where variable expansion is happening
				{
					char* env = get_var(vp);
					if (env != 0 && env[0] != 0)
					{
						const int sz = sizeof(buf) - (vp - buf);
						memset(vp, 0, sz);
						strncpy(vp, env, sz - 1);
					}
					vp = 0;
				}
				first = add_node(&first, buf, 0);
				vp = 0;
				// is_variable = 0;	// always reset the is_variable flag when storing a node
				memset(buf, 0, sizeof(buf));	// clear the buffer
				d = buf;
			}
			else
			{
				*d++ = c;	// take the character
				continue;
			}

			// continue;
		}
		if (buf[0] != 0 && !is_spaces_or_null(buf))
		{
			if (vp != 0)
			{
				char* env = get_var(vp);
				if (env != 0 && env[0] != 0)
				{
					const int sz = sizeof(buf) - (vp - buf);
					memset(vp, 0, sz);
					strncpy(vp, env, sz - 1);
				}
			}
			first = add_node(&first, buf, 0);
			// is_variable = 0;	// always reset the is_variable flag when storing a node
		}
		//print_nodes(first);
		process_nodes(first);
		free_nodes(first);

		if (strcmp(input_buf, "showenv") == 0)
		{
			// if showvar <cmd> is true then print that var
			// else print all i.e. showvar ONLY
			print_env_vars();
		}
	}
	return 0;
}

// Used for Internal Commands

void vars_on_start()
{
	add_or_change_vars("PATH", getenv("PATH"));
	add_or_change_vars("PROMPT", shellname);
	add_or_change_vars("CWD", getcwd(CWD, sizeof(CWD)));
	add_or_change_vars("USER", getenv("USER"));
	add_or_change_vars("HOME", getenv("HOME"));
	add_or_change_vars("SHELL", "TO/CHANGE");
	add_or_change_vars("TERMINAL", ttyname(STDIN_FILENO));
	add_or_change_vars("EXITCODE", EXITCODE);
}

int add_or_change_vars(char* shellVarName, char* shellVarContent)
{
	// first run tries to replace
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)
		{
			continue;	// empty slot - ignore it
		}
		if (strcmp(g_vars[i].name, shellVarName) == 0)
		{
			// replacement
			free(g_vars[i].value);
			g_vars[i].value = strdup(shellVarContent);
			return 1;
		}
	}
	// addition (as no replacement was done)
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name == 0)	// empty
		{
			g_vars[i].name = strdup(shellVarName);
			g_vars[i].value = strdup(shellVarContent);
			return 1;
		}
	}
	return 0;
}

void print_all_vars()
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name != 0 && g_vars[i].name[0] != 0)
		{
			printf("%s=%s\n", g_vars[i].name, g_vars[i].value);
		}
	}
}

void print_specific_var(char* var_name)
{
	for (int i = 0; i < g_var_count; ++i)
	{
		if (g_vars[i].name != 0 && g_vars[i].name[0] != 0 && strcmp(g_vars[i].name, var_name) == 0)
		{
			printf("%s=%s\n", g_vars[i].name, g_vars[i].value);
			break;
		}
	}
}

void print_env_vars()
{
	char** s = environ;

	for (; *s; s++)
	{
		printf("%s\n", *s);
	}
}