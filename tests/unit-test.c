#include <c-string.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

uint completed = 0;
uint passed = 0;
uint failed = 0;

#define ASSERT(expr)\
	if (!(expr)) {\
		printf("***[FAILED]: %s\n", #expr);\
		printf("\tLine:%d\n", __LINE__);\
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

int main(void) {
	// test new_str() and destroy_str()
	str_t *str = str_new();
	ASSERT(str != NULL);
	is_str_destroyed = false;
	str_destroy(&str);
	ASSERT(is_str_destroyed == true);
	
	// test new_str_from() and str_data()
	str = str_new_from("Some text");
	ASSERT(strcmp(str_data(str), "Some text") == 0);
	str_destroy(&str);

	// test str_replace()
	str = str_new_from("Some long long text");
	ASSERT(str_replace(str, "long", "amazing") == 0);
	ASSERT(strcmp(str_data(str), "Some amazing amazing text") == 0);
	ASSERT(str_replace(str, "amazing", "SDL_Rect") == 0);
	ASSERT(strcmp(str_data(str), "Some SDL_Rect SDL_Rect text") == 0);
	ASSERT(str_replace(str, "SDL_Rect", "") == 0);
	ASSERT(strcmp(str_data(str), "Some   text") == 0);
	ASSERT(str_replace(str, "SDL_Rect", "") == 0);
	str_destroy(&str);

	// test str_len() and str_capacity()
	str = str_new_from("Some text");
	ASSERT(str_len(str) == 9);
	ASSERT(str_capacity(str) == 18);
	str_destroy(&str);

#ifdef __GNUC__
	//test STR_AUTO
	{
		STR_AUTO str = str_new();
		is_str_destroyed = false;
	}
	ASSERT(is_str_destroyed == true);
#endif

	//test str_push()
	str = str_new_from("Some text");
	ASSERT(str_push(str, '!') == 0);
	ASSERT(strcmp(str_data(str), "Some text!") == 0);
	ASSERT(str_len(str) == 10);
	ASSERT(str_capacity(str) == 18);
	str_destroy(&str);

	//test str_pop()
	str = str_new_from("Some text!");
	ASSERT(str_pop(str) == '!');
	ASSERT(strcmp(str_data(str), "Some text") == 0);
	ASSERT(str_len(str) == 9);
	ASSERT(str_capacity(str) == 10);
	str_destroy(&str);

	//test str_cmp()
	str = str_new_from("Some text!");
	const char *src1 = "Some text!";
	const char *src2 = "some_text";
	ASSERT(str_cmp(str, src1) == 1);
	ASSERT(str_cmp(str, src2) == 0);
	str_destroy(&str);

	//test str_append()
	str = str_new_from("Some ");
	ASSERT(str_append(str, "text") != NULL);
	ASSERT(str_cmp(str, "Some text") == 1);

	// test str_clear()
	ASSERT(str_clear(str) == 0);
	ASSERT(str_len(str) == 0);
	ASSERT(str_capacity(str) == 16);
	ASSERT(str_cmp(str, "") == 1);

	str_destroy(&str);

	print_results();
}
