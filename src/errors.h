#ifndef ERRORS_H
#define ERRORS_H

typedef enum err_type {
    BAD_CHAR,
    OUT_OF_MEMORY,
    DUPLICATE_DECL,
    UNDECLARED_IDENT
} err_type;

extern int yylineno; /* from lexer */

void new_error(err_type type, ...);

void yyerror(char* s, ...);

#endif
