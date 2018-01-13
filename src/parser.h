#ifndef DCALC_PARSER_H
#define DCALC_PARSER_H

extern char *parse_t_strings[];
extern char *operators[];
extern unsigned  precedences[];

typedef enum parse_t
{
	EXPRESSION,
	OPERAND,
	OPERATOR,
    IMMEDIATE_MINUS,
	NONE
	
} parse_t;

typedef struct parse_node parse_node;

struct parse_node
{
	parse_node *parent;
	unsigned children_count;
	parse_node *children;
	parse_t type;
	char *token;
};

parse_node *parse(char *expression, parse_t type);
parse_node *parse_expression(char *expression, parse_node *current);

int indexof_cp(char *haystack, char needle);
int indexof_cparr(char *haystack[], char *needle);
#endif
