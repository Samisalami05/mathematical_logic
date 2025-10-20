#ifndef PROP_PARSER_H
#define PROP_PARSER_H

#define SYMBOL_AND '&'
#define SYMBOL_OR 'v'
#define SYMBOL_NOT '!'

typedef enum term_type {
	CONNECTIVE_AND,
	CONNECTIVE_OR,
	CONNECTIVE_NOT,
	ATOM
} term_type;

typedef struct term {
	unsigned int index;
	unsigned int length;
	term_type type;
	// char value;
	struct term** children;
	int child_count;
} term;

term* parse_term(char* str, unsigned int index, unsigned int length);

term* parse_parentheses(char* str, unsigned int* index); 

term* parse_formula(char* str);

#endif
