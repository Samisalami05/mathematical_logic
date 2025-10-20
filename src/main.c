#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "propositional/parser.h"
#include "propositional/converter.h"
#include "error_handler.h"

#define MAX_TERM_SYMBOL_LENGTH 10

static void print_term_child(char* str, term* t, int child_index);
static void print_term(char* str, term* t);
static void print_term_tree(char* str, term* t);

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

static void print_term_tree_recursive(char* str, term* t, unsigned int padding, char last) {
	for (int i = 0; i < padding; i++) {
		printf("|  ");
	}
	char* beginning = last ? "`- " : "|- ";
	if (t->type == ATOM) {
		printf("%s%.*s\n", beginning, t->length, &str[t->index]);
	}
	else {
		printf("%s%c\n", beginning, term_symbol(t->type));
	}
	
	if (t->children != NULL) {
		for (int i = 0; i < t->child_count; i++) {
			if (t->children[i] == NULL) {
				break;
			}
			print_term_tree_recursive(str, t->children[i], padding + 1, i == t->child_count - 1);
		}
	}
}

static void print_term_tree(char* str, term* t) {
	print_term_tree_recursive(str, t, 0, 0);
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
	printf("\n\n");

	print_term_tree(buffer, t);

	printf("\nConverting to cnf\n");
	t = convert_to_cnf(t);

	print_term(buffer, t);
	printf("\n");

	print_term_tree(buffer, t);
}
