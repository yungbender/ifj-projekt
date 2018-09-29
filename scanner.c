#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.h"


FILE *source;
void set_source_file(FILE *f){
	source = f;
}

struct token{
	int type;
	// union vrati 	i (ak je to int)
	// 		f (ak je to float)
	// 		str (ak je to string alebo const)
	union {
		int i;
		float f;
		char *str;
	} attr ;
};

// funkcia skontroluje ci ID nie je jeden z klucovych slov
void is_key_word (){

}

// funkcia nacita a vrati token
struct token get_token (){
	
	//int state = 0;
	//int c;



	struct token T1;
	T1.type = INTEGER ;
	T1.attr.i = 44;

	return T1;
}

int main (void){
	struct token LEX = get_token();	
	printf("Lexem je %d s hodnotou %d\n", LEX.type, LEX.attr.i);

	return 0;
}


