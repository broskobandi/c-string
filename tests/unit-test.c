#include <c-string.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

uint completed = 0;
uint passed = 0;
uint failed = 0;

#define ASSERT(expr)\
	if (!(expr)) {\
		printf("***[FAILED]: %s\n", #expr);\
		printf("\tLine: %d\n", __LINE__);\
		failed++;\
	} else {\
		printf("[PASSED]: %s\n", #expr);\
		passed++;\
	}\
	completed++;

void print_results() {
	printf("\nCompleted:\t%d\n", completed);
	if (!failed) {
		printf("All tests passed.\n");
	} else {
		printf("Passed:\t\t%d\n", passed);
		printf("Failed:\t\t%d\n", failed);
	}
}

// Tests
int test_str_new_empty() {
	str_auto str = str_new();
	ASSERT(str != NULL);
	_is_str_destroyed = false;
	return 0;
}

int test_str_new_not_empty() {
	str_auto str = str_new();
	str_status_t status = create_str(&str);
	ASSERT(status == STR_NOT_EMPTY);
	return 0;
}

int test_str_new_from() {
	const char *is = "is";
	const char *text = "text";
	str_auto str = str_new("This ", is, " some ", text);
	ASSERT(strcmp(str_data(str), "This is some text") == 0);
	return 0;
}

int test_str_new_from_long_text() {
	const char *long_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(long_text);
	ASSERT(str != NULL);
	ASSERT(strcmp(str_data(str), long_text) == 0);
	return 0;
}

int test_replace() {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	const char *result = "This IS some super duper long text.\n"
	"This text IS so long, this will surely need to realloc the memory,\n"
	"because this text IS definitely longer than 16 bytes, which IS the default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(original_text);
	str_replace(str, " is ", " IS ");
	ASSERT(strcmp(str_data(str), result) == 0);
	return 0;
}

int test_replace_long() {
	const char *original_text = "word word word text";
	const char *result = "some super duper duper duper duper duper duper long substitution some super duper duper duper duper duper duper long substitution some super duper duper duper duper duper duper long substitution text";
	str_auto str = str_new(original_text);
	str_replace(str, "word", "some super duper duper duper duper duper duper long substitution");
	printf("%s\n", str_data(str));
	ASSERT(strcmp(str_data(str), result) == 0);
	return 0;
}

int test_replace_short() {
	const char *original_text = "some super duper duper duper duper duper duper long substitution some super duper duper duper duper duper duper long substitution some super duper duper duper duper duper duper long substitution text";
	const char *result = "word word word text";
	str_auto str = str_new(original_text);
	str_replace(str, "some super duper duper duper duper duper duper long substitution", "word");
	printf("%s\n", str_data(str));
	ASSERT(strcmp(str_data(str), result) == 0);
	return 0;
}

int test_replace_nothing_with_something () {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	const char *result = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(original_text);
	str_replace(str, "", "something");
	ASSERT(strcmp(str_data(str), result) == 0);
	return 0;
}

int test_replace_something_with_nothing () {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	const char *result = "Thissome super duper long text.\n"
	"This textso long, this will surely need to realloc the memory,\n"
	"because this textdefinitely longer than 16 bytes, whichthe default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(original_text);
	str_replace(str, " is ", "");
	ASSERT(strcmp(str_data(str), result) == 0);
	return 0;
}

int test_len() {
	const char *original_text = "yo. ";

	const char *new_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(original_text);
	ASSERT(str_len(str) == strlen(original_text));
	str_append(str, new_text);
	ASSERT(str_len(str) == strlen(original_text) + strlen(new_text));

	return 0;
}

int test_len_zero() {
	str_auto str = str_new();
	ASSERT(str_len(str) == 0);
	return 0;
}

int test_str_cmp_true() {
	str_auto str = str_new("Some text");
	ASSERT(strcmp(str_data(str), "Some text") == 0);
	ASSERT(str_cmp(str, "Some text") == true);
	return 0;
}

int test_str_cmp_false() {
	str_auto str = str_new("Some text");
	ASSERT(strcmp(str_data(str), "carrot") != 0);
	ASSERT(str_cmp(str, "carrot") == false);
	return 0;
}

int test_str_cmp_empty_source() {
	str_auto str = str_new();
	ASSERT(strcmp(str_data(str), "") == 0);
	ASSERT(str_cmp(str, "") == true);
	return 0;
}

int test_push() {
	str_auto str = str_new();
	str_push(str, 'a');
	ASSERT(str_cmp(str, "a"));
	return 0;
}

int test_pop() {
	str_auto str = str_new("a");
	ASSERT(str_pop(str) == 'a');
	ASSERT(str_len(str) == 0);
	return 0;
}

int test_pop_underflow() {
	str_auto str = str_new();
	__attribute__((unused)) char c = str_pop(str);
	return 0;
}

int test_capacity() {
	str_auto str = str_new();
	const ulong default_capacity = 16;
	ulong expected_capacity = default_capacity;

	while (str_capacity(str) < 1000000) {
		str_append(str, "Some text, yay!!");
		if (str_len(str) + 1 > expected_capacity)
			expected_capacity *= 2;
		if(str_capacity(str) != expected_capacity) return -1;
	}

	return 0;
}

int test_clear() {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";

	str_auto str = str_new(original_text);
	str_clear(str);
	ASSERT(str_len(str) == 0);
	ASSERT(str_capacity(str) == 16);
	printf("%s\n", str_data(str));
	ASSERT(str_cmp(str, "") == true);
	return 0;
}

int test_clear_empty() {
	str_auto str = str_new();
	str_clear(str);
	ASSERT(str_len(str) == 0);
	ASSERT(str_capacity(str) == 16);
	ASSERT(str_cmp(str, ""));
	return 0;
}

int test_has_true() {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";
	str_auto str = str_new(original_text);
	
	ASSERT(str_has(str, "memory") == true);

	return 0;
}

int test_has_false() {
	const char *original_text = "This is some super duper long text.\n"
	"This text is so long, this will surely need to realloc the memory,\n"
	"because this text is definitely longer than 16 bytes, which is the default\n"
	"capacity. Fingers crossed!";
	str_auto str = str_new(original_text);
	
	ASSERT(str_has(str, "carrot") == false);

	return 0;
}

int test_has_empty() {
	str_auto str = str_new();
	ASSERT(str_has(str, "carrot soup") == false);
	return 0;
}

int main(void) {
	ASSERT(test_str_new_empty() == 0);
	ASSERT(_is_str_destroyed == true);
	ASSERT(test_str_new_not_empty() == 0);
	ASSERT(test_str_new_from() == 0);
	ASSERT(test_str_new_from_long_text() == 0);
	ASSERT(test_replace() == 0);
	ASSERT(test_replace_long() == 0);
	ASSERT(test_replace_short() == 0);
	ASSERT(test_replace_nothing_with_something() == STR_EMPTY);
	ASSERT(test_replace_something_with_nothing() == 0);
	ASSERT(test_len() == 0);
	ASSERT(test_len_zero() == 0);
	ASSERT(test_str_cmp_true() == 0);
	ASSERT(test_str_cmp_false() == 0);
	ASSERT(test_str_cmp_empty_source() == 0);
	ASSERT(test_push() == 0);
	ASSERT(test_pop() == 0);
	ASSERT(test_pop_underflow() == STR_EMPTY);
	ASSERT(test_capacity() == 0);
	ASSERT(test_clear() == 0);
	ASSERT(test_clear_empty() == 0);
	ASSERT(test_has_true() == 0);
	ASSERT(test_has_false() == 0);
	ASSERT(test_has_empty() == 0);

	print_results();
	return 0;
}
