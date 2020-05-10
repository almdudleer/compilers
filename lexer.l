
%{
    # include "y.tab.h"

    void yyerror(char* s);

    int yylval;

    char error_rep[128];

%}

%%
[/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]\n             { /* ignore comment */ }
";"                                               { return SEP; }
":="                                              { return ASN; }
"*"                                               { return MUL; }
"/"                                               { return DIV; }
"+"                                               { return ADD; }
"-"                                               { return SUB; }
">>"                                              { return BITR; }
"<<"                                              { return BITL; }
">"                                               { return MORE; }
"<"                                               { return LESS; }
"="                                               { return EQU; }
("IF"|"if"|"If")                                  { return IF; }
("ELSE"|"else"|"Else")                            { return ELSE; }
("BEGIN"|"begin"|"Begin")                         { return BEG; }
("END"|"end"|"End")                               { return END; }
("VAR"|"var"|"Var")                               { return VAR; }
","                                               { return COM; }
("("|"["|"{")                                     { return LPAR; }
(")"|"]"|"}")                                     { return RPAR; }
[0-9]+                                            { yylval = atoi(yytext); return NUM; }
\n                                                { return EOL; }
[ \t]                                             { /* ignore white space */ }
.                                                 { sprintf(error_rep,"Mystery character %c\n", *yytext); yyerror(error_rep); }
%%
