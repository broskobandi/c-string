// #define NDEBUG

#include "../include/c-string.h"
#include "../include/c-string-macros-wrappers.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

uint completed = 0;
uint passed = 0;
uint failed = 0;

#define ASSERT(expr)\
	if (!(expr)) {\
		printf("***[FAILED]: %s\n", #expr);\
		failed++;\
	} else {\
		printf("[PASSED]: %s\n", #expr);\
		passed++;\
	}\
	completed++;

void print_results() {
	if (!failed) {
		printf("\nCompleted tests: %d\n", completed);
		printf("All tests passed.\n");
	} else {
		printf("\nCompleted:\t%d\n", completed);
		printf("Passed:\t\t%d\n", passed);
		printf("Failed:\t\t%d\n", failed);
	}
}

// Tests

int test_str_new() {
	{
		STR_AUTO_T str = STR_NEW;
		ASSERT(str != NULL);
		is_str_destroyed = false;
	}
	ASSERT(is_str_destroyed == true);
	return STR_SUCCESS;
}

int test_str_new_from() {
	STR_AUTO_T str = STR_NEW_FROM("Some text");
	ASSERT(str != NULL);
	ASSERT(strcmp(STR_DATA(str), "Some text") == 0);
	return STR_SUCCESS;
}

int test_str_replace() {
	STR_AUTO_T str = STR_NEW_FROM("Some long long text");
	STR_REPLACE(str, "long", "great");
	ASSERT(strcmp(STR_DATA(str), "Some great great text") == 0);
	return STR_SUCCESS;
}

int test_str_cmp() {
	STR_AUTO_T str = STR_NEW_FROM("Some text");
	ASSERT(STR_CMP(str, "Some text") == true);
	return STR_SUCCESS;
}

int main(void) {
	TRY(test_str_new());
	TRY(test_str_new_from());
	TRY(test_str_replace());
	TRY(test_str_cmp());

	print_results();
	return 0;
}
