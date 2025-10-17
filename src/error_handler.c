#include "error_handler.h"
#include <stdio.h>

#define COL_RED "\x1B[31m"
#define COL_DEF "\x1B[0m"

void err_print(char* message, char* str, unsigned int index, unsigned int length) {
	fprintf(stderr, "%sERROR%s: %d - %s\n", COL_RED, COL_DEF, index, message);
	fprintf(stderr, "\t%s\n\t", str);
	for (unsigned int i = 0; i < index; i++) {
		printf(" ");
	}
	printf(COL_RED);
	for (unsigned int i = 0; i < length; i++) {
		if (i == 0) {
			printf("^");
			continue;
		}
		printf("~");
	}
	printf(COL_DEF);
	printf("\n");
}
