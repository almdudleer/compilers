debug := 0

comp: funcs.c funcs.h lex.yy.c y.tab.c eval.c eval.h
ifneq ($(debug), 1)
	gcc -o comp funcs.c lex.yy.c y.tab.c eval.c -lfl
else
	gcc -o comp -DDEBUG funcs.c lex.yy.c y.tab.c eval.c -lfl
endif

lex.yy.c: lexer.l y.tab.c
	flex lexer.l

y.tab.c: parser.y
	bison -dy parser.y

clean: 
	rm -f comp y.output y.tab.c y.tab.h lex.yy.c

