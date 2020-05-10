%{
# include <stdio.h>
# include <stdlib.h>
# include "funcs.h"
int yylex();
%}

%union {
        struct ast *a;
        double d;
    }


/* declare tokens */
%token <d> NUM
%token BITR BITL
%token IF ELSE
%token ASN
%token BEG END VAR
%token EOL

%type <a> exp factor term

%%

calclist: 
    | calclist exp EOL  {
                            printf("= %8.4g\n", eval($2));
                            treefree($2);
                            printf("> ");
                        }
    | calclist EOL      { printf("> "); } /* blank line or a comment */
    ;

exp: factor
    | exp '+' factor { $$ = newast('+', $1, $3); }
    | exp '-' factor { $$ = newast('-', $1, $3); }
    | exp '>' factor { $$ = newast('>', $1, $3); } /* or I should have put these (> < =) into another type of espression? */
    | exp '<' factor { $$ = newast('<', $1, $3); }
    | exp '=' factor { $$ = newast('=', $1, $3); }
    ;

factor: term
    | factor '*' term { $$ = newast('*', $1, $3); }
    | factor '/' term { $$ = newast('/', $1, $3); }
    ;

term: NUM               { $$ = newnum($1); }
    | '(' exp ')'       { $$ = $2; }
    | '-' term          { $$ = newast('M', $2, NULL); }
    | term BITR term    { $$ = newast('l', $1, $3); }
    | term BITL term    { $$ = newast('r', $1, $3); }
    ;
%%

