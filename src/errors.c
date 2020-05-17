#include <stdarg.h>
#include <stdio.h>
#include "errors.h"


void print_usage() {
    printf("This compiler has 2 options:\n\t-p\tuse this to output abstract syntax tree\n");
    printf("\t-f <filename>\tuse this to read compiled program from file\n");
}

// custom errors
void new_error(err_type type, ...) {
    va_list ap;
    va_start(ap, type);
    char msg[255];
    switch (type) {
        case BAD_CHAR:
            vsprintf(msg, "Unknown character: %c", ap);
            break;
        case OUT_OF_MEMORY:
            sprintf(msg, "Out of memory");
            break;
        case DUPLICATE_DECL:
            vsprintf(msg, "Identifier \"%s\" is already defined", ap);
            break;
        case UNDECLARED_IDENT:
            vsprintf(msg, "Use of undeclared identifier: %s", ap);
            break;
        case UNKNOWN_OPTION:
            vsprintf(msg, "Unknown option: -%c\n", ap);
            print_usage();
            break;
        case OPTION_REQUIRES_AN_ARGUMENT:
            vsprintf(msg, "Option -%c requires an argument.\n", ap);
            print_usage();
            break;
        case UNKNOWN_OPTION_CHARACTER:
            vsprintf(msg, "Unknown option character: %x\n", ap);
            print_usage();
            break;
        case UNKNOWN_OPTION_ERROR:
            vsprintf(msg, "Unknown option error has occured\n", ap);
            print_usage();
            break;
    }

    fprintf(stderr, "%d: %s\n", yylineno, msg);
}

// for other errors
void yyerror(char* s, ...) {
    va_list ap;
    va_start(ap, s);
    fprintf(stderr, "%d: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

