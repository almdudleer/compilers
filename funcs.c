
#  include <stdio.h>
#  include <stdlib.h>
#  include <stdarg.h>
#  include "funcs.h"

int yyparse (void);

struct ast * newast(int nodetype, struct ast *l, struct ast *r)
{
    printf("nodetype: %i", nodetype);
    struct ast *a = malloc(sizeof(struct ast));
        
    if(!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

struct ast * newnum(double d)
{
    printf("new number: %f", d);
    struct numval *a = malloc(sizeof(struct numval));
        
    if(!a) {
        yyerror("out of space");
        exit(0);
    }
    a->nodetype = 'C';
    a->number = d;
    return (struct ast *)a;
}

double eval(struct ast *a)
{
    double v;

    switch(a->nodetype) {
        case 'K': v = ((struct numval *)a)->number; break;

        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case '>': v = eval(a->l) > eval(a->r); break;
        case '<': v = eval(a->l) < eval(a->r); break;
        case '=': v = eval(a->l) == eval(a->r); break;
        case 'l': v = (int) eval(a->l) << (int) eval(a->r); break;
        case 'r': v = (int) eval(a->l) >> (int) eval(a->r); break;
        case 'M': v = -eval(a->l); break;
        default: printf("internal error: bad node %c\n", a->nodetype);
    }
    
    printf("Evaluated node type: %c\nEvaluated value: %f\n", a->nodetype, v);

    return v;
}

void treefree(struct ast *a)
{
    switch(a->nodetype) {

         /* two subtrees */
         case '+':
         case '-':
         case '*':
         case '/':
            treefree(a->r);

         /* one subtree */
         case 'M':
            treefree(a->l);

         /* no subtree */
         case 'C':
            free(a); 
            break;

         default: printf("internal error: free bad node %c\n", a->nodetype);
    }
}

void yyerror(char *s, ...)
{
    va_list ap;
    va_start(ap, s);

    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

int main()
{
    printf("> "); 
    return yyparse();
}

