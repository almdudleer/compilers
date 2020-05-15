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
%token SEP // markup
%token EXIT
%token ','

%nonassoc <fn> CMP
%right ASN
%left BITR BITL
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <a> exp stmt list composite program
%type <sl> symlist

%start root
%%

root: program { 
                    printf("= %8.4g\n", eval($1));
                    treefree($1);
              }
    ;

program: VAR symlist composite { $$ = newrt($2, $3); }
    | composite { $$ = newrt(NULL, $1); }
    ;


symlist: NAME { $$ = newsymlist($1, NULL); }
       | NAME ',' symlist { $$ = newsymlist($1, $3); }
       ;

list: { $$ = NULL; }
    | stmt SEP list { if ($3 == NULL)
                          $$ = $1;
                      else
                          $$ = newast('L', $1, $3);
                    }
    ;

stmt: IF '(' exp ')' stmt %prec LOWER_THAN_ELSE{ $$ = newflow('I', $3, $5, NULL); }
    | IF '(' exp ')' stmt ELSE stmt { $$ = newflow ('I', $3, $5, $7); }
    | EXIT '(' exp ')' { $$ = newast('E', $3, NULL); }
    | composite { $$ = newast('c', $1, NULL); }
    | exp
    ;

composite: BEG list END { $$ = newast('d', $2, NULL); };

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

