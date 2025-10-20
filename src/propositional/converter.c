#include "converter.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

static term* formula_negate(term* formula);
static term* de_morgans_law(term* formula);

static term* formula_negate(term* formula) {
	term* negation = malloc(sizeof(term));
	negation->type = CONNECTIVE_NOT;
	negation->index = -1;
	negation->child_count = 1;
	negation->children = malloc(sizeof(term*));
	negation->children[0] = formula;
	return formula;
}

static term* de_morgans_law(term* formula) {
	term** children = formula->children;
	int child_count = formula->child_count;
	if (formula->type == CONNECTIVE_NOT && formula->child_count == 1) {
		switch (children[0]->type) {
			case CONNECTIVE_AND:
				children = formula->children[0]->children;
				formula->type = CONNECTIVE_OR;
				formula->child_count = 2;
				formula->children = realloc(formula->children, sizeof(term*) * 2);

				children[0] = formula_negate(children[0]);
				children[1] = formula_negate(children[1]);

				formula->children = children;
				break;
			case CONNECTIVE_OR:
				children = formula->children[0]->children;
				formula->type = CONNECTIVE_AND;
				formula->child_count = 2;
				formula->children = realloc(formula->children, sizeof(term*) * 2);

				children[0] = formula_negate(children[0]);
				children[1] = formula_negate(children[1]);

				formula->children = children;
				break;
			default:
				printf("Not a de morgan term, %d\n", formula->index);
				break;
		}
	}

	for (int i = 0; i < child_count; i++) {
		de_morgans_law(children[i]);
	}
	return NULL;
}


term* convert_to_cnf(term* formula) {
	de_morgans_law(formula);
	return formula;
}
