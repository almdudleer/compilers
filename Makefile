debug := 0

comp: src/main.c src/nodes.c src/nodes.h src/tree.c src/tree.h src/errors.h src/errors.c out/lex.yy.c out/parser.tab.c
ifneq ($(debug), 1)
	gcc -o comp $(CFLAGS) src/* out/* -lfl
else
	gcc -o comp -DDEBUG src/* out/* -lfl
endif

out/lex.yy.c: scripts/lexer.l out/parser.tab.h out/parser.tab.c
	mkdir -p out
	flex --outfile="out/lex.yy.c" scripts/lexer.l

out/parser.tab.c out/parser.tab.h: scripts/parser.y
	mkdir -p out
	bison --defines="out/parser.tab.h" --output="out/parser.tab.c" scripts/parser.y

clean: 
	rm -rf comp out

