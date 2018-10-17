#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "scanner.h"


int line = 0; // Line counter
bool lineBeginning = true; // Global variable indicating that we're on the beginning of line
FILE *source;

/* Function for setting the input file */
void set_source_file(FILE *f)
{
	source = f;
	lineBeginning = true; // If we somehow changed file in the middle of program (ie. testing), we reset global variables
	line = 0;
}

/* Checking if identifier is key word  */
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

/* Analyzes and returns next token, returns ID and value/line number, if possible */
struct token get_token()
{
	struct token T1;
	int state = 0;
	int c;
	int i = 0;

	string attr; 
	if(str_init(&attr) == STR_ERROR) // Memory allocation for string failed
	{
		T1.type = INT_ERR;
		return T1;
	}

	while(1)
	{
		c = getc(source);
		switch(state)
		{
			case 0: // init state
				if(c == '\n' && lineBeginning == false) // New line, returning EOL token
				{
					lineBeginning = true;
					T1.type = END_OF_LINE;
					T1.attr.i = line;
					str_free(&attr);
					return T1;	
				}
				else if(lineBeginning == true && c != EOF) // looking for =begin string, line condition for comments to work on first line
				{
					line++;
					lineBeginning = false;

					if(c == '=')
					{
						c = getc(source);
						for(i = 0; i < 5 && c == "begin"[i]; i++)
						{
							c = getc(source);
						}
						if(i == 5) // =begin string found
						{
							ungetc(c, source);
							state = BC;
						}
						else // =begin string wasnt found, reverting read characters
						{
							ungetc(c, source); // We return the char
							fseek(source, (-1-i), SEEK_CUR); // And set back the position before the '=' character
							state = 1;
						}
					}
					else
					{
						ungetc(c, source);
						state = 0;
					}
				}
				else if(c == '#') 
				{
					state = LC; // line comment
				}
				else if(isspace(c))	
				{
					state = 0; // white space
				}
				else if(c == EOF)
				{
					T1.type = END_OF_FILE;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}  
				else
				{ 
					state = 1;
					ungetc(c, source);
				}
				break;

			case LC: // line comment
				if(c == '\n' || c == EOF)
				{
					ungetc(c, source);
					state = 0;
				}
				break;

			case BC: // block comment
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
						state = END_C;
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

			case TMP_C: // temporary state for BC
				if(c == '\n')
				{
					line++;
					c = getc(source);
					if(c == '\n') // Seems unnecessary, but its needed for not messing up line number
					{
						line++;
						break;
					}
					for(i = 0; i < 3 && c == "=end"[i] && c != EOF; i++) // checking for the end on the beginning of each line
					{
						c = getc(source);
					}
					if(i == 3)
					{
						state = END_C;
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

			case END_C: // end of block comment	
				if(c == '\n' || c == EOF) //comment ends
				{
					state = 0;
					ungetc(c, source);
				}
				else if(c == '\t' || c == ' ') // comment continues on the same line
				{
					state = COMM;
				}
				else
				{
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				break;

			case COMM: // line comment after END_C
				if(c == '\n' || c == EOF)
				{
					state = 0;
					ungetc(c, source);
				}
				break;

			case 1: // state after white space/comment
				if(islower(c) || c == '_')
				{
					str_add_char(&attr, c); // save the first letter
					state = ID_STATE; // Identifier
				}
				else if(c == '0')
				{
					 state = ZERO; // zero state to get rid of extra zeros
				}
				else if(isdigit(c))
				{
					str_add_char(&attr, c); // save the first number
					state = IL; // integer literal
				}
				else if(c == '\"')
				{
					str_add_char(&attr, c);
					state = TMP_SL; // string literal
				}
				else if(c == EOF)
				{
					T1.type = END_OF_FILE;
					str_free(&attr);
					return T1;
				}
				else
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
					else if(c == '[') 
					{
						T1.type = OPEN_BRACKET;
					}
					else if(c == ']') 
					{
						T1.type = CLOSE_BRACKET;
					}
					else if(c == '{') 
					{
						T1.type = OPEN_BRACE;
					}
					else if(c == '}') 
					{
						T1.type = CLOSE_BRACE;
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
				else if(c == '!' ||c == '?')
				{
					state = ID_F;
					str_add_char(&attr, c);
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

			case ID_F: // FS: identifier of function
				if(isalnum(c))
				{ // Nothing is allowed after question or exclamation mark
					T1.type = L_ERR;
					T1.attr.i = line;
					str_free(&attr);
					return T1;
				}
				else
				{
					ungetc(c, source);
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

			case TMP_FL: // temporary state for FL
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

			case FL: // FS: floating literal
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
					T1.attr.f = atof(attr.str);
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_F_EX: // temporary state for FL with exponent
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

			case SIGN_F: // signed exponent for FL
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

			case FL_EX: // FS: floating literal with exponent
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					T1.type = FLOAT;
					T1.attr.f = atof(attr.str);
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_I_EX: // temporary state for IL with exponent
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

			case SIGN_I: // signed exponent for IL
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

			case IL_EX: // FS: integer literal with exponent
				if(isdigit(c))
				{
					str_add_char(&attr, c);
				}
				else
				{
					ungetc(c, source);
					if((atof(attr.str) - (int)atof(attr.str)) == 0) // We test whether we created a floating point number
					{
						T1.type = INTEGER;
						T1.attr.i = atof(attr.str); // We use atof because atoi wont work with exponent, so we convert it and then cut the decimal part
					}
					else
					{
						T1.type = FLOAT; //Negative exponent created negative number, so we use float
						T1.attr.f = atof(attr.str);
					} 
					
					str_free(&attr);
					return T1;
				}
				break;

			case TMP_SL: // temporary state for SL
				if(c == '\\') // escape sequence
				{
					state = ES;
					str_add_char(&attr, c);
				}
				else if(c == '\"') //string ending
				{
					state = SL;
					str_add_char(&attr, c);
				}
				else if(c > 31) // accepting characters with ASCII value over 31
				{
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

			case ES: // escape sequence
				if(c == 'x')
				{
					state = X_ES;
					str_add_char(&attr, c);
				}
				else if(c == '\"' || c == '\\' || c == 'n' || c == 't' || c == 's') // Accepted escape sequences
				{
					state = TMP_SL;
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

			case X_ES: // hexadecimal escape sequence \xh
				if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F')) // Accepted hexadecimal alphanumericals
				{
					state = XHH_ES;
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

			case XHH_ES: // hexadecimal escape sequence \xhh
				if(c == '\\')
				{
					state = ES;
					str_add_char(&attr, c);
				}
				else if(c == '\"')
				{
					state = SL;
					str_add_char(&attr, c);
				}
				else if(c > 31)
				{
					state = TMP_SL;
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