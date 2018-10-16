#define STR_LEN_INC 128
// konstanta STR_LEN_INC udava, na kolko bytov sa vykona pociatocna alokaca pamete
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla 

#define STR_ERROR   1
#define STR_SUCCESS 0

typedef struct {
	char* str;	// miesto pre dany retezec ukonceny znakom '\0'
	int length;	// skutocna dlzka retazca
	int allocSize;	// velkost alokovanej pamete
} string;


int str_init(string *s);
void str_free(string *s);

void str_clear(string *s);
int str_add_char(string *s1, char c);
int str_copy_string(string *s1, string *s2);
int str_cmp_string(string *s1, string *s2);
int str_cmp_const_str(string *s1, char *s2);

char *str_get_str(string *s);
int str_get_length(string *s);

//ADDED FUNCTIONS
//void str_replace(string *s1, char* s2);
