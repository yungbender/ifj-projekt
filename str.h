/* 
Constant STR_LEN_INC defines how many bytes we allocate for the string during initialization
and subsequently how many bytes we allocate when we run of free space when appending the string
*/
#define STR_LEN_INC 128

#define STR_ERROR   1
#define STR_SUCCESS 0

typedef struct {
	char* str;	// Char array allocated for the string, ended with '\0'
	int length;	// Length of string
	int allocSize;	// Size of allocated memory
}string;


int str_init(string *s);
void str_free(string *s);

void str_clear(string *s);
int str_add_string(string *s1, char* c);
int str_add_char(string *s1, char c);
int str_copy_string(string *s1, string *s2);
int str_cmp_string(string *s1, string *s2);
int str_cmp_const_str(string *s1, char *s2);
int str_copy_const_string(string *s1, char *s2);
char *str_get_str(string *s);
int str_get_length(string *s);
