#include "../include/c-string.h"
#include <stdio.h>
#include <string.h>

typedef struct test {
	unsigned int completed;
	unsigned int passed;
	unsigned int failed;
} test_t;

void print_test_result(const test_t *test) {
	printf("\nCompleted:\t%d\n", test->completed);
	printf("Passed:\t\t%d\n", test->passed);
	printf("Failed:\t\t%d\n\n", test->failed);
}

#define ASSERT(expr)\
	if (!(expr)) {\
		printf("[FAILED]: %s\n", #expr);\
		test.failed++;\
	} else {\
		printf("[PASSED]: %s\n", #expr);\
		test.passed++;\
	}\
	test.completed++;

/**
 * Unit tests for c-string
 * */

int test_new_and_destroy() {
	{
		STR_AUTO_T *str = STR_NEW;
		if (!str) return 1;
		is_str_destroyed = 0;
	}
	if (is_str_destroyed != 1) return 1;
	return 0;
}

int test_new_from_and_data() {
	const char *data = "Some text";
	STR_AUTO_T *str = STR_NEW_FROM(data);
	if (!str) return 1;
	if (strcmp(data, STR_DATA(str)) != 0) return 1;
	return 0;
}

int test_cat() {
	const char *data1 = "Some ";
	STR_AUTO_T *str1 = STR_NEW_FROM(data1);
	const char *data2 = "text";
	STR_AUTO_T *str2 = STR_NEW_FROM(data2);
	STR_CAT(str1, str2);
	if (strcmp(STR_DATA(str1), "Some text") != 0) return 1;
	return 0;
}

int test_replace() {
	STR_AUTO_T *str = STR_NEW_FROM("Some long and interesting text with long and interesting words");
	STR_REPLACE(str, "interesting", "fun");
	if (strcmp(STR_DATA(str), "Some long and fun text with long and fun words") != 0) return 1;
	return 0;
}

int test_push_and_pop() {
	STR_AUTO_T *str = STR_NEW;
	STR_PUSH(str, 'a');
	if (STR_POP(str) != 'a') return 1;
	return 0;
}

int test_pop_underflow() {
	STR_AUTO_T *str = STR_NEW;
	STR_POP(str);
	return 0;
}

int test_len_and_capacity() {
	const char *text = "Some text";
	STR_AUTO_T *str = STR_NEW_FROM(text);
	unsigned long expected_capacity = (unsigned long)((float)strlen(text) * 1.5f);
	unsigned long expected_len = strlen(text);
	if (STR_LEN(str) != expected_len) return 1;
	if (STR_CAPACITY(str) != expected_capacity) return 1;
	return 0;
}

int main(void) {
	test_t test = {0};

	ASSERT(test_new_and_destroy() == 0);
	ASSERT(test_new_from_and_data() == 0);
	ASSERT(test_cat() == 0);
	ASSERT(test_replace() == 0);
	ASSERT(test_push_and_pop() == 0);
	ASSERT(test_pop_underflow() == 1);
	ASSERT(test_len_and_capacity() == 0);

	print_test_result(&test);
	return 0;
}

