#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "../error_handler.h"

static void skip_whitespace(char* str, unsigned int* index);

static void skip_whitespace(char* str, unsigned int* index) {
	while (str[*index] == ' ') {
		*index += 1;
	}
}

term_type term_connective(char* str, int index) {
	switch (str[index]) {
		case SYMBOL_AND:
			return CONNECTIVE_AND;
		case SYMBOL_OR:
			return CONNECTIVE_OR;
		case SYMBOL_NOT:
			return CONNECTIVE_NOT;
		default:
			fprintf(stderr, "Can't interpret connective %c\n", str[index]);
	}
	return ATOM;
}

char term_symbol(term_type type) {
	switch (type) {
		case CONNECTIVE_AND:
			return SYMBOL_AND;
		case CONNECTIVE_OR:
			return SYMBOL_OR;
		case CONNECTIVE_NOT:
			return SYMBOL_NOT;
		default:
			return '?';
	}
}

// Returns length
unsigned int skip_term(char* str, unsigned int* index) {
	unsigned int len = 0;
	while (str[*index] != '\0' && str[*index] != ' ' && str[*index] != ')') {
		*index += 1;
		len++;
	}
	return len;
}

term* create_term(char* str, unsigned int* index) {
	term* t = malloc(sizeof(term));
				
	unsigned int term_length = skip_term(str, index);

	if (str[*index - term_length] == SYMBOL_NOT && term_length == 1) {
		t->type = CONNECTIVE_NOT;
		t->index = *index - term_length;
		t->length = term_length;
		t->child_count = 0;
		t->children = NULL;
	}
	else if (str[*index - term_length] == SYMBOL_NOT) { // Split if symbol contains ! term_connective
		if (str[*index - term_length + 1] == '(') {
			t->type = CONNECTIVE_NOT;
			t->index = *index - term_length;
			t->length = 1;
			t->child_count = 1;
			t->children = malloc(sizeof(term*) * 1);
			*index -= term_length - 2;
			t->children[0] = parse_parentheses(str, index);
		}
		else {
			term* t2 = malloc(sizeof(term));
			t2->type = ATOM;
			t2->index = *index - term_length + 1;
			t2->length = term_length - 1;
			t2->child_count = 0;
			t2->children = NULL;

			t->type = CONNECTIVE_NOT;
			t->index = *index - term_length;
			t->length = 1;
			t->child_count = 1;
			t->children = malloc(sizeof(term*) * 1);
			t->children[0] = t2;
		}
	}
	else {
		t->type = ATOM;
		t->index = *index - term_length;
		t->length = term_length;
		t->child_count = 0;
		t->children = NULL;
	}
	return t;
}


term* parse_parentheses(char* str, unsigned int* index) {
	term* sub_root = NULL;
	term* temp = NULL;

	int start_index = *index;
	int term_index = 0;
	while (str[*index] != ')' && str[*index] != '\0') {
		if (str[*index] == ' ') {
			skip_whitespace(str, index);
		}
		else {
			term* t = NULL;
			if (str[*index] == '(') {
				*index += 1;
				t = parse_parentheses(str, index);
				if (t == NULL) {
					return NULL;
				}
			}
			else {
				t = create_term(str, index);

				printf("term: index - %d, length - %d, type - %d\n", t->index, t->length, t->type);
			}

			switch (term_index) {
				case 0:
					temp = t;
					break;
				case 1: // Is a connective (excluding !)
					sub_root = t;
					t->type = term_connective(str, t->index);
					t->child_count = 2;
					t->children = malloc(sizeof(term*) * 2);
					t->children[0] = temp;
					break;
				case 2:	
					sub_root->children[1] = t;
					break;
				default:
					err_print("To many terms inside parentheses\n", str, start_index, *index - start_index);
					return NULL;
			}

			term_index++;
		}			
	}
	if (str[*index] == ')') {
		*index += 1;
	}
	else {
		err_print("Missing closing parentheses\n", str, *index, 1);
		return NULL;
	}
	if (sub_root == NULL) {
		sub_root = temp;
	}
	return sub_root;
}

term* parse_formula(char* str) {
	unsigned int index = 0;
	skip_whitespace(str, &index);
	if (str[index] == '(') {
		index++;
	}
	term* root = parse_parentheses(str, &index);

	return root;
}
