#include <string.h>
#include <malloc.h>
#include "str.h"

// initialization of string
int str_init(string *s)
{
	if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
		return STR_ERROR;
	s->str[0] = '\0';
	s->length = 0;
	s->allocSize = STR_LEN_INC;
	return STR_SUCCESS;
}

// funkcia uvolni retazec z pamate
void str_free(string *s)
{
	free(s->str);
}

// funkcia vymaze obsah retazca
void str_clear(string *s)
{
	s->str[0] = '\0';
	s->length = 0;
}

// funkcia prida na koniec retazca jeden znak
int str_add_char(string *s1, char c)
{
	if (s1->length + 1 >= s1->allocSize) {    // pamet nestaci, je potreba provest realokaci
		if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
			return STR_ERROR;
		s1->allocSize = s1->length + STR_LEN_INC;
	}
	s1->str[s1->length] = c;
	s1->length++;
	s1->str[s1->length] = '\0';
	return STR_SUCCESS;
}

// funkcia kopiruje retazec s2 do s1
int str_copy_string(string *s1, string *s2)
{
	int newLength = s2->length;
	if (newLength >= s1->allocSize) {     // nedostatocna pamat, je potreba vykonat realokacia
		if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
			return STR_ERROR;
		s1->allocSize = newLength + 1;
	}
	strcpy(s1->str, s2->str);
	s1->length = newLength;
	return STR_SUCCESS;
}

// funkcia porovna retazce s1 a s2
int str_cmp_string(string *s1, string *s2)
{
	return strcmp(s1->str, s2->str);
}

// funkcia porovna retazec s1 s konstantnym retazcom s2
int str_cmp_const_str(string *s1, char* s2)
{
	return strcmp(s1->str, s2);
}

// funkcia vrati textovu cast retazca
char *str_get_str(string *s)
{
	return s->str;
}

//funkcia vrati dlzku daneho retazca
int str_get_length(string *s)
{
	return s->length;
}


/*
//ADDED FUNCTION TO REPLACE STRING
void str_replace(string *s1, char* s2)
{
	strClear(s1);
	for(int i = 0; s2[i] != '\0'; i++){
		strAddChar(s1, s2[i]);
	}
}
*/
