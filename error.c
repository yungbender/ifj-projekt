/************************************************************************
 * 
 * Compiler implementation for imperative programming language IFJ18
 * 
 * Autors:
 * Sasák Tomáš - xsasak01
 * Venkrbec Tomáš - xvenkr01
 * Krajči Martin - xkrajc21
 * Dižová Natália - xdizov00 
 * 
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "scanner.h"

/**
 * Function which based on error id, will generate defined error and close the compiler.
 * @param id Number of error.
 */
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
            fprintf(stderr,"Semantic error, using undefined variable! \n");
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
        case EXPECTED_EOL:
            fprintf(stderr, "Syntax error, expected EOL after keyword \n");
            exit(SY_ERR);
        case UNEXPECTED_IF:
            fprintf(stderr,"Syntax error, unexpected token inside if condition! \n");
            exit(SY_ERR);
        case EXPECTED_ELSE:
            fprintf(stderr,"Syntax error, expected else keyword! \n");
            exit(SY_ERR);
        case EXPECTED_END:
            fprintf(stderr,"Syntax error, expected end keyword! \n");
            exit(SY_ERR);
        case UNDEFINED_VAR:
            fprintf(stderr,"Semantic error, undefined variable inside expression or function call!\n");
            exit(DATA_ERR);
        case UNEXPECTED_FUN:
            fprintf(stderr,"Semantic error, unexpected function call inside expression!\n");
            exit(SE_ERR);
        case UNEXPECTED_EXPR:
            fprintf(stderr,"Syntax error, unexpected token in expression! \n");
            exit(SY_ERR);
        case UNEXPECTED_TOKEN:
            fprintf(stderr,"Syntax error, unexpected token! \n");
            exit(SY_ERR);
        case INCOMPATIBLE_EXPR:
            fprintf(stderr,"Semantic error, wrong data type in expression! \n");
            exit(DATA_ERR);
        case WRONG_OP_CONCAT:
            fprintf(stderr, "Semantic error, wrong operations inside concatenation! \n");
            exit(DATA_ERR);
        case EXPECTED_EXPR:
            fprintf(stderr,"Syntax error, missing expression! \n");
            exit(SY_ERR);
        case ID_REDEF:
            fprintf(stderr, "Semantic error, redefinition of variable! \n");
            exit(DEF_ERR);
    }
}
