#include "util.h"
#include "string.h"

void str_remove_first(char* str) {
	for (int i = 1; i < strlen(str) + 1; i++) {
		str[i - 1] = str[i];
	}
}
