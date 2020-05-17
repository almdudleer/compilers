#include "eval.h"
#include <stdio.h>
#include <stddef.h>

double eval(struct ast* a)
{
    #ifdef DEBUG
    printf("EVAL\n");
    #endif
    double v = 1.0;
    
    switch(a->nodetype) {
        case 'D': /* definintion */; break;

        case 'I': /* flow */ {
            struct flow* node = (struct flow*) a;
            if (eval(node->cond))
                v = eval(node->doif);
            else if (node->doelse != NULL)
                v = eval(node->doelse); 
            break;
        }

        case 'L': /* statement list */
        case 'P': /* program root */
            if (a->l != NULL)
                eval(a->l);
            if (a->r != NULL)
                eval(a->r);
            v = 0;
            break;

        case 'A': /* assignment */ {
            struct symasgn* node = (struct symasgn*) a;
            node->sym->value = eval(node->newval);
            v = node->sym->value;
            break;
        }

        case 'R': /* reference */ {
            struct symref* node = (struct symref*) a;
            v = node->sym->value;
            break;
        }

        case 'M': /* uminus */
            v = -eval(a->l); break;

        case 'p': /* EXIT */
            printf("%f\n", eval(a->l));
            v = 0;
            break;

        case 'c': /* composite */
            eval(a->l);
            v = 0;
            break;

        case 'n': v = ((struct numval* )a)->number; break;
        case '+': v = eval(a->l) + eval(a->r); break;
        case '-': v = eval(a->l) - eval(a->r); break;
        case '*': v = eval(a->l) * eval(a->r); break;
        case '/': v = eval(a->l) / eval(a->r); break;
        case '>': v = eval(a->l) > eval(a->r); break;
        case '<': v = eval(a->l) < eval(a->r); break;
        case '=': v = eval(a->l) == eval(a->r); break;
        case 'l': v = (int) eval(a->l) << (int) eval(a->r); break;
        case 'r': v = (int) eval(a->l) >> (int) eval(a->r); break;

        default: yyerror("internal error: bad node %c\n", a->nodetype);
    }

#ifdef DEBUG
    printf("Evaluated node type: %c\nEvaluated value: %f\n", a->nodetype, v);
#endif
    
    return v;
}   

