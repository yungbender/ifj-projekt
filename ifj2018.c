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
#include "parser.h"

/**
 * Main function which calls all compiler functions. Firstly sets correct input from where is input source code read. Parses it. And if parsing 
 * is okay, calls code generator which generates output ifjcode18 code.
 * @return Function returns 0 if compilation went well.
 **/
int main()
{
    set_source_file(stdin);
 
    parse();
 
    generate_code();
 
    return OK;

}