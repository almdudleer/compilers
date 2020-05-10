/* recognize tokens */
%option noyywrap nodefault yylineno
%{
# include "funcs.h"
# include "y.tab.h"
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]\n             { /* ignore comment */ }
":="                                              { return ASN; }
">>"                                              { return BITR; }
"<<"                                              { return BITL; }
"+" |
"-" |
"*" |
"/" |
">" |
"<" |
"=" |
"(" |
")" |
","                                               { return yytext[0]; }
("IF"|"if"|"If")                                  { return IF; }
("ELSE"|"else"|"Else")                            { return ELSE; }
("BEGIN"|"begin"|"Begin")                         { return BEG; }
("END"|"end"|"End")                               { return END; }
("VAR"|"var"|"Var")                               { return VAR; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?                                  { yylval.d = atof(yytext); return NUM; }
\n                                                { return EOL; }
[ \t]                                             { /* ignore white space */ }
.                                                 { yyerror("Mystery character %c\n", *yytext); }
%%

