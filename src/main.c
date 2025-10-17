#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TERM_SYMBOL_LENGTH 10

typedef enum term_type {
	CONNECTIVE,
	FORMULA
} term_type;

typedef struct term {
	// term_type type;
	char* symbol;
	// char value;
	struct term** children;
	int child_count;
} term;

static void skip_whitespace(char* str, int* index);
static char* parse_term(char* str, int* index);
static term* parse_parentheses(char* str, int* index); 
static term* parse_formula(char* str);

static void print_term_child(term* t, int child_index);
static void print_term_tree(term* t);

static void skip_whitespace(char* str, int* index) {
	while (str[*index] == ' ') {
		*index += 1;
	}
}

static char* parse_term(char* str, int* index) {
	char* buffer = malloc(sizeof(char) * MAX_TERM_SYMBOL_LENGTH + 1); // +1 to include '\0'
	int i = 0;
	while (str[*index] != '\0' && str[*index] != ' ' && str[*index] != ')' && i < MAX_TERM_SYMBOL_LENGTH) {
		buffer[i] = str[*index];
		*index += 1;
		i++;
	}
	buffer[i] = '\0';
	return buffer;
}

static term* parse_parentheses(char* str, int* index) {
	term* sub_root = NULL;
	term* temp = NULL;

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
			}
			else {
				t = malloc(sizeof(term));

				char* symbol = parse_term(str, index);
				printf("term %d: %s\n", term_index, symbol);

				if (symbol[0] == '!') { // Split if symbol contains ! connective
					char* not_symbol = malloc(2);
					not_symbol[0] = '!';
					not_symbol[1] = '\0';

					str_remove_first(symbol);

					term* t2 = malloc(sizeof(term));
					t2->symbol = symbol;
					t2->child_count = 0;
					t2->children = NULL;

					t->symbol = not_symbol;
					t->child_count = 1;
					t->children = malloc(sizeof(term*) * 1);
					t->children[0] = t2;
				}
				else {
					t->symbol = symbol;
					t->child_count = 0;
					t->children = NULL;
				}
			}

			switch (term_index) {
				case 0:
					temp = t;
					break;
				case 1: // Is a connective (excluding !)
					sub_root = t;
					t->child_count = 2;
					t->children = malloc(sizeof(term*) * 2);
					t->children[0] = temp;
					break;
				case 2:
					sub_root->children[1] = t;
					break;
				default:
					fprintf(stderr, "To many terms inside parentheses\n");
					return NULL;
			}

			term_index++;
		}			
	}
	if (str[*index] == ')') {
		*index += 1;
	}
	if (sub_root == NULL) {
		sub_root = temp;
	}
	return sub_root;
}

static term* parse_formula(char* str) {
	int index = 0;
	skip_whitespace(str, &index);
	if (str[index] == '(') {
		index++;
	}
	term* root = parse_parentheses(str, &index);

	printf("%d\n", index);
	return root;
}

static void print_term_child(term* t, int child_index) {
	if (child_index < t->child_count) {
		term* child = t->children[child_index];
		print_term_tree(child);
	}
	else {
		fprintf(stderr, "Child with index %d does not exist in term %s\n", child_index, t->symbol);
	}
}

static void print_term_tree(term* t) {	
	if (t->children != NULL) { // Is a connective
		switch (t->child_count) {
			case 0:
				fprintf(stderr, "Not a well formed formula: Invalid child_count %d in leaf\n", t->child_count);
				break;
			case 1:
				printf("(");
				printf("%s", t->symbol);
				print_term_child(t, 0);
				printf(")");
				break;
			case 2:
				printf("(");
				print_term_child(t, 0);
				printf(" %s ", t->symbol);
				print_term_child(t, 1);
				printf(")");
				break;
			default:
				fprintf(stderr, "Something went wrong when printing the term %s: Invalid child-count %d\n", t->symbol, t->child_count);
		}
	}
	else {
		printf("%s", t->symbol);
	}
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
	print_term_tree(t);
	printf("\n");
}
