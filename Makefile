comp: funcs.c funcs.h lex.yy.c y.tab.c 
	gcc -o comp funcs.c lex.yy.c y.tab.c -lfl

lex.yy.c: lexer.l y.tab.c
	flex lexer.l

y.tab.c: parser.y
	bison -dy parser.y

clean: 
	rm comp y.output y.tab.c y.tab.h lex.yy.c 2>/dev/null

