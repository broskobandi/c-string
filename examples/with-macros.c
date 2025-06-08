#include <c-string.h>

// All macros are designed to return from the caller function with the
// appropriate status code upon failure.
// Getter functions return the desired value directly, meaning they can 
// be used for in line initialisation.

int main(void) {
	const char *hello = "Hello";
	const char *this = "This";
	const char *c_string = "c-string";

	// Powerful initialisation function:
	str_auto str = str_new(hello, ", World! ", this, " is ", c_string, "!");

	// The function can be called without argument too:
	str_auto empty_str = str_new();

	// 'str_auto' uses a gcc extension attribute 'cleanup'.
	// string objects created with this are automatically
	// destroyed when they go out of scope. If you'd like to
	// create an instance that does not get destroyed automatically,
	// simply use the custom type directly:
	
	str_t *manual_str = str_new("Don't forget to free me!");

	// note: this needs to be freed explicitly with the following function:
	str_destroy(&manual_str);

	// Functions to operate over the string objects:
	
	// Appends 'src' at the end of 'str'.
	str_append(str, " This is a new library");
	// Replaces all instances of 'new' to 'awesome'
	str_replace(str, "new", "kickass");
	// Returns a pointer to the data stored in str.
	// This is reference and not a copy!
	// This is the only 'unsafe' feature of the library. Tinking of a better solution.
	str_data(str);
	// Returns the length of 'str'
	str_len(str);
	// Returns the capacity of 'str'
	str_capacity(str);
	// Appends '!' at the end of 'str'
	str_push(str, '!');
	// Compares contents of 'str' with the second argument. This function returns a bool.
	str_cmp(str, "Hello, World! This is c-string! This is a kickass library!");
	// Checks if 'str' has "c-string" in it. This function also returns a bool.
	str_has(str, "c-string");
	// Removes and returns last char of 'str'.
	str_pop(str);
	// Clears content of 'str'.
	str_clear(str);

	return 0;
}
