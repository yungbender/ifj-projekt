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
#include <string.h>
#include <malloc.h>
#include "str.h"

/**
 * Function initalizes string before it's first use.
 * @param s Pointer to the string.
 */
int str_init(string *s)
{
	if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
		return STR_ERROR;
	s->str[0] = '\0';
	s->length = 0;
	s->allocSize = STR_LEN_INC;
	return STR_SUCCESS;
}

/**
 * Function free's out the string used memory.
 * @param s Pointer to the string.
 */
void str_free(string *s)
{
	free(s->str);
}

/**
 * Function appends char array on the end of string.
 * @param s1 Pointer to the string.
 * @param c Pointer to the string array which will be appended.
 */
int str_add_string(string *s1, char* c)
{
	int length = strlen(c);
	for(int i = 0; i < length; i++)
	{
		str_add_char(s1, c[i]);
	}
	return STR_SUCCESS;
}

/** 
 * Function appends one char on the end of string.
 * @param s1 Pointer to the string.
 * @param c Char which will be appended at the end of s1.
 */
int str_add_char(string *s1, char c)
{
	if (s1->length + 1 >= s1->allocSize) {     // Not enough memory, we need to reallocate
		if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
			return STR_ERROR;
		s1->allocSize = s1->length + STR_LEN_INC;
	}
	s1->str[s1->length] = c;
	s1->length++;
	s1->str[s1->length] = '\0';
	return STR_SUCCESS;
}

/**
 * Function copies second string into first string.
 * @param s1 Pointer to the string (destination).
 * @param s2 Pointer to the string (source).
 */
int str_copy_string(string *s1, string *s2)
{
	int newLength = s2->length;
	if (newLength >= s1->allocSize) {     // Not enough memory, we need to reallocate
		if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
			return STR_ERROR;
		s1->allocSize = newLength + 1;
	}
	strcpy(s1->str, s2->str);
	s1->length = newLength;
	return STR_SUCCESS;
}

/**
 * Function copies constant string array into string.
 * @param s1 Pointer to the string (destination).
 * @param s2 Pointer to the constant char array (source).
 */
int str_copy_const_string(string *s1, char *s2)
{
	int newLength = strlen(s2);
	if (newLength >= s1->allocSize) {     // Not enough memory, we need to reallocate
		if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
			return STR_ERROR;
		s1->allocSize = newLength + 1;
	}
	strcpy(s1->str, s2);
	s1->length = newLength;
	return STR_SUCCESS;
}

/**
 * Function compares right string with left string and returns result.
 * @param s1 Pointer to the left string.
 * @param s2 Pointer to the right string.
 */
int str_cmp_string(string *s1, string *s2)
{
	return strcmp(s1->str, s2->str);
}

/**
 * Function compares left string with constant char array and returns result.
 * @param s1 Pointer to the left string.
 * @param s2 Constant char array.
 */
int str_cmp_const_str(string *s1, char* s2)
{
	return strcmp(s1->str, s2);
}
