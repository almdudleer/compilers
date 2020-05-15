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
"," |
"+" |
"-" |
"*" |
"/" |
"(" |
")"                                               { return yytext[0]; }
("IF"|"if"|"If")                                  { return IF; }
("ELSE"|"else"|"Else")                            { return ELSE; }
("BEGIN"|"begin"|"Begin")                         { return BEG; }
("END"|"end"|"End")                               { return END; }
("VAR"|"var"|"Var")                               { return VAR; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?                                  { yylval.d = atof(yytext); return NUM; }
";"                                               { return SEP; }
">"                                               { yylval.fn = 1; return CMP; }
"<"                                               { yylval.fn = 2; return CMP; }
"="                                               { yylval.fn = 3; return CMP; }
[ \t\n]                                             { /* ignore white space */ }
[a-zA-Z_][a-zA-Z0-9_]*                            { yylval.s = lookup(yytext); return NAME; }
.                                                 { yyerror("Mystery character %c\n", *yytext); }
%%

