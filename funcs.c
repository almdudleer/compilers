#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "funcs.h"

#define HASHSIZE 1000

struct symbol symbolHashTable[HASHSIZE];

int yyparse (void);

struct ast* newrt(struct symlist *syml, struct ast *l) {
    printf("NEW RT (%p, %p)\n", syml, l);
    while (syml != NULL) {
        printf("\tdefine symbol: %s\n", syml->sym->name);
        syml = syml->next;
    }
    return l;
}

struct ast* newast(int nodetype, struct ast* l, struct ast* r)
{
    int ltype;
    int rtype;

    if (l == NULL) ltype = 0;
    else ltype = l->nodetype;

    if (r == NULL) rtype = 0;
    else rtype = r->nodetype;

    printf("NEW AST (%c, r: %c, l: %c)\n", nodetype, ltype, rtype);

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
    printf("NEW NUM (%f)\n", d); 
    struct numval* leaf = malloc(sizeof(struct numval));
    if (leaf == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    leaf->nodetype = 'n';
    leaf->number = d;
    return (struct ast*) leaf;
}

struct ast* newcmp(int cmptype, struct ast* l, struct ast* r) {
    printf("NEW CMP (cmptype, l, r); CMP type: %i\n", cmptype); 
    struct ast* node = malloc(sizeof(struct ast));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = cmptype;
    node->l = l;
    node->r = r;
    return (struct ast*) node;
}

struct ast* newref(struct symbol* s) {
    printf("NEW REF (%s)\n", s->name); 
    struct symref* node = malloc(sizeof(struct symref));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = 'N';
    node->s = s;
    return (struct ast*) node;
}

struct ast* newasgn(struct symbol* s, struct ast* v) {
    printf("NEW ASGN (%s, value); Value type: %c\n", s->name, v->nodetype); 
    struct symasgn* node = malloc(sizeof(struct symasgn));
    if (node == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    node->nodetype = '=';
    node->s = s;
    node->v = v;
    return (struct ast*) node;
}

struct symlist* newsymlist(struct symbol* sym, struct symlist* next) {
    struct symlist* sl = malloc(sizeof(struct symlist));
    if (sl == NULL) {
        yyerror("out of memory");
        exit(1);
    }
    sl->sym = sym;
    sl->next = next;
    printf("NEW SYMLIST (%s, %p)\n", sl->sym->name, (void*)sl->next);
    return sl;
}

struct ast* newflow(int nodetype, struct ast* cond, struct ast* bi, struct ast* be) {
    printf("NEW flow (nodetype, cond, l, r); flow type: %c\n", nodetype);
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

double eval(struct ast* a)
{
    printf("EVAL\n");
    double v = 1.0;

//    switch(a->nodetype) {
//        case 'K': v = ((struct numval* )a)->number; break;
//        case '+': v = eval(a->l) + eval(a->r); break;
//        case '-': v = eval(a->l) - eval(a->r); break;
//        case '*': v = eval(a->l)*  eval(a->r); break;
//        case '/': v = eval(a->l) / eval(a->r); break;
//        case '1': v = eval(a->l) > eval(a->r); break;
//        case '2': v = eval(a->l) < eval(a->r); break;
//        case '3': v = eval(a->l) == eval(a->r); break;
//        case 'l': v = (int) eval(a->l) << (int) eval(a->r); break;
//        case 'r': v = (int) eval(a->l) >> (int) eval(a->r); break;
//        case 'M': v = -eval(a->l); break;
//        case ''
//        default: printf("internal error: bad node %c\n", a->nodetype);
//    }
//    
//    printf("Evaluated node type: %c\nEvaluated value: %f\n", a->nodetype, v);

    return v;
}

void treefree(struct ast* a)
{
    printf("TREE FREE\n");
//    switch(a->nodetype) {
//
//         /* two subtrees* /
//         case '+':
//         case '-':
//         case '*':
//         case '/':
//            treefree(a->r);
//
//         /* one subtree* /
//         case 'M':
//            treefree(a->l);
//
//         /* no subtree* /
//         case 'C':
//            free(a); 
//            break;
//
//         default: printf("internal error: free bad node %c\n", a->nodetype);
//    }
}

void print_tree(struct ast* a, int level) {
    printf("%i. ", level);
    level++;

    if(!a) {
        printf("Tree is empty.\n");
        return;
    }

    switch(a->nodetype) {
        case 'N': 
            printf("Symbol %s\n", ((struct symref *)a)->s->name);
            break;
        case 'n':
            printf("Number %f\n", ((struct numval *)a)->number);
            break;
        case '=': 
            printf("Asign %s := (value type %c)\n", ((struct symasgn *)a)->s->name, ((struct symasgn *)a)->v->nodetype);
            print_tree(((struct symasgn *)a)->v, level);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
            printf("Binary Operation %c (left value type %c, right value type %c)\n", a->nodetype, a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case '1':
            printf("Binary Operation > (left value type %c, right value type %c)\n", a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case '2':
            printf("Binary Operation < (left value type %c, right value type %c)\n", a->l->nodetype, a->r->nodetype);
            print_tree(a->l, level);
            print_tree(a->r, level);
            break;
        case '3':
            printf("Binary Operation == (left value type %c, right value type %c)\n", a->l->nodetype, a->r->nodetype);
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
            if (((struct flow *)a)->doelse != NULL) {
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
        default:
            printf("Bad nodetype %c\n", a->nodetype);
            return;
    }
}

void yyerror(char* s, ...)
{
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
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
        if(symbolPtr->name && !strcmp(symbolPtr->name, s)) { 
            return symbolPtr;
        }
        if(!symbolPtr->name) {
            symbolPtr->name = strdup(s);
            symbolPtr->value = 0;
            return symbolPtr;
        }
        if(symbolPtr++ >= symbolHashTable + HASHSIZE) {
            symbolPtr = symbolHashTable;
        }
    }
    fprintf(stderr, "Overflow of hash table of symbols.\n");
    exit(1);
}

int main()
{
    return yyparse();
}

