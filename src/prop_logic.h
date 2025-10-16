#ifndef PROP_LOGIC_H
#define PROP_LOGIC_H

#include "logic.h"

typedef struct prop_var {
	char* symbol;
	char value;
} prop_var;

typedef struct prop_formula {
	prop_var* var1;
	prop_var* var2;
	logic_conn conn;
} prop_formula;

char prop_formula_result(prop_formula* formula);

void prop_formula_print(prop_formula* formula);

void prop_var_print(prop_var* var);

#endif
