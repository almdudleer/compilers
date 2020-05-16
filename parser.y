%{
# include <stdio.h>
# include <stdlib.h>
# include "funcs.h"
int yylex();
%}

%union {
        struct ast *a;
        double d;
        char* str;
        struct symlist *sl;
    }


/* declare tokens */
%token <d> NUM // number
%token <str> NAME // variable
%token IF ELSE BEG END VAR // keywords
%token SEP // markup
%token EXIT
%token ','

%right ASN
%nonassoc '>' '<' '='
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
                   print_tree($1, 1);
                   printf("= %8.4g\n", eval($1));
                   treefree($1);
                   exit(0);
              };


program: program composite { $$ = newast('P', $1, $2); }
       | VAR symlist { $$ = newdef($2); }
       | { $$ = NULL; }
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
    | composite
    | exp
    ;

composite: BEG list END { $$ = newast('c', $2, NULL); };

exp: exp '>' exp        { $$ = newast('>', $1, $3); }
   | exp '<' exp        { $$ = newast('<', $1, $3); }  
   | exp '=' exp        { $$ = newast('=', $1, $3); } 
   | exp '+' exp        { $$ = newast('+', $1, $3); }
   | exp '-' exp        { $$ = newast('-', $1, $3);}
   | exp '*' exp        { $$ = newast('*', $1, $3); }
   | exp '/' exp        { $$ = newast('/', $1, $3); }
   | '(' exp ')'        { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
   | NAME ASN exp       { $$ = newasgn($1, $3); }
   | NAME               { $$ = newref($1); }
   | NUM                { $$ = newnum($1); }
   | exp BITR exp       { $$ = newast('r', $1, $3); }
   | exp BITL exp       { $$ = newast('l', $1, $3); }
   ;
%%

