#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "calculator.h"

#include "evaluator.h"

char *evaluate_expression(parse_node *expression_node)
{
    char **postfix = malloc(sizeof(char*) * 256);
    
    int postfix_count = 0;

    stack *operator_stack;

    operator_stack = malloc(sizeof(stack)); 
    
    operator_stack->item_count = 0;
    operator_stack->items = malloc(0);

    parse_node *current;
    

    for(int i = 0; i < expression_node->children_count; ++i)
    {
       current = &expression_node->children[i];

       if(current->type == EXPRESSION)
       {
            current->token = evaluate_expression(current);
            current->type  = OPERAND;       
       }
        

    }
    
    int immediate_minus_status;
    char buffer[BUFSIZ];

    char *top_token;
    for(int i = 0; i < expression_node->children_count; ++i)
    {

        immediate_minus_status = 0;

        current = &expression_node->children[i];

        while(current->type == IMMEDIATE_MINUS)
        {
            immediate_minus_status = !immediate_minus_status;
            current = &expression_node->children[++i];
        }

        if(current->type == OPERAND)
        {
            if(immediate_minus_status)
            {
                sprintf(buffer, "-%s", current->token);
            }
            else
            {
                strncpy(buffer, current->token, strlen(current->token) + 1);
            }
            
            postfix[postfix_count] = malloc(strlen(buffer) + 1);

            strncpy(postfix[postfix_count++], buffer, strlen(buffer) + 1); 
            
            memset(buffer, 0, BUFSIZ);
        }

        if(current->type == OPERATOR)
        {
            if(operator_stack->item_count == 0)
            {
                stack_push(operator_stack, current->token);
            }
            else
            {
                int top_precedence = precedences[indexof_cparr(operators, stack_peek(operator_stack))];
                
                int current_precedence = precedences[indexof_cparr(operators, current->token)];

                while(top_precedence > current_precedence)
                {
                    top_token = stack_pop(operator_stack);
                    
                    postfix[postfix_count] = malloc(strlen(top_token) + 1);
                    strncpy(postfix[postfix_count++], top_token, strlen(top_token) + 1);
                    
                    if(operator_stack->item_count == 0) break;

                    top_precedence = precedences[indexof_cparr(operators, stack_peek(operator_stack))];    
                }
                stack_push(operator_stack, current->token);
            }
        }
    }

    while(operator_stack->item_count != 0)
    {
        top_token = stack_pop(operator_stack);
        postfix[postfix_count] = malloc(strlen(top_token) + 1);
        strncpy(postfix[postfix_count++], top_token, strlen(top_token) + 1);
    }
    
    mpf_set_default_prec(4096);

    mpf_t lhand, rhand, result;
    mpf_inits(lhand, rhand, result, NULL);

    stack *evaluation_stack; 
    evaluation_stack = malloc(sizeof(stack));
    evaluation_stack->item_count = 0;
    evaluation_stack->items = malloc(0);
    
    char *result_string;

    for(int i = 0; i < postfix_count; ++i)
    {
        char *current_token = postfix[i];
        
        if(indexof_cparr(operators, current_token) != -1)
        {
            mpf_set_str(rhand, stack_pop(evaluation_stack), 10);
            mpf_set_str(lhand, stack_pop(evaluation_stack), 10);

            evaluate(result, lhand, rhand, current_token);
            
            free(result_string); 

            ssize_t bufsz = gmp_snprintf(NULL, 0, "%.8Ff", result );

            result_string = malloc(bufsz + 1);

            gmp_snprintf(result_string, bufsz + 1, "%.8Ff", result);

            stack_push(evaluation_stack, result_string);

        }
        else
        {
            stack_push(evaluation_stack, current_token);
        }
          
    }
    return stack_pop(evaluation_stack);
}


void evaluate(mpf_t rop, mpf_t lhand, mpf_t rhand, char *operator)
{
 
    if(strcmp(operator, "^") == 0)
    {
        /* unfortunately, you can only raise to the power of integers */
        unsigned long exponent = (unsigned long)mpf_get_d(rhand);
        mpf_pow_ui(rop, lhand, exponent);
        return;
    }
    
    if(strcmp(operator, "*") == 0)
    {
       mpf_mul(rop, lhand, rhand);
       return;
    }
    if(strcmp(operator, "/") == 0)
    {
        mpf_div(rop, lhand, rhand);
        return;
    }  
    if(strcmp(operator, "+") == 0)
    {
        mpf_add(rop, lhand, rhand);
        return;
    }
    if(strcmp(operator, "-") == 0) 
    {
        mpf_sub(rop, lhand, rhand);
        return;
    }

}
