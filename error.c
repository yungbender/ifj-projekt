#include <stdio.h>
#include <stdlib.h>
#include "error.h"


void print_error_exit(int id)
{
    switch(id)
    {
        case IDF:
            fprintf(stderr,"Syntax error, expected function identificator! \n");
            exit(SY_ERR);
        case IDF_REDEF:
            fprintf(stderr,"Semantic error, Redefinition of function! \n");
            exit(DEF_ERR);
        case UNEXPECTED_F:
            fprintf(stderr,"Syntax error, unexpected token inside function! \n");
            exit(SY_ERR);
        case INT_ERR:
            fprintf(stderr,"Compiler internal error occured! \n");
            exit(INT_ERR);
    }
}