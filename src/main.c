#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "errors.h"
#include "tree.h"

int printflag = 0;

int yyparse (void);

int main(int argc, char **argv)
{
    int c = 0;

    while ((c = getopt (argc, argv, "p:")) != -1) {
        switch(c) {
            
            case 'p':
                printflag = 1;
                break;

            case '?':
                if (isprint(optopt)) {
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
    printf ("printflag = %d\n", printflag);
#endif

    return yyparse();
}
