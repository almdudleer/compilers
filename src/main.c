#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "errors.h"
#include "fileparse.h"

int yyparse (void);

int main(int argc, char **argv)
{
    int printflag = 0;
    int fileflag = 0;
    char *filevalue = NULL;
    int c = 0;

    while ((c = getopt (argc, argv, "pf:")) != -1) {
        switch(c) {
            
            case 'p':
                printflag = 1;
                break;

            case 'f':
                fileflag = 1;
                filevalue = optarg;
                break;

            case '?':
                if (optopt == 'f') {
                    new_error(OPTION_REQUIRES_AN_ARGUMENT, optopt);
                } else if (isprint(optopt)) {
                    new_error(UNKNOWN_OPTION, optopt);
                } else {
                    new_error(UNKNOWN_OPTION_CHARACTER, optopt);
                }
                exit(1);

            default:
                new_error(UNKNOWN_OPTION_ERROR, optopt);
                exit(1);
        }
    }

#ifdef DEBUG
    printf ("printflag = %d, fileflag = %d, filevalue = %s\n", printflag, fileflag, filevalue);
#endif
    

    if (fileflag) {
        printf("went to file\n");
        FILE* fp = fopen(filevalue, "a");
        printf("fileflag worked\n");
        return callFileParse(fp);
    }
    else {
        printf("simple yyparse\n");
        return yyparse();
        printf("it worked\n");
    }
}
