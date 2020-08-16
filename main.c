#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

int main(int argc, char *argv[]) {
	char **str_arr = alloc(sizeof(void*) * argc);
	if (str_arr == NULL) {
		fprintf(stderr, "Failed to allocate memory at line %d!\n", __LINE__);
		return -1;
	}

	for (int i = 0; i < argc; ++i) {
		unsigned int copy_len = strlen(argv[i]) + 1;
		str_arr[i] = alloc(copy_len);
		if (str_arr[i] == NULL) {
			fprintf(stderr, "Failed to allocate memory at line %d!\n", __LINE__);
			return -1;
		}
		strncpy(str_arr[i], argv[i], copy_len);
	}

	for (int i = 0; i < argc; ++i) {
		puts(str_arr[i]);
		dealloc(str_arr[i]);
	}

	dealloc(str_arr);
	return 0;
}
