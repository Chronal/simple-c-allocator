#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

int main(int argc, char *argv[]) {
	unsigned int total_len = 0;
	for (int i = 0; i < argc; ++i) {
		total_len += strlen(argv[i]);
	}
	total_len += 1; //Null terminator

	char *str = alloc(total_len);

	char* endptr = str;

	for (int i = 0; i < argc; ++i) {
		endptr = stpcpy(endptr, argv[i]);
	}

	puts(str);

	dealloc(str);
	return 0;
}
