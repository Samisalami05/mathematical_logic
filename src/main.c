#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handler.h"

#define MAX_TERM_SYMBOL_LENGTH 10

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

static void skip_whitespace(char* str, unsigned int* index);
static term* parse_term(char* str, unsigned int index, unsigned int length);
static term* parse_parentheses(char* str, unsigned int* index); 
static term* parse_formula(char* str);

static void print_term_child(char* str, term* t, int child_index);
static void print_term(char* str, term* t);
static void print_term_tree(char* str, term* t);

static void skip_whitespace(char* str, unsigned int* index) {
	while (str[*index] == ' ') {
		*index += 1;
	}
}

// Returns length
static unsigned int skip_term(char* str, unsigned int* index) {
	unsigned int len = 0;
	while (str[*index] != '\0' && str[*index] != ' ' && str[*index] != ')') {
		*index += 1;
		len++;
	}
	return len;
}

static term_type term_connective(char* str, int index) {
	switch (str[index]) {
		case SYMBOL_AND:
			return CONNECTIVE_AND;
		case SYMBOL_OR:
			return CONNECTIVE_OR;
		case SYMBOL_NOT:
			return CONNECTIVE_NOT;
		default:
			fprintf(stderr, "Can't interpret connective %c", str[index]);
	}
	return ATOM;
}

static term* create_term(char* str, unsigned int* index) {
	term* t = malloc(sizeof(term));
				
	unsigned int term_length = skip_term(str, index);	

	if (str[*index - term_length] == SYMBOL_NOT) { // Split if symbol contains ! connective	
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
	else {
		t->type = ATOM;
		t->index = *index - term_length;
		t->length = term_length;
		t->child_count = 0;
		t->children = NULL;
	}
	return t;
}


static term* parse_parentheses(char* str, unsigned int* index) {
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

static term* parse_formula(char* str) {
	unsigned int index = 0;
	skip_whitespace(str, &index);
	if (str[index] == '(') {
		index++;
	}
	term* root = parse_parentheses(str, &index);

	printf("%d\n", index);
	return root;
}

static void print_term_child(char* str, term* t, int child_index) {
	if (child_index < t->child_count) {
		term* child = t->children[child_index];
		print_term(str, child);
	}
	else {
		fprintf(stderr, "Child with index %d does not exist in term %.*s\n", child_index, t->length, &str[t->index]);
	}
}

static void print_term(char* str, term* t) {
	switch (t->type) {
		case ATOM:
			printf("%.*s", t->length, &str[t->index]);
			break;
		case CONNECTIVE_NOT:
			printf("(");
			printf("%c", SYMBOL_NOT);
			print_term_child(str, t, 0);
			printf(")");
			break;
		case CONNECTIVE_AND:
			printf("(");
			print_term_child(str, t, 0);
			printf(" %c ", SYMBOL_AND);
			print_term_child(str, t, 1);
			printf(")");
			break;
		case CONNECTIVE_OR:
			printf("(");
			print_term_child(str, t, 0);
			printf(" %c ", SYMBOL_OR);
			print_term_child(str, t, 1);
			printf(")");
			break;
		default:
			fprintf(stderr, "Something went wrong when printing the term %.*s: Invalid term type %d\n", t->length, &str[t->index], t->type);
	}
}

static void print_term_tree_recursive(char* str, term* t, unsigned int* leaves) {
	printf("%c", str[t->index]);
}

static void print_term_tree(char* str, term* t) {
	unsigned int leaf_count = 0;
	printf("%c\n", str[t->index]);
}

int main(void) {
	char buffer[1000];

	printf("Enter a formula: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remove trailing newline, if any
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("You entered: %s\n", buffer);
    } else {
        fprintf(stderr, "Error reading input.\n");
    }

	term* t = parse_formula(buffer);
	if (t == NULL)
		exit(EXIT_FAILURE);
	print_term(buffer, t);
	printf("\n");

	print_term_tree(buffer, t);
}
