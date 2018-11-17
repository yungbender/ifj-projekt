#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"


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
        case SAME_PARAM:
            fprintf(stderr,"Semantic error, parameters cannot have same name! \n");
            exit(DEF_ERR);
        case UNDEF_F:
            fprintf(stderr,"Semantic error, calling undefined function! \n");
            exit(SE_ERR);
        case PARAM_NUM:
            fprintf(stderr,"Semantic error, wrong number of parameters inside function call! \n");
            exit(SE_ERR);
        case UNDEF_V:
            fprintf(stderr,"Semtantic error, using undefined variable! \n");
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
        case UNEXPECTED_END:
            fprintf(stderr,"Syntax error, unexpected token after keyword END! \n");
            exit(SY_ERR);
        case WRONG_PARAM:
            fprintf(stderr,"Syntax error, unexpected token inside function call! \n");
            exit(SY_ERR);
        case L_ERR:
            fprintf(stderr,"Lexical error, wrong lexem structure! \n");
            exit(L_ER);
        case COND_ERR:
            fprintf(stderr,"Semantic error, wrong data type in condition! \n");
            exit(DATA_ERR);
        case EXPECTED_DO:
            fprintf(stderr, "Syntax error, expecting do after condition in while \n");
            exit(SY_ERR);
        case UNEXPECTED_W:
            fprintf(stderr, "Syntax error, unexpected token after keyword do \n");
            exit(SY_ERR);
        case UNEXPECTED_IF:
            fprintf(stderr,"Syntax error, unexpected token inside if! \n");
            exit(SY_ERR);
        case UNEXPECTED_ELSE:
            fprintf(stderr,"Syntax error, unexpected else! \n");
            exit(SY_ERR);
    }
}