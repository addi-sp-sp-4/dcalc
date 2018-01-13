#include "stack.h"
#include <gmp.h>

#ifndef DCALC_EVALUATOR_H
#define DCALC_EVALUATOR_H

char *evaluate_expression(parse_node *expression_node);
void evaluate(mpf_t rop, mpf_t lhand, mpf_t rhand, char *operator);

#endif
