// kniznica pre lexikalny analyzator

// vycet stavov
// state 0 je pociatocny stav
// state 1 je po preskoceni komentarov a WS
enum comment { 	LC = 10,	// line comment
		BC,		// block comment
		END_C,		// end of block comment	
		TMP_C,		// temporary state for BC
		COMM		// line comment after END_C
};
enum number { 	ZERO = 100,	// FS: zero
		IL,		// FS: integer literal
		FL,		// FS: floating literal
		TMP_FL,		// temporary state for FL
		TMP_F_EX,	// temporary state for FL with exponent
		TMP_I_EX,	// temporary state for IL with exponent
		SIGN_F,		// signed exponent for FL
		SIGN_I,		// signed exponent for IL
		FL_EX,		// FS: floating literal with exponent
		IL_EX		// FS: integer literal with exponent
};
enum ident { 	ID = 1000,	// FS: identifier
		ID_F 		// FS: identifier of function
};
enum string { 	SL = 10000,	// FS: string literal
		TMP_SL,		// temporary state for SL
		ES,		// escape sequence
		X_ES,		// hexadecimal escape sequence \xh
		XHH_ES		// hexadecimal escape sequence \xhh
};


// ======= LEXICAL ERROR =======
#define L_ERR     -1


// ======= TYPES OF TOKEN =======
#define IDF     0
#define ID      1
#define INTEGER 2
#define FLOAT   3
#define STRING  4

#define END_OF_FILE 5

// KEYWORD
#define DEF   10
#define DO    11
#define ELSE  12
#define END   13
#define IF    14
#define NOT   15
#define NIL   16
#define THEN  17
#define WHILE 18

// OPERATOR
#define ASSIGMENT     20 // '='
#define PLUS          21 // '+'
#define MINUS         22 // '-'
#define ASTERISK      23 // '*'
#define SLASH         24 // '/'
#define LESS_THAN     25 // '<'
#define GREATER_THAN  26 // '>'
#define LESS_EQUAL    27 // '<='
#define GREATER_EQUAL 28 // '>='
#define EQUAL         29 // '=='
#define NOT_EQUAL     30 // '!='

// SPECIAL
#define OPEN_PARENTH  40 // '('
#define CLOSE_PARENTH 41 // ')'
#define OPEN_BRACKET  42 // '['
#define CLOSE_BRACKET 43 // ']'
#define OPEN_BRACE    44 // '{'
#define CLOSE_BRACE   45 // '}'
#define COMMA         46 // ','

//hlavicka funkcie simulujuca lexikalny analyzator
//void set_source_file(FILE *f);
//int get_token();
