#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodes.h"
#include "errors.h"

#define HASHSIZE 1000

// symbol table utils
struct symbol symtable[HASHSIZE];

static unsigned hash(char* sym)
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
    struct symbol* symbolPtr = & symtable[hash(s) % HASHSIZE];
    int i = HASHSIZE;
    while(i-- >= 0) {
        if((symbolPtr->name == NULL) || (strcmp(symbolPtr->name, s) == 0)) {
            return symbolPtr;
        }
        if(symbolPtr++ >= symtable + HASHSIZE) {
            symbolPtr = symtable;
        }
    }
    fprintf(stderr, "Symbol table overflow.\n");
    exit(3);
}

// ast node constructors
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
        new_error(OUT_OF_MEMORY);
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
        new_error(OUT_OF_MEMORY);
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
        new_error(UNDECLARED_IDENT, symname);
        exit(2);
    }
    struct symref* node = malloc(sizeof(struct symref));
    if (node == NULL) {
        new_error(OUT_OF_MEMORY);
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
        new_error(UNDECLARED_IDENT, symname);
        exit(2);
    }
    struct symasgn* node = malloc(sizeof(struct symasgn));
    if (node == NULL) {
        new_error(OUT_OF_MEMORY);
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
        new_error(OUT_OF_MEMORY);
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
        new_error(OUT_OF_MEMORY);
        exit(1);
    }
    node->nodetype = nodetype;
    node->cond = cond;
    node->doif = bi;
    node->doelse = be;
    return (struct ast*) node;
}

struct ast* newdef(struct symlist* syml) {
    struct symlist* head = syml;
    while (syml != NULL) {
        #ifdef DEBUG
        printf("\tdefine symbol: %s\n", syml->symname);
        #endif
        struct symbol* sym = lookup(syml->symname);
        if (sym->name != NULL) {
            new_error(DUPLICATE_DECL, sym->name);
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

// symbol list util
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

