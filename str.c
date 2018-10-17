#include <string.h>
#include <malloc.h>
#include "str.h"

// Initialization of string
int str_init(string *s)
{
	if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
		return STR_ERROR;
	s->str[0] = '\0';
	s->length = 0;
	s->allocSize = STR_LEN_INC;
	return STR_SUCCESS;
}

// Function frees the string from memory
void str_free(string *s)
{
	free(s->str);
}

// Function clears the contents of string
void str_clear(string *s)
{
	s->str[0] = '\0';
	s->length = 0;
}

// Function adds one char on the end of string s1
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

// Function copies string s2 into s1
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

// Function compares strings s1 and s2
int str_cmp_string(string *s1, string *s2)
{
	return strcmp(s1->str, s2->str);
}

// Function compares string s1 with constant char array s2
int str_cmp_const_str(string *s1, char* s2)
{
	return strcmp(s1->str, s2);
}

// Function returns the char* part from string structure
char *str_get_str(string *s)
{
	return s->str;
}

// Function returns length of string
int str_get_length(string *s)
{
	return s->length;
}

int main()
{
	return 0;
}