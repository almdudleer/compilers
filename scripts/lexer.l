/* recognize tokens */
%option noyywrap nodefault yylineno
%{
#include "../src/funcs.h"
#include "../src/errors.h"
#include "../out/parser.tab.h"
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
">" |
"<" |
"=" |
"/" |
"(" |
")"                                               { return yytext[0]; }
"if"                                              { return IF; }
"else"                                            { return ELSE; }
"begin"                                           { return BEG; }
"end"                                             { return END; }
"var"                                             { return VAR; }
"print"                                           { return PRINT; }  
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?                                  { yylval.d = atof(yytext); return NUM; }
";"                                               { return SEP; }
[ \t\n]                                             { /* ignore white space */ }
[a-zA-Z_][a-zA-Z0-9_]*                            { yylval.str = strdup(yytext); return NAME; }
.                                                 { new_error(BAD_CHAR, *yytext); }
%%

