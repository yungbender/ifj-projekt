#include "parser.h"

#define OK 0

/**
 * Main function which calls all compiler functions. Firstly sets correct input from where is input source code read. Parses it. And if parsing 
 * is okay, calls code generator which generates output ifjcode18 code.
 * @return Function returns 0 if it went well.
 **/
int main()
{
    set_source_file(stdin);

    parse();

    generate_code();

    return OK;
}