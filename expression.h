int table_index(tToken token);
void reduce_by_rule(tStack *tmp_stack);
void opt_reduce();
void opt_switch();
void insert_list(tTList **list, tToken token);
void copy_list_to_stack(tTList *list);
void opt_eq();
void pars_expression();
void parse_concatenation();