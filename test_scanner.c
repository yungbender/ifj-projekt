#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scanner.c"

FILE *f;
extern int line;
extern bool lineBeginning;

void print_lex(struct token LEX)
{
	if(LEX.type == INT_ERR)
	{
		fprintf(stderr, "ERROR: Internal compiler error\n");
		//return INT_ERR;
	}
	if(LEX.type == -1)
	{
		fprintf(stderr,"ERROR: Lexical analyzer error at line: %d\n", line);
		//return 1;
	}
	else if(LEX.type == INTEGER || LEX.type == END_OF_LINE)
	{
		printf("Lexem je typu %d s hodnotou %d\n", LEX.type, LEX.attr.i);
	}
	else if(LEX.type == FLOAT)
	{
		printf("Lexem je typu %d s hodnotou %f\n", LEX.type, LEX.attr.f);
	}
	else if(LEX.type == IDF || LEX.type == ID || LEX.type == STRING)
	{
		printf("Lexem je typu %d s hodnotou %s\n", LEX.type, LEX.attr.str.str);
		str_free(&LEX.attr.str);
	}
	else
	{
		printf("Lexem je typu %d\n", LEX.type);
	}
}

void open_file(char *file)
{
	if((f = fopen(file, "r")) == NULL)
	{
		fprintf(stderr, "ERROR: Cant open file\n");
		return;
	}
	set_source_file(f);
}

int main()
{
	struct token LEX;

	open_file("tests_scanner/test01");
	puts("TEST01 -- Empty file");
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 0);
	fclose(f);

	open_file("tests_scanner/test02");
	puts("TEST02 -- Comment with text on the first and last line");
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test03");
	puts("TEST03 -- Comment without text on the first and last line");
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test04");
	puts("TEST04 -- Line comment");
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 2);
	fclose(f);

	open_file("tests_scanner/test05");
	puts("TEST05 -- Line comment in the middle of the line");
	LEX = get_token();
	assert(LEX.type == INTEGER && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test06");
	puts("TEST06 -- Line comment on the end of the line");
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "ahoj") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test07");
	puts("TEST07 -- Empty string");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test08");
	puts("TEST08 -- Normal string");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"abcdef\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test09");
	puts("TEST09 -- String with various escape sequences");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"abcdef\\\"\\\\\\n\\t\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test10");
	puts("TEST10 -- Short hexadecimal sequence");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"ahoj\\x1\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test11");
	puts("TEST11 -- Long hexadecimal sequence");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"ahoj\\x11\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test12");
	puts("TEST12 -- Short hexadecimal sequence, text after");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"ahoj\\x1testovic\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test13");
	puts("TEST13 -- Long hexadecimal sequence, text after");
	LEX = get_token();
	assert(LEX.type == STRING && str_cmp_const_str(&LEX.attr.str, "\"ahoj\\x11testovic\"") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test14");
	puts("TEST14 -- Many zeros should produce error");
	LEX = get_token();
	assert(LEX.type == -1 && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test15");
	puts("TEST15 -- Single zero");
	LEX = get_token();
	assert(LEX.type == INTEGER && LEX.attr.i == 0);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test16");
	puts("TEST16 -- Unsigned integer");
	LEX = get_token();
	assert(LEX.type == INTEGER && LEX.attr.i == 1234);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test17");
	puts("TEST17 -- Various floats beginning with zero");
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)0.01);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)0.0);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 2);
	fclose(f);

	open_file("tests_scanner/test18");
	puts("TEST18 -- Various floats beginning with integer");
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)1.1);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)1.0);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 2);
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)1.02);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test19");
	puts("TEST19 -- Many various floats with exponents beginning with integer");
	for(int i = 0; i < 6; i++)
	{
		LEX = get_token();
		assert(LEX.type == FLOAT && LEX.attr.f == (double)10.001);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 1);
	}
	for(int i = 0; i < 18; i++)
	{
		LEX = get_token();
		assert(LEX.type == INTEGER && LEX.attr.i == 10);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 7);
	}
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)0.1);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 25);
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)0.1);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 26);
	fclose(f);

	open_file("tests_scanner/test20");
	puts("TEST20 -- Many various floats with exponents beginning with zero");
	for(int i = 0; i < 6; i++)
	{
		LEX = get_token();
		assert(LEX.type == FLOAT && LEX.attr.f == (double)0.01);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 1);
	}
	for(int i = 0; i < 5; i++)
	{
		LEX = get_token();
		assert(LEX.type == INTEGER && LEX.attr.i == 0);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 7);
	}
	LEX = get_token();
	assert(LEX.type == INTEGER && LEX.attr.i == 0);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 12);
	fclose(f);

	open_file("tests_scanner/test21");
	puts("TEST21 -- Various identifiers");
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "ahoj") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "_ahoj") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 2);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "_") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test22");
	puts("TEST22 -- Various function identifiers");
	LEX = get_token();
	assert(LEX.type == IDF && str_cmp_const_str(&LEX.attr.str, "_ahoj?") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 1);
	LEX = get_token();
	assert(LEX.type == IDF && str_cmp_const_str(&LEX.attr.str, "_ahoj!") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 2);
	LEX = get_token();
	assert(LEX.type == IDF && str_cmp_const_str(&LEX.attr.str, "_AhOj!") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 3);
	LEX = get_token();
	assert(LEX.type == IDF && str_cmp_const_str(&LEX.attr.str, "_?") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 4);
	fclose(f);

	open_file("tests_scanner/test23");
	puts("TEST23 -- Keywords");
	for(int i = 0; i < 8; i++)
	{
		LEX = get_token();
		assert(LEX.type == 10 + i);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 1);
	}
	LEX = get_token();
	assert(LEX.type == 18);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 9);
	fclose(f);

	open_file("tests_scanner/test24");
	puts("TEST24 -- Operators");
	for(int i = 0; i < 10; i++)
	{
		LEX = get_token();
		assert(LEX.type == 20 + i);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 1);
	}
	LEX = get_token();
	assert(LEX.type == 30);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 11);
	fclose(f);

	open_file("tests_scanner/test25");
	puts("TEST25 -- Special characters");
	for(int i = 0; i < 6; i++)
	{
		LEX = get_token();
		assert(LEX.type == 40 + i);
		LEX = get_token();
		assert(LEX.type == END_OF_LINE && LEX.attr.i == i + 1);
	}
	LEX = get_token();
	assert(LEX.type == 46);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 7);
	fclose(f);

	open_file("tests_scanner/test26");
	puts("TEST26 -- ERROR: Wrong identifier");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test27");
	puts("TEST27 -- ERROR: Wrong function identifier");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test28");
	puts("TEST28 -- ERROR: Incomplete decimal part of number");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test29a");
	puts("TEST29 -- ERROR: Missing or wrong character in exponent part of decimal number");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test29b");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test29c");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test30a");
	puts("TEST30 -- ERROR: Missing or wrong character in exponent part of integer");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test30b");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test30c");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test31");
	puts("TEST31 -- ERROR: Wrong character in string");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test32");
	puts("TEST32 -- ERROR: Wrong escape sequence in string");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test33");
	puts("TEST33 -- ERROR: Wrong character after short hexa form");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test34");
	puts("TEST34 -- ERROR: Wrong character after long hexa form");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test35");
	puts("TEST35 -- ERROR: Wrong character after x in hexa form");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test36a");
	puts("TEST36 -- ERROR: =begin not finished");
	LEX = get_token();
	assert(LEX.type == ASSIGNMENT);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "begi") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test36b");
	LEX = get_token();
	assert(LEX.type == ASSIGNMENT);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "beg") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test36c");
	LEX = get_token();
	assert(LEX.type == ASSIGNMENT);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "be") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);
	open_file("tests_scanner/test36d");
	LEX = get_token();
	assert(LEX.type == ASSIGNMENT);
	LEX = get_token();
	assert(LEX.type == ID && str_cmp_const_str(&LEX.attr.str, "b") == 0);
	str_free(&LEX.attr.str);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test37");
	puts("TEST37 -- ERROR: =end misspelled");
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 5);
	fclose(f);

	open_file("tests_scanner/test38");
	puts("TEST38 -- ERROR: Hierarchic nesting of comments");
	LEX = get_token();
	assert(LEX.type == END_OF_LINE && LEX.attr.i == 3);
	LEX = get_token();
	assert(LEX.type == ASSIGNMENT);
	LEX = get_token();
	assert(LEX.type == END);
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 4);
	fclose(f);

	open_file("tests_scanner/test39");
	puts("TEST39 -- ERROR: Text right after =begin");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);

	open_file("tests_scanner/test40");
	puts("TEST40 -- ERROR: Text right after =end");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test41");
	puts("TEST41 -- ERROR: Comment wasn't ended");
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 3);
	fclose(f);

	open_file("tests_scanner/test42");
	puts("TEST42 -- ERROR: Decimal exponent part");
	LEX = get_token();
	assert(LEX.type == FLOAT && LEX.attr.f == (double)10.0);
	LEX = get_token();
	assert(LEX.type == L_ERR && LEX.attr.i == 1);
	fclose(f);	

	open_file("tests_scanner/test43");
	puts("TEST43 -- Empty line comment");
	LEX = get_token();
	assert(LEX.type == END_OF_FILE && LEX.attr.i == 1);
	fclose(f);

	puts("\nEverything is OK\n");
	return 0;
}
