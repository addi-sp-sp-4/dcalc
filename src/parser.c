#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

char *parse_t_strings[] = {"EXPRESSION", "OPERAND", "OPERATOR", "IMMEDIATE_MINUS", "NONE"};

int is_digit(char c);

char *operators[] = {"^", "*", "/", "+", "-", NULL};
unsigned precedences[] = {2, 1, 1, 0, 0, 0};

parse_node *parse(char *expression, parse_t type)
{
	parse_node *top = malloc(sizeof(parse_node) * 1);
	top->parent = NULL; /* No parent for top level node */
	top->children = malloc(0);
	top->children_count = 0;
	top->type = type;

	top->token = malloc( sizeof (char) * (strlen(expression) + 1));
	strncpy(top->token, expression, strlen(expression) + 1);
	
	return parse_expression(expression, top);
}

parse_node *parse_expression(char* expression, parse_node *current)
{
	int i = 0, j = 0, k = 0, is_floating_point = 0, encountered_parentheses = 0, nodes_made = 0;

	char current_char = expression[i];
	char buffer[BUFSIZ];
    char operator[BUFSIZ];
	parse_t current_node_type;	
    parse_node *previous = NULL;
    
	while(current_char != 0)
	{
		current_node_type = NONE;

		j = 0;
        
        operator_check:
        k = 0; 
       
        /* TODO add support for multi char operators eventually */
        operator[k++] = current_char;
		if(current_node_type == NONE && indexof_cparr(operators, operator) != -1)
		{
            if(strcmp(operator, "-") == 0)
            {
                if(previous == NULL || previous->type == OPERATOR || previous->type == IMMEDIATE_MINUS)
                {
                    current_node_type = IMMEDIATE_MINUS;
                    buffer[j++] = current_char;
                    current_char = expression[++i];
                    goto operand_check;
                }
            }
            

            if( previous == NULL || previous->type == OPERATOR || previous->type == IMMEDIATE_MINUS)
            {
                printf("Operator at wrong place\n");
                return NULL;
            }

			current_node_type = OPERATOR;
			buffer[j++] = current_char;	
			current_char = expression[++i];	
		}
	    
        operand_check:
		is_floating_point = 0;
       	if(current_node_type == NONE)
        {
            
		    while(is_digit(current_char) || current_char == '.')
		    {
                if( !(previous == NULL || previous->type == OPERATOR || previous->type == IMMEDIATE_MINUS))
                {
                    printf("Operand at wrong place\n");
                    return NULL;
                }

			    current_node_type = OPERAND;

			    if(current_char == '.')
			    {
				    if(is_floating_point) 
				    {
					    printf("ENCOUNTERED DOUBLE .\n");
					    return NULL;
				    }

				    is_floating_point = 1;

			    }
		        buffer[j++] = current_char;	
			    current_char = expression[++i];

		    }
        }

        expression_check:
		encountered_parentheses = 0;
		if(current_node_type == NONE && current_char == '(')
		{
            if( !(previous == NULL || previous->type == OPERATOR || previous->type == IMMEDIATE_MINUS))
            {
                printf("Expression at wrong place\n");
                return NULL;
            }
            
			current_node_type = EXPRESSION;

			while(encountered_parentheses != -1)
			{
				current_char = expression[++i];
				
				if(current_char == ')')
				{
					encountered_parentheses--;
				}
				else if(current_char == '(')
				{
					encountered_parentheses++;
				}

				
				buffer[j++] = current_char;

				if(current_char == 0)
				{
					printf("UNBALANCED PARENS\n");
					return NULL; /* unbalanced parentheses */
				}	
			}
			buffer[j - 1] = 0;
			current_char = expression[++i];

            if(strcmp(buffer, "") == 0)
            {
                strncpy(buffer, "0", 2);
                j = 1;
            }
		}
		
		if(current_node_type == NONE)
		{
			printf("NODE IS NONE\n");
			return NULL;
		}
		buffer[j] = 0; /* terminate string */
		
		if(current_node_type == OPERAND && buffer[j-1] == '.')
		{
			printf("LAST CHAR IS DOT\n");
			return NULL;	
		}
		current->children_count++;
		current->children = realloc(current->children, sizeof(parse_node) * current->children_count);
		
		parse_node *child = &current->children[current->children_count - 1];
		
		child->parent = current;							            /* parent */	
		child->children_count = 0;							            /* children_count */
		child->children = malloc(0);					            	/* children */
		child->type = current_node_type;					            /* type   */
		child->token = malloc(sizeof(char) * (strlen(buffer) + 1));
		strncpy(child->token, buffer, strlen(buffer) + 1);              /* token  */

		if(current_node_type == EXPRESSION)
		{
			child = parse_expression(buffer, child);
			if(child == NULL) return NULL; 
		}
        
        nodes_made++;
        previous = child;

        memset(buffer, 0, strlen(buffer));
		
	}
    
    if(previous != NULL)
    if(previous->type == IMMEDIATE_MINUS || previous->type == OPERATOR)
    {
        printf("%s at wrong place\n", parse_t_strings[previous->type]);
        return NULL;
    }

	return current;
}

int is_digit(char c)
{	
	return c >= '0' && c <= '9';
}

int indexof_cp(char *haystack, char needle)
{
	int i = 0;
	while(haystack[i] != 0)
	{
		if(haystack[i] == needle)
		{
			return i;
		}
		++i;
	}
	return -1;
}

int indexof_cparr(char *haystack[], char *needle)
{
    
    int i = 0;
    while(haystack[i] != NULL)
    {

        if(strcmp(haystack[i], needle) == 0)
        {
            return i;
        }
        ++i;
    }
    return -1;
}
