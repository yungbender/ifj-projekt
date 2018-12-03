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
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"

/*
 * Global variable for keeping track which line we're on, for more accurate error messages 
 */
int line = 0;

/*
 * Global variable indicating that we're on the beginning of line, helps us distinguish block comments
 */
bool lineBeginning = true;

/*
 * Global variable for input file
 */
FILE *source;

/**
 * Function for initialising scanner source file and setting the global variables to default value
 * @param f Contains opened input file
 */
void set_source_file(FILE *f)
{
	source = f;
	lineBeginning = true; // If we somehow changed file in the middle of program (ie. testing), we reset global variables
	line = 0;
}

/**
 * Function for initialising scanner source file and setting the global variables to default value
 * @param f Contains opened input file
 */
void is_key_word (struct token *LEX)
{
	if(str_cmp_const_str(&LEX->attr.str, "nil") == 0) 
	{
		LEX->type = NIL;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "def") == 0) 
	{
		LEX->type = DEF;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "do") == 0) 
	{
		LEX->type = DO;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "else") == 0) 
	{
		LEX->type = ELSE;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "end") == 0) 
	{
		LEX->type = END;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "if") == 0) 
	{
		LEX->type = IF;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "then") == 0) 
	{
		LEX->type = THEN;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "while") == 0) 
	{
		LEX->type = WHILE;
	} 
	else if(str_cmp_const_str(&LEX->attr.str, "not") == 0) 
	{
		LEX->type = NOT;
	} 
	else 
	{
		return; 
	}

	str_free(&LEX->attr.str); // If we changed id to keyword, we dont need the string
}

/*
 * Function analyzes and returns next token, returns ID and value/line number, if possible
 * @return Functions returns next token from input file
 */
struct token get_token()
{
	struct token T1; // Made for storing token type and data
	int state = 0; // Integer containing current state in FSM
	int c; // Last loaded character from input file
	int i = 0; 
	unsigned int convertedDecimal; // Unsigned value of converted escape sequence
	char decimal[3] = {0,}; // Just for storing numbers when converting hexadecimal escape sequence to decimal
	char hexa[2] = {0,}; // Used to store hexadecimal escape sequence, we need it so we can convert it into decimal

	string attr; // The dynamic string contains identifier names and string values
	if(str_init(&attr) == STR_ERROR) // Error when allocating memory for string
	{
		T1.type = INT_ERR;
		return T1;
	}

	while(1)
	{

		c = getc(source); // Next character
		switch(state)
		{
			case 0: // Awaiting non-whitespace or comment, initial state
				if(c == '\n' && lineBeginning == false) // The line ends, returning EOL token
				{
					lineBeginning = true;
					T1.type = END_OF_LINE;
					T1.attr.i = line;
					str_free(&attr);
					return T1;	
				}
				else if(lineBeginning == true && c != EOF) // Yet we haven't looked for =begin string
				{
					line++;
					lineBeginning = false;

					if(c == '=') // Checking if block comment is starting
					{
						c = getc(source);
						for(i = 0; i < 5 && c == "begin"[i]; i++)
						{
							c = getc(source);
						}
						if(i == 5) // =begin string found
						{
								
							ungetc(c, source);
							if(isspace(c))
							{
								state = BC;	
							}
							else
							{
								T1.type = L_ERR;
								T1.attr.i = line;
								str_free(&attr);
								return T1;
							}
						}
						else // =begin string wasnt found, error
						{
							T1.type = L_ERR;
							T1.attr.i = line;
							str_free(&attr);
							return T1;
						}
					}
					else
					{
						ungetc(c, source);
						state = 0;
					}
				}
				else if(c == '#') // Line comment
				{
					state = LC; 
				}
				else if(isspace(c))	// White space
				{
					state = 0; 
				}
				else if(c == EOF) // End of file
				{
					T1.type = END_OF_FILE;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}  
				else // Non-whitespace or comment, moving to state 1
				{ 
					state = 1;
					ungetc(c, source);
				}
				break;

			case LC: // Line comment, just waiting for EOL or EOF
				if(c == '\n' || c == EOF)
				{
					ungetc(c, source);
					state = 0;
				}
				break;

			case BC: // Block comment
				if(c == '\t' || c == ' ')
				{
					state = TMP_C;
				}
				else if(c == '\n') // Checking for empty comment
				{
					line++;
					c = getc(source);
					for(i = 0; i < 3 && c == "=end"[i] && c != EOF; i++)
					{
						c = getc(source);
					}
					if(i == 3)
					{
						c = getc(source);
						if(isspace(c) || c == EOF)
						{
							ungetc(c, source);
							state = END_C;
						}
						else
						{
							state = TMP_C;
						}
					}
					else
					{
						state = TMP_C;
					}
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_C: // Temporary state for BC
				if(c == '\n')
				{
					line++;
					c = getc(source);
					if(c == '\n') // Needed for having correct line number
					{
						line++;
						break;
					}
					for(i = 0; i < 3 && c == "=end"[i] && c != EOF; i++) // Checking for the =end string on the beginning of each line
					{
						c = getc(source);
					}
					if(i == 3)
					{
						c = getc(source);
						if(isspace(c) || c == EOF)
						{
							state = END_C;
						}
						ungetc(c, source);
					}
				}
				else if(c == EOF)
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case END_C: // End of block comment	
				if(c == '\n' || c == EOF) // Comment ends
				{
					state = 0;
					ungetc(c, source);
				}
				else if(c == '\t' || c == ' ') // Comment continues on the same line
				{
					state = LC;
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case 1: // State after whitespace/comment
				if(islower(c) || c == '_')
				{
					str_add_char(&attr, c); // Save the first letter
					state = ID_STATE; // Identifier
				}
				else if(c == '0')
				{
					 state = ZERO; // Zero number state
				}
				else if(isdigit(c))
				{
					str_add_char(&attr, c); // Save the first number
					state = IL; // Integer literal
				}
				else if(c == '\"')
				{
					state = TMP_SL; // String literal
				}
				else if(c == EOF)
				{
					T1.type = END_OF_FILE;
					str_free(&attr);
					return T1;
				}
				else // Special character or error
				{
					// Testing if c is unique one-letter operator
					if(c == '+') 
					{
						T1.type = PLUS;
					}
					else if(c == '-') 
					{
						T1.type = MINUS;
					}
					else if(c == '*') 
					{
						T1.type = ASTERISK;
					}
					else if(c == '/') 
					{
						T1.type = SLASH;
					}
					// Testing if c is special character
					else if(c == '(') 
					{
						T1.type = OPEN_PARENTH;
					}
					else if(c == ')') 
					{
						T1.type = CLOSE_PARENTH;
					}
					else if(c == ',') 
					{
						T1.type = COMMA;
					}
					//Testing if c is non-unique one or two letter operator
					else if(c == '=')
					{
						c = getc(source);
						if(c == '=') 
						{
							T1.type = EQUAL;
						}
						else
						{
							T1.type = ASSIGNMENT;
							ungetc(c, source);
						}

					}
					else if(c == '<')
					{
						c = getc(source);
						if(c == '=') 
						{
							T1.type = LESS_EQUAL;
						}
						else
						{
							T1.type = LESS_THAN;
							ungetc(c, source);
						}

					}
					else if(c == '>')
					{
						c = getc(source);
						if(c == '=') 
						{
							T1.type = GREATER_EQUAL;
						}
						else
						{
							T1.type = GREATER_THAN;
							ungetc(c, source);
						}

					}
					else if(c == '!')
					{
						c = getc(source);
						if(c == '=') 
						{
							T1.type = NOT_EQUAL;
						}
						else
						{
							T1.type = L_ERR;
							T1.attr.i = line;
						}
					}
					else // If c is none of the above, return error
					{
						T1.type = L_ERR;
						T1.attr.i = line;
					}
					str_free(&attr);
					return T1;
				}
				break;

			case ID_STATE: // FS: identifier
				if(isalnum(c) || c == '_')
				{
					str_add_char(&attr, c);
				}
				else if(c == '!' || c == '?') // Function identifier can end with ? or !
				{
					str_add_char(&attr, c);
					T1.type = IDF;
					if(str_init(&T1.attr.str) == STR_ERROR)
					{
						T1.type = INT_ERR;
						return T1;
					}
					str_copy_string(&T1.attr.str, &attr);
					str_free(&attr);
					return T1;
				}
				else 
				{ 
					ungetc(c, source);
					T1.type = ID;
					if(str_init(&T1.attr.str) == STR_ERROR)
					{
						T1.type = INT_ERR;
						return T1;
					}
					str_copy_string(&T1.attr.str, &attr);
					str_free(&attr);
					is_key_word(&T1);
					return T1;
				}
				break;

			case ZERO: // FS: zero
				if(c == '.')
				{
					str_add_char(&attr, '0');
					str_add_char(&attr, c);
					state = TMP_FL;
				}
				else if(c == 'e' || c == 'E')
				{
					state = TMP_I_EX;
					str_add_char(&attr, '0');
					str_add_char(&attr, c);
				}
				else if(isdigit(c)) // There cant be zeros before other numbers
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				else // We return zero as standalone integer token
				{ 
					ungetc(c, source);
					T1.type = INTEGER;
					T1.attr.i = 0;
					str_free(&attr);
					return T1;
				}
				break;

			case IL: // FS: Integer literal
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else if(c == '.')
				{
					state = TMP_FL;
					str_add_char(&attr, c);
				}
				else if(c == 'e' || c == 'E')
				{
					state = TMP_I_EX;
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					T1.type = INTEGER;
					T1.attr.i = atoi(attr.str);
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_FL: // Temporary state for FL
				if(isdigit(c))
				{
					state = FL;
					str_add_char(&attr, c);
				}
				else 
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case FL: // FS: Floating literal
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else if(c == 'e' || c == 'E')
				{
					state = TMP_F_EX;
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					T1.type = FLOAT;
					T1.attr.f = strtod(attr.str, NULL);
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_F_EX: // Temporary state for FL with exponent
				if(isdigit(c))
				{
					state = FL_EX;
					str_add_char(&attr, c);
				}
				else if(c == '+' || c == '-')
				{
					state = SIGN_F;
					str_add_char(&attr, c);
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case SIGN_F: // Signed exponent for FL
				if(isdigit(c))
				{
					state = FL_EX;
					str_add_char(&attr, c);
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case FL_EX: // FS: Floating literal with exponent
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					T1.type = FLOAT;
					T1.attr.f = strtod(attr.str, NULL);
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_I_EX: // Temporary state for IL with exponent
				if(isdigit(c))
				{
					state = IL_EX;
					str_add_char(&attr, c);
				}
				else if(c == '+' || c == '-')
				{
					state = SIGN_I;
					str_add_char(&attr, c);
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case SIGN_I: // Signed exponent for IL
				if(isdigit(c))
				{
					state = IL_EX;
					str_add_char(&attr, c);
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case IL_EX: // FS: Integer literal with exponent
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					if((strtod(attr.str, NULL) - (int)strtod(attr.str, NULL)) == 0) // We test whether we created a floating point number
					{
						T1.type = INTEGER;
						T1.attr.i = strtod(attr.str, NULL); // We use strtod because atoi wont work with exponent, so we convert it and then cut the decimal part
					}
					else
					{
						T1.type = FLOAT; // Negative exponent created negative number, so we use float
						T1.attr.f = strtod(attr.str, NULL);
					} 
					
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_SL: // Temporary state for SL
				if(c == '\\') // Escape sequence
				{
					state = ES;
				}
				else if(c == '\"') // String is ending
				{
					state = SL;
				}
				else if(c > 31) // Accepting characters with ASCII value over 31
				{
					if(c == 32) // Space has to be written as escape sequence
					{
						str_add_string(&attr, "\\032");	
					}
					else if(c == '#') // Hash sign has to be written as escape sequence
					{
						str_add_string(&attr, "\\035");	
					}
					else
					{
						str_add_char(&attr, c);		
					}
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case ES: // Start of escape sequence
				if(c == 'x')
				{
					state = X_ES;
				}
				else if(c == '\"' || c == '\\' || c == 'n' || c == 't' || c == 's') // Accepted escape sequences
				{
					state = TMP_SL;
					if(c == '\"') // Saving quotation mark as ascape sequence
					{
						str_add_string(&attr, "\\034");
					}
					else if(c == '\\') // Saving backslash as escape sequence
					{
						str_add_string(&attr, "\\092");
					}
					else if(c == 'n') // Newline escape sequence
					{
						str_add_string(&attr, "\\010");
					}
					else if(c == 't') // Tab indent escape sequence
					{
						str_add_string(&attr, "\\009");
					}
					else if(c == 's') // Space escape sequence
					{
						str_add_string(&attr, "\\032");
					}
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case X_ES: // Hexadecimal escape sequence \xh
				if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) // Accepted hexadecimal alphanumericals
				{
					state = XHH_ES;
					hexa[0] = c; // We save the first digit of hexadecimal number
					hexa[1] = 0;
					hexa[2] = '\0';
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case XHH_ES: // Hexadecimal escape sequence \xhh
				if(c == '\\')
				{
					state = ES;
				}
				else if(c == '\"')
				{
					state = SL;
				}
				else if(c > 31)
				{
					state = TMP_SL;
					if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) // Accepted hexadecimal alphanumericals
					{
						hexa[1] = c; // We save the second digit of hexa number
					}
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}

				// We need to convert hexa number into decimal and store it
				sscanf(hexa, "%x", &convertedDecimal); //Convert hexa string into decimal
				sprintf(decimal, "%d", convertedDecimal); //Convert decimal number into string
				str_add_char(&attr, '\\'); // Start escape sequence
				if(atoi(decimal) < 100)
				{
					str_add_char(&attr, '0'); // If the number consists of less than 3 digits, we need to add zero 
				}
				if(atoi(decimal) < 10)
				{
					str_add_char(&attr, '0'); // If the number consists of less than 2 digits, we need to add another zero
				}
				str_add_string(&attr, decimal);

				if(!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || c == '\"' || c == '\\'))
				{
					str_add_char(&attr, c);	// If we have short hexa sequence, dont forget the letter that ended the sequence
				}				

				break;

			case SL: // FS: string literal
				ungetc(c, source);
				T1.type = STRING;
				if(str_init(&T1.attr.str) == STR_ERROR)
				{
					T1.type = INT_ERR;
					return T1;
				}
				str_copy_string(&T1.attr.str, &attr);
				str_free(&attr);
				return T1;
		}
	}
}