#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "calculator.h"
#include "stack.h"
#include "evaluator.h"

void print_parse_nodes(parse_node *node, int indent_amount)
{
    
    char indent[BUFSIZ];
    
    int i;

    for(i = 0; i < indent_amount * 4; ++i)
    {
        indent[i] = '-'; 
    }
    indent[i+1] = 0;

    printf("%s%s: %s\n", indent, parse_t_strings[(int)node->type], node->token);

    for(i = 0; i < node->children_count; ++i)
    {
        print_parse_nodes(&node->children[i], indent_amount + 1);
    } 
}


char *calculate(char *expression, int pnode)
{
    
    parse_node *filled = parse(expression, EXPRESSION);

    if(filled == NULL)
    {
        printf("Syntax Error\n");
        return NULL;
    }
    
    if(pnode) print_parse_nodes(filled, 0);  

    else printf("%s\n", evaluate_expression(filled)); 


    return NULL;    
}

int main(int argc, char **argv)
{
    
    char expression[BUFSIZ];

    while(1)
    {
        printf("(calc)# ");
        if(fgets(expression, BUFSIZ, stdin) == NULL) exit(0);
        
        expression[strlen(expression) - 1] = 0;
        
        
        char* i = expression;
        char* j = expression;
        while(*j != 0)
        {
            *i = *j++;
            if(!isspace(*i)) i++;
        }
        *i = 0; 
        
        calculate(expression, argc > 1 ? 1 : 0);
        memset(expression, 0, BUFSIZ);
    }

}

