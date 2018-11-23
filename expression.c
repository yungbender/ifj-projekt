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
#include "parser.h"
#include "scanner.h"

// precedence table symbols
typedef enum {
	S = 500,		// < shift
	R, 			// > reduce
	E, 			// = equal
	N4,	  		// error number 4
	N2,			// error number 2
	K			// expression OK
} T_sign;

typedef enum {
	I_plus_minus,		// 0 +-
	I_mul_div,		// 1 */
	I_rel_op,		// 2 RO
	I_comp_op,		// 3 CO
	I_open_par,		// 4 (
	I_close_par,		// 5 )
	I_data,			// 6 i
	I_dollar		// 7 $
} PT_idx; //precedence table index

// precedence table
int prec_table[8][8] = {
//	   +-  */  RO  CO  (   )   i   $
	{  R,  S,  R,  R,  S,  R,  S,  R  }, // +-
	{  R,  R,  R,  R,  S,  R,  S,  R  }, // */
	{  S,  S,  N4, N4, S,  R,  S,  R  }, // RO (relation operators) < > <= >=
	{  S,  S,  N4, N4, S,  R,  S,  R  }, // CO (compare operators) == !=
	{  S,  S,  S,  S,  S,  E,  S,  N2 }, // (
	{  R,  R,  R,  R,  N2, R,  N2, R  }, // )
	{  R,  R,  R,  R,  N2, R,  N2, R  }, // i
	{  S,  S,  S,  S,  S,  N2, S,  K }  // $
};

#define DOLLAR 500	// $
#define STOP 501 	// <
#define NONTERM 502  	// nonterminal E
tToken tmp_head;	// token for saving head_stack "without nonterm"
tToken stop_token;
tToken rule_token;

//get precedence table index
PT_idx table_index(tToken token)
{
	switch (token.type)
	{
		case PLUS:
		case MINUS:
			return I_plus_minus;
		case ASTERISK:
		case SLASH:
			return I_mul_div;
		case EQUAL:
		case NOT_EQUAL:
			return I_comp_op;
		case LESS_THAN:
		case LESS_EQUAL:
		case GREATER_THAN:
		case GREATER_EQUAL:
			return I_rel_op;
		case OPEN_PARENTH:
			return I_open_par;
		case CLOSE_PARENTH:
			return I_close_par;
		case INTEGER:
		case FLOAT:
		case STRING:
		case ID:
		case NONTERM:
			return I_data;
		default:
			return I_dollar;
	}
}

 
void reduce_by_rule(tStack *tmp_stack)
{
	rule_token.type = NONTERM;
	tToken head = head_stack(tmp_stack);
	//E->i
	if(head.type == ID || head.type == INTEGER || head.type == FLOAT || head.type == STRING)
	{
		insert_instr(pData.instrs, PUSHS);
		insert_param(pData.instrs, head);
		pop_stack(tmp_stack);
		push_stack(pData.stack, rule_token);
	}
	else if(head.type == OPEN_PARENTH)
	{
		//E->(E)
		pop_stack(tmp_stack);
		head = head_stack(tmp_stack);
		if(head.type == NONTERM)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == CLOSE_PARENTH)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
			}
		}
	}
	else if(head.type == NONTERM)
	{
		pop_stack(tmp_stack);
		head = head_stack(tmp_stack);
		//E->E+E
		if(head.type == PLUS)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, ADDS);
			}
		}
		//E->E-E
		else if(head.type == MINUS)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, SUBS);
			}
		}
		//E->E*E
		else if(head.type == ASTERISK)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, MULS);
			}
		}
		//E->E/E
		else if(head.type == SLASH)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, DIVS);
			}
		}
		//E->E==E
		else if(head.type == EQUAL)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, EQS);
			}
		}
		//E->E!=E
		else if(head.type == NOT_EQUAL)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs,NOTS);
				insert_instr(pData.instrs,EQS);
			}
		}
		//E->E>E
		else if(head.type == GREATER_THAN)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, GTS);
			}
		}
		//E->E<E
		else if(head.type == LESS_THAN)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs, LTS);
			}
		}
		//E->E>=E
		else if(head.type == GREATER_EQUAL)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs,NOTS);
				insert_instr(pData.instrs,LTS);
				// NOTS LTS || GTS ORS EQS ?
			}
		}
		//E->E<=E
		else if(head.type == LESS_EQUAL)
		{
			pop_stack(tmp_stack);
			head = head_stack(tmp_stack);
			if(head.type == NONTERM)
			{
				pop_stack(tmp_stack);
				push_stack(pData.stack, rule_token);
				insert_instr(pData.instrs,NOTS);
				insert_instr(pData.instrs,GTS);
				// NOTS GTS || LTS ORS EQS ?
			}
		}
	}
	head = head_stack(tmp_stack);
	if(head.type != EMPTY)
	{
		error(UNEXPECTED_EXPR);
	}
}


// operation reduce '>'  =========================================================
void opt_reduce()
{
	tStack tmp_stack;
	init_stack(&tmp_stack);
	tToken head;
	do{	//temporary token for saving data from main stack
		tToken tmp = head_stack(pData.stack);
		//push data to temporary stack
		push_stack(&tmp_stack, tmp);
		pop_stack(pData.stack);
		head = head_stack(pData.stack);
	} while(head.type != STOP);
	pop_stack(pData.stack);
	tmp_head = head_stack(pData.stack);
	reduce_by_rule(&tmp_stack);
}

// operation switch '<'  =========================================================
void opt_switch()
{
	stop_token.type = STOP;
	rule_token.type = NONTERM;
	// check if type of top_stack is NONTERM to correct pushing STOP
	tToken top_stack = head_stack(pData.stack);
	if(top_stack.type == NONTERM)
	{
		pop_stack(pData.stack);
		push_stack(pData.stack, stop_token);
		push_stack(pData.stack, rule_token);
		push_stack(pData.stack, pData.token);
	}
	else 
	{
		push_stack(pData.stack, stop_token);
		push_stack(pData.stack, pData.token);
	}
	tmp_head = head_stack(pData.stack);
	pData.token = get_token();
	if(pData.token.type == L_ERR)
	{
		fprintf(stderr,"Lexical error, wrong lexem structure at line %d! \n",pData.token.attr.i);
		error(L_ERR);
	}
}
// operation equal '='  ==========================================================
void opt_eq()
{	
	rule_token.type = NONTERM;
	tToken check = head_stack(pData.stack);
	tToken head;
	// reduce all between open parenth and close parenth ... 
	do{
		pop_stack(pData.stack);
		head = head_stack(pData.stack);
	}while(head.type != STOP);
	// ... including stop token
	pop_stack(pData.stack);
	tmp_head = head_stack(pData.stack);
	if(check.type == NONTERM)
		push_stack(pData.stack, rule_token);
	pData.token = get_token();
}

void pars_expression()
{
	// push end of stack = dollar
	clear_stack(pData.stack);
	tToken stack_end;	
	stack_end.type = DOLLAR;
	push_stack(pData.stack, stack_end);
	tmp_head.type = DOLLAR;
	while (1) 
	{
		PT_idx activ = table_index(pData.token);
		PT_idx top = table_index(tmp_head);
		if((prec_table[top][activ]) == S)
		{	
			opt_switch();
		}
		else if((prec_table[top][activ]) == R)
		{
			opt_reduce();
		}
		else if((prec_table[top][activ]) == E)
		{
			opt_eq();
		}
		else if((prec_table[top][activ]) == N4)
		{
			error(DATA_ERR);
		}
		else if((prec_table[top][activ]) == N2)
		{
			error(UNEXPECTED_EXPR);
		}
		else
			return;
	}
}

void parse_concatenation()
{
 	while(pData.token.type != END_OF_LINE || pData.token.type != END_OF_FILE)
	{
		// string + string + string + id
		if(pData.token.type == STRING || pData.token.type == ID)
		{
			insert_instr(pData.instrs, CONCAT);
			insert_param(pData.instrs, pData.token);
			GET_TOKEN();
		}
		else
		{
			error(UNEXPECTED_TOKEN);
		}
		if(pData.token.type == PLUS)
		{
			GET_TOKEN();
		}
		else if(pData.token.type == END_OF_FILE || pData.token.type == END_OF_LINE)
		{
			break;
		}
		else
		{
			error(UNEXPECTED_TOKEN);
		}
	}
}

