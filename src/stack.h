#ifndef DCALC_STACK_H
#define DCALC_STACK_H

typedef struct stack {

    unsigned int item_count; 
    char **items;

} stack;

extern void stack_init(stack *s);
extern void stack_push(stack *s, char* item);
extern char *stack_peek(stack *s);
extern char *stack_pop(stack *s);
#endif
