%{
#  include <stdio.h>

int yylex();

void yyerror(char *s) {
      fprintf (stderr, "%s\n", s);
     }
%}

/* declare tokens */
%token NUM
%token LPAR RPAR
%token BITR BITL
%token MUL DIV 
%token ADD SUB
%token MORE LESS EQU
%token IF ELSE
%token ASN
%token COM
%token BEG END VAR
%token OP CP
%token EOL
%token SEP

%%

calclist: 
 | calclist exp EOL { printf("= %d\n> ", $2); }
 | calclist EOL { printf("> "); } /* blank line or a comment */
 ;

exp: factor
 | exp ADD exp { $$ = $1 + $3; }
 | exp SUB factor { $$ = $1 - $3; }
 ;

factor: term
 | factor MUL term { $$ = $1 * $3; }
 | factor DIV term { $$ = $1 / $3; }
 ;

term: NUM
 | OP exp CP { $$ = $2; }
 ;
%%

int main()
{
  printf("> "); 
  yyparse();
  return 0; 
}
