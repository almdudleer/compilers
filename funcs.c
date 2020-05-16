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
//    printf("NEW AST (%c, %c, %c)\n", nodetype, l->nodetype, r->nodetype);
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
    struct numval* leave = malloc(sizeof(struct numval));
    leave->nodetype = 'n';
    leave->number = d;
    return (struct ast*) leave;
}

struct ast* newcmp(int cmptype, struct ast* l, struct ast* r) {
    printf("NEW CMP (cmptype, l, r)\n Node type: %c\n", cmptype); 
    return NULL;
}

struct ast* newref(struct symbol* s) {
    printf("NEW REF (%s)\n", s->name); 
    return NULL;
}

struct ast* newasgn(struct symbol* s, struct ast* v) {
    printf("NEW ASGN (%s, value)\n", s->name); 
    return NULL;
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

struct ast* newflow(int nodetype, struct ast* cond, struct ast* tl, struct ast* tr) {
    printf("NEW flow (nodetype, cond, l, r)\nNode type: %c\n", nodetype);
    return NULL;
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

