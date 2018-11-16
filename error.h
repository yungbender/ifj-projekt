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
#define L_ERR	111
#define IDF_REDEF 100
#define UNEXPECTED_F 101
#define UNEXPECTED_EOF 102
#define UNDEF_F 103 // calling nonexisting function
#define PARAM_NUM 104
#define UNEXPECTED_END 105
#define UNDEF_V 106
#define WRONG_PARAM 107
#define SAME_PARAM 108
#define UNEXPECTED_IF 109
#define UNEXPECTED_ELSE 110


void print_error_exit(int id);