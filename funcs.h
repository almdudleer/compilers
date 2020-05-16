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
    int nodetype;         /* type Const - C */
    double number;
};

struct symbol { /* a variable name */
    char *name;
    double value;
};

struct symref {
    int nodetype; /* type N */
    struct symbol *s;
};

struct symasgn {
    int nodetype; /* type = */
    struct symbol *s;
    struct ast *v; /* value */
};

struct symlist {
    struct symbol *sym;
    struct symlist *next;
};


/* build an AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newnum(double d);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
struct ast *newrt(struct symlist *syml, struct ast *l);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* evaluate an AST */
double eval(struct ast *);

/* delete and free an AST */
void treefree(struct ast *);

struct symbol *lookup(char*);

