# c-string
Dynamic string written in C.
The aim of the project is to provide a dynamically allocated
string object for C that prioritises type and memory safety,
encapsulation, and error propagation.

## Requirements
Cmake version 4.0+ is required for building the library.
gcc or clang is required for using the macro wrappers.

## Installation
```bash
git clone https://github.com/broskobandi/c-string.git &&
mkdir c-string/build &&
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
sudo make install
```

## Usage
### With macros - RECOMMENDED (clang or gcc required)
```c
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
```
### Without gcc/clang specific macros (more verbose but more portable. Pure C)
```c
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
```
## Status codes
The library currently doesn't have a mechanism to print the returned status code.
If something goes wrong, check the returned status code against the following list:

STR_SUCCESS = 0
STR_ALLOC_ERROR = 1
STR_REALLOC_ERROR = 2
STR_NOT_EMPTY = 3
STR_EMPTY = 4
STR_NULL_PTR = 5

## Testing
In the root directory of the cloned repository run:
```bash
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
./tests/unit-test
```
