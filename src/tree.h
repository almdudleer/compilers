#ifndef EVAL
#define EVAL
#include "nodes.h"

/* evaluate an AST */
double eval(struct ast *);

/* print AST */
void print_tree(struct ast * a, int level);

/* delete and free an AST */
void treefree(struct ast *);

#endif
