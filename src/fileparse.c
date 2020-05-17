#include <stdio.h>
#include "fileparse.h"

int yyparse (FILE*);

int callFileParse(FILE* filename) {
    printf("callFileParse called!");
    return yyparse(filename);
}
