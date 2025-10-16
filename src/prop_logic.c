#include "prop_logic.h"
#include <stdio.h>

char prop_formula_result(prop_formula* formula) {
	switch (formula->conn) {
		case CONN_AND:
			return formula->var1->value && formula->var2->value;
		case CONN_OR:
			return formula->var1->value || formula->var2->value;
		case CONN_NOT:
			return !formula->var1->value;
		default:
			fprintf(stderr, "Invalid formula: Cant calculate result\n");
	}
	return -1;
}

void prop_formula_print(prop_formula* formula) {
	char res = prop_formula_result(formula);
	
	if (formula->conn == CONN_NOT) {
		printf("(%c%s)", logic_conn_symbol(formula->conn), formula->var1->symbol);
	}
	else {
		printf("(%s %c %s)", formula->var1->symbol, logic_conn_symbol(formula->conn), formula->var2->symbol);
	}
	printf(" -> %s\n", res ? "True" : "False");
}

void prop_var_print(prop_var* var) {
	printf("%s -> %s\n", var->symbol, var->value ? "True" : "False");
}
