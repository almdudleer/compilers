/* interface to the lexer */
extern int yylineno; /* from lexer */
void yyerror(char *s, ...);

/* nodes in the Abstract Syntax Tree */
struct ast {
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct numval {
    int nodetype;         /* type Const - n */
    double number;
};

struct symbol { /* a variable name */
    char *name;
    double value;
};

struct symref {
    int nodetype; /* type N */
    struct symbol *sym;
};

struct symasgn {
    int nodetype; /* type = */
    struct symbol *sym;
    struct ast *newval; /* value */
};

struct symlist {
    char* symname;
    struct symlist *next;
};

struct flow {
    int nodetype; /* type = I */
    struct ast *cond; /* condition */
    struct ast *doif; /* if do branch */
    struct ast *doelse; /* optional else branch */
};


/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newref(char* symname);
struct ast *newasgn(char* symname, struct ast *newval);
struct symlist *newsymlist(char* symname, struct symlist *next);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* print AST */
void print_tree(struct ast * a, int level);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

void dodef(struct symlist* syml);

struct symbol *lookup(char*);

