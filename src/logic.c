#include "logic.h"
#include <stdio.h>


char logic_conn_symbol(logic_conn conn) {
	switch (conn) {
		case CONN_AND:
			return '^';
		case CONN_OR:
			return 'v';
		case CONN_NOT:
			return '!';
		default:
			fprintf(stderr, "Invalid formula: Can't calculate result\n");
	}
	return '?';
}
