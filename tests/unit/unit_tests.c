#include "../../src/string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct tests {
	unsigned int completed;
	unsigned int passed;
	unsigned int failed;
} tests_t;

#define ASSERT(bool)\
	if (!(bool)) {\
		printf("***[FAILED]: %s\n", __func__);\
		tests->failed++;\
	} else {\
		printf("[PASSED]: %s\n", __func__);\
		tests->passed++;\
	}\
	tests->completed++;

void test_create_string(tests_t *tests) {
	string_t *string = create_string();
	ASSERT(string != NULL);
	destroy_string(&string);
}

void test_create_string_from(tests_t *tests) {
	string_t *string = create_string_from("Some text");
	ASSERT(string != NULL);
	destroy_string(&string);
}

void test_destroy_string(tests_t *tests) {
	_is_string_destroyed = false;
	string_t *string = create_string_from("Some text");
	destroy_string(&string);
	ASSERT(_is_string_destroyed == true);
}

void test_destroy_cstring(tests_t *tests) {
	_is_cstring_destroyed = false;
	char *cstring = malloc(16);
	destroy_cstring(&cstring);
	ASSERT(_is_cstring_destroyed == true);
}

void test_auto_string(tests_t *tests) {
	_is_string_destroyed = false;
	{
		AUTO_STRING *string = create_string();
	}
	ASSERT(_is_string_destroyed == true);
}

void test_auto_cstring(tests_t *tests) {
	_is_cstring_destroyed = false;
	{
		AUTO_CSTRING *cstring = malloc(16);
	}
	ASSERT(_is_cstring_destroyed == true);
}

void test_cstring(tests_t *tests) {
	char *input_text = "Some text";
	AUTO_STRING *string = create_string_from(input_text);
	AUTO_CSTRING *cstring = string->cstring(string);
	ASSERT(strcmp(input_text, cstring) == 0);
}

void test_slice(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	AUTO_CSTRING *slice = string->slice(string, 1, 3);
	ASSERT(strcmp(slice, "ome") == 0)
}

void test_at(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	char c = string->at(string, 2);
	ASSERT(c == 'm');
}

void test_len(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	int len = string->len(string);
	ASSERT(len == 9);
}

void test_capacity(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	int capacity = string->capacity(string);
	ASSERT(capacity == 10);
}

void test_pop(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	char c = string->pop(string);
	ASSERT(c == 't');
}

void test_cat(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	char *other_string = "sometext";
	string->cat(string, other_string);
	AUTO_CSTRING *new_string = string->cstring(string);
	ASSERT(strcmp(new_string, "Some textsometext") == 0);
}

void test_push(tests_t *tests) {
	AUTO_STRING *string = create_string();
	string->push(string, 'a');
	char c = string->pop(string);
	ASSERT(c == 'a');
}

void test_remove_at(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	string->remove_at(string, 4);
	AUTO_CSTRING *cstring = string->cstring(string);
	ASSERT(strcmp(cstring, "Sometext") == 0);
}

void test_set(tests_t *tests) {
	AUTO_STRING *string = create_string_from("Some text");
	string->set(string, 4, '!');
	AUTO_CSTRING *cstring = string->cstring(string);
	ASSERT(strcmp(cstring, "Some!text") == 0);
}

void test__expand_string_data(tests_t *tests) {
	string_t *string = create_string();
	for (int i = 0; i < 130; i++) {
		string->push(string, 'a');
		if (string->len(string) < 16)
			if (string->capacity(string) != 16) {ASSERT(false)}
		if (string->len(string) > 16 && string->len(string) < 32)
			if (string->capacity(string) != 32) {ASSERT(false)}
		if (string->len(string) > 32 && string->len(string) < 64)
			if (string->capacity(string) != 64) {ASSERT(false)}
		if (string->len(string) > 64 && string->len(string) < 128)
			if (string->capacity(string) != 128) {ASSERT(false)}
		if (string->len(string) > 128 && string->len(string) < 192)
			if (string->capacity(string) != 192) {ASSERT(false)}
		if (string->len(string) > 192 && string->len(string) < 288)
			if (string->capacity(string) != 288) {ASSERT(false)}
	}
	destroy_string(&string);
	AUTO_STRING *string2 = create_string();
	for (int i = 1; i < 13; i++) {
		string2->cat(string2, "Some text!");
		if (string2->len(string2) < 16)
			if (string2->capacity(string2) != 16) {ASSERT(false)}
		if (string2->len(string2) > 16 && string2->len(string2) < 32)
			if (string2->capacity(string2) != 32) {ASSERT(false)}
		if (string2->len(string2) > 32 && string2->len(string2) < 64)
			if (string2->capacity(string2) != 64) {ASSERT(false)}
		if (string2->len(string2) > 64 && string2->len(string2) < 128)
			if (string2->capacity(string2) != 128) {ASSERT(false)}
		if (string2->len(string2) > 128 && string2->len(string2) < 192)
			if (string2->capacity(string2) != 192) {ASSERT(false)}
		if (string2->len(string2) > 192 && string2->len(string2) < 288)
			if (string2->capacity(string2) != 288) {ASSERT(false)}
	}
	ASSERT(true);
}

void test__shrink_string_data(tests_t *tests) {
	AUTO_STRING *string = create_string();
	for (int i = 0; i < 15; i++) {
		string->cat(string, "Some text!");
	}
	for (int i = 0; i < 150; i++) {
		string->pop(string);
		if (string->len(string) > 96)
			if (string->capacity(string) != 192) {ASSERT(false)}
		if (string->len(string) > 48 && string->len(string) < 96)
			if (string->capacity(string) != 96) {ASSERT(false)}
		if (string->len(string) > 24 && string->len(string) < 48)
			if (string->capacity(string) != 48) {ASSERT(false)}
		if (string->len(string) > 12 && string->len(string) < 24)
			if (string->capacity(string) != 24) {ASSERT(false)}
		if (string->len(string) < 12)
			if (string->capacity(string) != 24) {ASSERT(false)}
	}
	ASSERT(true);
}

int main(void) {
	tests_t tests = {0};

	test_create_string(&tests);
	test_create_string_from(&tests);
	test_destroy_string(&tests);
	test_destroy_cstring(&tests);
	test_auto_string(&tests);
	test_auto_cstring(&tests);
	test_cstring(&tests);
	test_slice(&tests);
	test_at(&tests);
	test_len(&tests);
	test_capacity(&tests);
	test_pop(&tests);
	test_cat(&tests);
	test_push(&tests);
	test_remove_at(&tests);
	test_set(&tests);
	test__expand_string_data(&tests);
	test__shrink_string_data(&tests);

	printf(
		"\nCompleted:\t%d\nPassed:\t\t%d\nFailed:\t\t%d\n\n",
		tests.completed, tests.passed, tests.failed
	);
	return 0;
}
