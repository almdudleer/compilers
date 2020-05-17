#ifndef FUNCS_H
#define FUNCS_H

// a variable, symbol table entry
struct symbol {
    char* name;
    double value;
};

// ast nodes
struct ast {
    int nodetype;
    struct ast* l;
    struct ast* r;
};

struct numval {
    int nodetype;
    double number;
};

struct symref {
    int nodetype;
    struct symbol* sym;
};

struct symasgn {
    int nodetype;
    struct symbol* sym;
    struct ast* newval;
};

struct symdef {
    int nodetype;
    struct symlist* syml;
};

struct symlist {
    char* symname;
    struct symlist* next;
};

struct flow {
    int nodetype;
    struct ast* cond;
    struct ast* doif;
    struct ast* doelse;
};

// ast node constructors
struct ast* newast(int nodetype, struct ast* l, struct ast* r);

struct ast* newnum(double d);

struct ast* newref(char* symname);

struct ast* newasgn(char* symname, struct ast* newval);

struct ast* newdef(struct symlist* syml);

struct symlist* newsymlist(char* symname, struct symlist* next);

struct ast* newflow(int nodetype, struct ast* cond, struct ast* tl, struct ast* tr);

// lookup in symbol table
struct symbol* lookup(char*);

// symbol list util
void listfree(struct symlist* l);

#endif
