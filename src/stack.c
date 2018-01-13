#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"

void stack_init(stack *s)
{
    s = malloc(sizeof(stack));

    s->item_count = 0;

    s->items = malloc(0);

}

void stack_push(stack *s, char *item)
{
    s->item_count++;
    s->items = realloc(s->items, sizeof(char*) * s->item_count);
    
    s->items[s->item_count - 1] = malloc(strlen(item) + 1);
    strncpy(s->items[s->item_count - 1], item, strlen(item) + 1);
}

char *stack_peek(stack *s)
{
    size_t top_length = strlen(s->items[s->item_count - 1]) + 1;
    char *top = malloc(sizeof(char) * top_length );
    strncpy(top, s->items[s->item_count - 1], top_length);

    return top;
}

char *stack_pop(stack *s)
{
    char *top = stack_peek(s);
    free(s->items[s->item_count - 1]);

    s->item_count--;
    s->items = realloc(s->items, sizeof(char*) * s->item_count);

    return top;
}
