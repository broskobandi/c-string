#include "../include/c-string-utils.h"
#include <stdio.h>
#include <sys/types.h>

// Debugging
const char *str_status_arr[] = {
#define X(name) #name,
	STR_STATUS_LIST
#undef X
};

void print_status(
	const char *function,
	const char *status_str,
	const char *file,
	int line
) {
	printf(
		"%s exited with status: %s.\nFile: %s\nLine: %d\n",
		function, status_str, file, line
	);
}

