#include "../../src/string.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct tests {
	unsigned int completed;
	unsigned int passed;
	unsigned int failed;
} tests_t;

#define ASSERT(bool)\
	if (!(bool)) {\
		printf("***[FAILED]: Line: %d\n", __LINE__);\
		tests.failed++;\
	} else {\
		printf("[PASSED]: Line: %d\n", __LINE__);\
		tests.passed++;\
	}\
	tests.completed++;


int main(void) {
	tests_t tests = {0};

	_is_string_destroyed = false;
	_is_cstring_destroyed = false;
	{
		AUTO_STRING *string = create_string();
		AUTO_STRING *string2 = create_string_from("Some text");
		ASSERT(string);
		AUTO_CSTRING *cstring = string->cstring(string);
		AUTO_CSTRING *cstring2 = string2->slice(string2, 1, 3);
		ASSERT(strcmp(cstring2, "ome") == 0);
		char c = string2->at(string2, 2);
		ASSERT(c == 'm');
		ASSERT(string2->len(string2) == 9);
		ASSERT(string2->capacity(string2) == 10);
		char c2 = string2->pop(string2);
		ASSERT(c2 == 't');
		ASSERT(string2->len(string2) == 8);
		string2->cat(string2, "text");
		AUTO_CSTRING *cstring3 = string2->cstring(string2);
		ASSERT(strcmp(cstring3, "Some textext") == 0);
		string2->push(string2, '!');
		AUTO_CSTRING *cstring4 = string2->cstring(string2);
		ASSERT(strcmp(cstring4, "Some textext!") == 0);
		string2->remove_at(string2, 4);
		AUTO_CSTRING *cstring5 = string2->cstring(string2);
		ASSERT(strcmp(cstring5, "Sometextext!") == 0);
		string2->set(string2, 2, 'o');
		AUTO_CSTRING *cstring6 = string2->cstring(string2);
		ASSERT(strcmp(cstring6, "Sooetextext!") == 0);
	}
	ASSERT(_is_string_destroyed == true);
	ASSERT(_is_cstring_destroyed == true);

	printf(
		"\nCompleted:\t%d\nPassed:\t\t%d\nFailed:\t\t%d\n\n",
		tests.completed, tests.passed, tests.failed
	);
	return 0;
}
