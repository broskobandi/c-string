#include <c-string.h>

// All functions are designed to return a status code (except str_destroy).
// Getter and factory functions work through out parameters.

int main(void) {
	const char *hello = "Hello";
	const char *this = "This";
	const char *c_string = "c-string";

	// Initialisation function:
	// str_auto str = str_new(hello, ", World! ", this, " is ", c_string, "!");
	str_t *str = NULL;
	str_status_t status = create_str(&str);
	if (status) return (int)status;

	// The 'TRY' macro is designed to check the status code and return
	// from the caller function in one line. This macro doesn't require gcc or clang.
	
	// Let's append some text at the end of 'str'
	TRY(str->append(str, hello));
	TRY(str->append(str, ", World! "));
	TRY(str->append(str, this));
	TRY(str->append(str, " is "));
	TRY(str->append(str, c_string));
	TRY(str->append(str, "!"));

	// Further functions to operate over 'str'

	// Appends 'src' at the end of 'str'.
	TRY(str->append(str, " This is a new library"));
	// Replaces all instances of 'new' to 'awesome'
	TRY(str->replace(str, "new", "kickass"));
	// Returns a pointer to the data stored in str.
	// This is reference and not a copy!
	// This is the only 'unsafe' feature of the library. Tinking of a better solution.
	const char *data = NULL;
	TRY(str->data(str, &data));
	// Returns the length of 'str'
	unsigned long len = 0;
	TRY(str->len(str, &len));
	// Returns the capacity of 'str'
	unsigned long capacity = 0;
	TRY(str->capacity(str, &capacity));
	// Appends '!' at the end of 'str'
	TRY(str->push(str, '!'));
	// Compares contents of 'str' with the second argument
	bool is_same = false;
	TRY(str->cmp(str, "Hello, World! This is c-string! This is a kickass library!", &is_same));
	// Checks if 'str' has "c-string" in it
	bool has_c_string = false;
	TRY(str->has(str, "c-string", &has_c_string));
	// Removes and returns last char of 'str'
	char last_char;
	TRY(str->pop(str, &last_char));
	// Clears content of 'str'.
	TRY(str->clear(str));
	
	// note: 'str' needs to be freed explicitly with the following function:
	str_destroy(&str);

	return 0;
}
