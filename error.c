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
            fprintf(stderr,"Semantic error, redefinition of function! \n");
            exit(DEF_ERR);
        case UNDEF_F:
            fprintf(stderr,"Semantic error, calling undefined function! \n");
            exit(SE_ERR);
        case PARAM_NUM:
            fprintf(stderr,"Semantic error, wrong number of parameters inside function call! \n");
            exit(SE_ERR);
        case UNEXPECTED_F:
            fprintf(stderr,"Syntax error, unexpected token inside function! \n");
            exit(SY_ERR);
        case INT_ERR:
            fprintf(stderr,"Compiler internal error occured! \n");
            exit(INT_ERR);
        case UNEXPECTED_EOF:
            fprintf(stderr,"Syntax error, unexpected end of file! \n");
            exit(SY_ERR);
    }
}