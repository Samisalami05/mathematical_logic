#ifndef CONVERTER_H
#define CONVERTER_H

#include "parser.h"
#include <stddef.h>

typedef struct clause {
	char** propositions;
	size_t count;
} clause;

term* convert_to_cnf(term* formula);

#endif
