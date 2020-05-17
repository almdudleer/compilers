debug := 0
CFLAGS :=

comp: src/main.c src/nodes.c src/nodes.h src/eval.c src/eval.h src/errors.h src/errors.c out/lex.yy.c out/parser.tab.c
ifeq ($(debug), 1)
    CFLAGS := $(CFLAGS) -DDEBUG
endif
	gcc -o comp $(CFLAGS) src/* out/* -lfl

out/lex.yy.c: scripts/lexer.l out/parser.tab.h out/parser.tab.c
	mkdir -p out
	flex --outfile="out/lex.yy.c" scripts/lexer.l

out/parser.tab.c out/parser.tab.h: scripts/parser.y
	mkdir -p out
	bison --defines="out/parser.tab.h" --output="out/parser.tab.c" scripts/parser.y

clean: 
	rm -rf comp out

