// ======= ERRORS =======
#define L_ER     1
#define SY_ERR	  2
#define DEF_ERR	  3 // not defined var,function, redefinition, etc.
#define DATA_ERR  4 // wrong data type operations (string++, etc.)
#define ARG_ERR   5 // calling parameters count != defined parameters count
#define SE_ERR	  6 // other semanthic errors
#define ZERO_DIV  9 // division by zero
#define INT_ERR	  99
// Unnoficial errors for error case
#define L_ERR 111
#define IDF_REDEF 100
#define UNEXPECTED_F 101
#define UNEXPECTED_EOF 102
#define UNDEF_F 103 // calling nonexisting function
#define PARAM_NUM 104
#define UNEXPECTED_END 105
#define UNDEF_V 106
#define WRONG_PARAM 107
// ======= TYPES OF TOKEN =======
#define IDF     0
#define ID      1
#define INTEGER 2
#define FLOAT   3
#define STRING  4

#define END_OF_FILE 5
#define END_OF_LINE 6

// KEYWORDS
#define DEF   10
#define DO    11
#define ELSE  12
#define END   13
#define IF    14
#define NOT   15
#define NIL   16
#define THEN  17
#define WHILE 18

// OPERATORS
#define PLUS          20 // '+'
#define MINUS         21 // '-'
#define ASTERISK      22 // '*'
#define SLASH         23 // '/'
#define ASSIGNMENT    24 // '='
#define EQUAL         25 // '=='
#define LESS_THAN     26 // '<'
#define LESS_EQUAL    27 // '<='
#define GREATER_THAN  28 // '>'
#define GREATER_EQUAL 29 // '>='
#define NOT_EQUAL     30 // '!='

// SPECIAL CHARACTERS
#define OPEN_PARENTH  40 // '('
#define CLOSE_PARENTH 41 // ')'
#define OPEN_BRACKET  42 // '['
#define CLOSE_BRACKET 43 // ']'
#define OPEN_BRACE    44 // '{'
#define CLOSE_BRACE   45 // '}'
#define COMMA         46 // ','

void print_error_exit(int id);