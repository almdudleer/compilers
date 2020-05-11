%{
# include <stdio.h>
# include <stdlib.h>
# include "funcs.h"
int yylex();
%}

%union {
        struct ast *a;
        double d;
        struct symbol *s;
        struct symlist *sl;
    }


/* declare tokens */
%token <d> NUM // number
%token <s> NAME // variable
%token BITR BITL // bitwise
%token IF ELSE BEG END VAR // keywords
%token CMP // comparison
%token ASN // assignment
%token EOL SEP // markup

%type <a> exp stmt list composite
%type <sl> symlist

%start root

%%

root: 
    | VAR symlist composite    {
                                    printf("= %8.4g\n", eval($2));
                                    treefree($2);
                               }
    | composite { }
    ;

symlist: NAME { }
       | NAME ',' symlist { }
       ;

list: { $$ = NULL }
    | stmt { }
    | stmt SEP list { if ($3 == NULL)
                          $$ = $1;
                      else
                          $$ = newast('L', $1, $3);
                    }
    ;

stmt: NAME ASN exp { }
    | IF "(" exp ")" stmt { }
    | IF "(" exp ")" stmt ELSE stmt { }
    | composite { }
    | exp { } // ??????????
    ;

composite: BEG list END { };

exp: exp CMP exp    { $$ = newcmp($2, $1, $3); }
   | exp '+' exp    { $$ = newast('+', $1,$3); }
   | exp '-' exp    { $$ = newast('-', $1,$3);}
   | exp '*' exp    { $$ = newast('*', $1,$3); }
   | exp '/' exp    { $$ = newast('/', $1,$3); }
   | '(' exp ')'    { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
   | NUM            { $$ = newnum($1); }
   | NAME { $$ = newref($1); }
   | NAME '=' exp   { $$ = newasgn($1, $3); }
   | FUNC '(' explist ')' { $$ = newfunc($1, $3); }
   | NAME '(' explist ')' { $$ = newcall($1, $3); }
   | exp BITR exp    { $$ = newast('l', $1, $3); }
   | exp BITL exp    { $$ = newast('r', $1, $3); }
   ;
%%

