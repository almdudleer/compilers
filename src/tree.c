#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

double eval(struct ast* a)
{
    if (!a) {
        return 0;
    }

    #ifdef DEBUG
    printf("EVAL node (type %c)\n", a->nodetype);
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
            if (a->l)
                eval(a->l);
            if (a->r)
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

        case 'M': /* unminus */
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

        default: fprintf(stderr, "internal error: bad node %c\n", a->nodetype);
    }

#ifdef DEBUG
    printf("Evaluated node type: %c\nEvaluated value: %f\n", a->nodetype, v);
#endif
    
    return v;
}

void print_tree(struct ast* a, int level) {
    
    if (printflag == 0) {
        return;
    }

    printf("%i. ", level);
    level++;

    if(!a) {
        printf("Tree is empty.\n");
        return;
    }

    switch(a->nodetype) {
        case 'P':
            printf("Program root\n");
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case 'D':
            printf("Symbols definition\n");
            break;
        case 'R':
            printf("Reference to %s\n", ((struct symref *)a)->sym->name);
            break;
        case 'n':
            printf("Number %f\n", ((struct numval *)a)->number);
            break;
        case 'A':
            printf("Assign %s := (value type %c)\n", ((struct symasgn *)a)->sym->name, ((struct symasgn *)a)->newval->nodetype);
            printf("%i. Variable name %s\n", level, ((struct symasgn *)a)->sym->name);
            print_tree(((struct symasgn *)a)->newval, level);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '>':
        case '<':
        case '=':
            printf("Binary Operation %c (left value type %c, right value type %c)\n", a->nodetype, a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case 'l':
            printf("Binary Operation << (left value type %c, right value type %c)\n", a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case 'r':
            printf("Binary Operation >> (left value type %c, right value type %c)\n", a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case 'M':
            printf("Unary Minus (argument type %c)\n", a->l->nodetype);
            print_tree(a->l, level);
            break;
        case 'I':
            printf("If statement with");
            if (((struct flow *)a)->doelse == NULL) {
                printf("out");
            }
            printf(" else part (condition type %c; if branch type %c", ((struct flow *)a)->cond->nodetype, ((struct flow *)a)->doif->nodetype);
            if (((struct flow *)a)->doelse != NULL) {
                printf("; else branch type %c", ((struct flow *)a)->doelse->nodetype);
            }
            printf(")\n");
            print_tree(((struct flow *)a)->cond, level);
            print_tree(((struct flow *)a)->doif, level);
            if (((struct flow *)a)->doelse != NULL) {
                print_tree(((struct flow *)a)->doelse, level);
            }
            break;
        case 'c':
            printf("Composite type (Begin ... End)\n");
            if (a->l) {
                print_tree(a->l, level);
            }
            break;
        case 'L':
            printf("Sequense of several commands\n");
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case 'p':
            printf("Print with argument of type %c\n", a->l->nodetype);
            print_tree(a->l, level);
            return;
        default:
            printf("Bad nodetype %c\n", a->nodetype);
            return;
    }
}

void treefree(struct ast* a)
{
    if (!a) {
        return;
    }

#ifdef DEBUG
    printf("free node %c\n", a->nodetype);
#endif

    switch(a->nodetype) {
            /* define all variables */
        case 'D':
            if (((struct symdef *)a)->syml) {
                listfree(((struct symdef *)a)->syml);
            }
            free((struct symdef *)a);
            return;

            /* three subtrees */
        case 'I':
            treefree(((struct flow *)a)->cond);
            treefree(((struct flow *)a)->doif);
            if (((struct flow *)a)->doelse) {
                treefree(((struct flow *)a)->doelse);
            }
            free((struct flow *)a);
            return;

            /* list of composite */
        case 'L':
            if (a->l) {
                treefree(a->l);
            }
            if (a->r) {
                treefree(a->r);
            }
            break;

            /* program root */
        case 'P':
            if (a->l) {
                treefree(a->l);
            }
            if (a->r) {
                treefree(a->r);
            }
            break;

            /* assignment */
        case 'A':
            treefree(((struct symasgn *)a)->newval);
            free((struct symasgn *)a);
            return;

            /* reference type */
        case 'R':
            free((struct symref *)a);
            return;

            /* two subtrees */
        case '+':
        case '-':
        case '*':
        case '/':
        case '>':
        case '<':
        case '=':
        case 'l':
        case 'r':
            treefree(a->r);

            /* one subtree */
        case 'M':
        case 'c':
        case 'p':
            treefree(a->l);
            break;

            /* no subtree */
        case 'n':
            free((struct numval *)a);
            return;

        default: printf("internal error: free bad node %c\n", a->nodetype);
    }

    free(a);

    return;
}
