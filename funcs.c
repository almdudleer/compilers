#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "funcs.h"

#define HASHSIZE 1000

struct symbol symbolHashTable[HASHSIZE];

int yyparse (void);

struct ast* newast(int nodetype, struct ast* l, struct ast* r)
{
    #ifdef DEBUG
    int ltype;
    int rtype;

    if (l == NULL) ltype = 0;
    else ltype = l->nodetype;

    if (r == NULL) rtype = 0;
    else rtype = r->nodetype;


    printf("NEW AST (%c, r: %c, l: %c)\n", nodetype, ltype, rtype);
    #endif

    struct ast* node = malloc(sizeof(struct ast));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = nodetype;
    node->l = l;
    node->r = r;
    return node;
}

struct ast* newnum(double d)
{
    #ifdef DEBUG
    printf("NEW NUM (%f)\n", d); 
    #endif
    struct numval* leaf = malloc(sizeof(struct numval));
    if (leaf == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    leaf->nodetype = 'n';
    leaf->number = d;
    return (struct ast*) leaf;
}

struct ast* newref(char* symname) {
    #ifdef DEBUG
    printf("NEW REF (%s)\n", symname);
    #endif
    struct symbol* sym = lookup(symname);
    if (sym->name == NULL) {
        yyerror("Use of undeclared identifier: %s", symname);
        exit(2);
    }
    struct symref* node = malloc(sizeof(struct symref));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = 'R';
    node->sym = sym;
    return (struct ast*) node;
}

struct ast* newasgn(char* symname, struct ast* newval) {
    #ifdef DEBUG
    printf("NEW ASGN (%s, value); Value type: %c\n", symname, newval->nodetype); 
    #endif
    struct symbol* sym = lookup(symname);
    if (sym->name == NULL) {
        yyerror("Use of undeclared identifier: %s", symname);
        exit(2);
    }
    struct symasgn* node = malloc(sizeof(struct symasgn));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = 'A';
    node->sym = sym;
    node->newval = newval;
    return (struct ast*) node;
}

struct symlist* newsymlist(char* symname, struct symlist* next) {
    #ifdef DEBUG
    printf("NEW SYMLIST (%s, %p)\n", symname, (void*)next);
    #endif

    struct symlist* sl = malloc(sizeof(struct symlist));
    if (sl == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    sl->symname = symname;
    sl->next = next;
    return sl;
}

struct ast* newflow(int nodetype, struct ast* cond, struct ast* bi, struct ast* be) {
    #ifdef DEBUG
    printf("NEW flow (%d, cond: %p, if: %p, else: %p)\n", nodetype, cond, bi, be);
    #endif
    struct flow* node = malloc(sizeof(struct flow));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = nodetype;
    node->cond = cond;
    node->doif = bi;
    node->doelse = be;
    return (struct ast*) node;
}

void listfree(struct symlist* l) {
    #ifdef DEBUG
    printf("Free symlist element named %s\n", l->symname);
    #endif

    if (l->next != NULL) {
        listfree(l->next);
    }
    free(l->symname);
    free(l);
}

void treefree(struct ast* a)
{
    #ifdef DEBUG
    printf("free node %c\n", a->nodetype);
    #endif

    if (a == NULL) {
        return;
    }

    switch(a->nodetype) {
        /* define all variables */
        case 'D':
            if (((struct symdef *)a)->syml != NULL) {
                listfree(((struct symdef *)a)->syml);
            }
            free((struct symdef *)a);
            return;

        /* three subtrees */
        case 'I':
            treefree(((struct flow *)a)->cond);
            treefree(((struct flow *)a)->doif);
            if (((struct flow *)a)->doelse != NULL) {
                treefree(((struct flow *)a)->doelse);
            }
            free((struct flow *)a);
            return;

        /* list of composite */
        case 'L':
            if (a->l != NULL) {
                treefree(a->l);
            }
            if (a->r != NULL) {
                treefree(a->r);
            }
            break;

        /* program root */
        case 'P':
            if (a->l != NULL) {
                treefree(a->l);
            }
            if (a->r != NULL) {
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

void print_tree(struct ast* a, int level) {
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
            if (a->l != NULL) {
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

void yyerror(char* s, ...)
{
    // TODO: change
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

static unsigned getHash(char* sym)
{
    unsigned int hash = 0;
    unsigned elem;
    while(elem =* sym++) { 
        hash = hash*9 ^ elem;
    }
    return hash;
}

struct symbol* lookup(char* s)
{
    struct symbol* symbolPtr = & symbolHashTable[getHash(s) % HASHSIZE];
    int i = HASHSIZE;
    while(i-- >= 0) {
        if((symbolPtr->name == NULL) || (strcmp(symbolPtr->name, s) == 0)) { 
            return symbolPtr;
        }
        if(symbolPtr++ >= symbolHashTable + HASHSIZE) {
            symbolPtr = symbolHashTable;
        }
    }
    fprintf(stderr, "Symbol table overflow.\n");
    exit(3);
}

struct ast* newdef(struct symlist* syml) {
    struct symlist* head = syml;
    while (syml != NULL) {
        #ifdef DEBUG
        printf("\tdefine symbol: %s\n", syml->symname);
        #endif
        struct symbol* sym = lookup(syml->symname);
        if (sym->name != NULL) {
            yyerror("identifier \"%s\" already defined", sym->name);
            exit(4);
        }
        sym->name = syml->symname;
        sym->value = 0;
        syml = syml->next;
    }
    struct symdef* node = malloc(sizeof(struct symdef));
    node->nodetype = 'D';
    node->syml = head;
    return (struct ast*) node;
}

int main()
{
    return yyparse();
}

