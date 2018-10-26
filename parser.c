#include <stdio.h>
#include "parser.h"

tPData pData;

void init_parser()
{
    stack_init(pData.global);
    stack_init(pData.local); 
}

void start()
{
    // here goes parsing operations
}

int parse()
{
    init_parser();
    // get first token
    pData.token = get_token();
    start();
}

int main()
{
    return 0;
}