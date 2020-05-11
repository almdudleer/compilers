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
        int fn;
    }


/* declare tokens */
%token <d> NUM // number
%token <s> NAME // variable
%token IF ELSE BEG END VAR // keywords
%token EOL SEP // markup
%token EXIT

%right ASN
%nonassoc <fn> CMP
%left BITR BITL
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <a> exp stmt list composite program
%type <sl> symlist

%start root
%%

root: program { 
                    printf("= %8.4g\n", eval($1));
                    treefree($1);
              }
    ;

program: VAR symlist composite { }
    | composite { }
    ;


symlist: NAME { }
       | NAME ',' symlist { }
       ;

list: { $$ = NULL; }
    | stmt { }
    | stmt SEP list { if ($3 == NULL)
                          $$ = $1;
                      else
                          $$ = newast('L', $1, $3);
                    }
    ;

stmt: IF '(' exp ')' stmt { }
    | IF '(' exp ')' stmt ELSE stmt { }
    | EXIT '(' exp ')' { }
    | composite { }
    | exp { } // ??????????
    ;

composite: BEG list END { };

exp: exp CMP exp        { $$ = newcmp($2, $1, $3); }
   | exp '+' exp        { $$ = newast('+', $1,$3); }
   | exp '-' exp        { $$ = newast('-', $1,$3);}
   | exp '*' exp        { $$ = newast('*', $1,$3); }
   | exp '/' exp        { $$ = newast('/', $1,$3); }
   | '(' exp ')'        { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
   | NAME ASN exp       { $$ = newasgn($1, $3); }
   | NAME               { $$ = newref($1); }
   | NUM                { $$ = newnum($1); }
   | exp BITR exp       { $$ = newast('l', $1, $3); }
   | exp BITL exp       { $$ = newast('r', $1, $3); }
   ;
%%

