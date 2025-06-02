# c-string
Dynamic string written in C

## Requirements
cmake version 3.30+ is necessary for building and installing the library and the associated header file. Alternatively, if you don't want to build and install the library, just clone the repo and copy the source files into your project dir.

## Installation
Clone and cd into the repo, generate make files, compile the library file and install the library and the corresponding header file into your system's lib and include dirs with the following command:
```bash
git clone https://github.com/broskobandi/c-string.git &&
mkdir c-string/build &&
cd c-string/build &&
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
sudo make install
```

## Usage
Example usage:
```c
#include <c-string.h>

int main(void) {
	// Instantiate a string object:
	str_t *string = str_new();

	// Add some text:
	str_append(string, "Some text");

	//// You can also instantiate and initialise the object in the same time:
	// str_t *string = str_new_from("Some text");

	// Or add a single character:
	str_push(string, '!');

	// Remove and return the last character:
	char last_char = str_pop(string);

	// Get a reference to the string
	// (Useful when a function requires a simple string)
	const char *simple_string = str_data(string);

	// Replace all occurrences of a substring:
	str_replace(string, "Some", "Some awesome");

	// See how long the string is (excluding '\0'):
	int length = str_len(string);

	// See how many bytes can be stored in the object without having to use realloc
	// (which is done implicitly if the length exceeds the capacity)
	int capacity = str_capacity(string);
	//// Note: the above two functions return int instead of unsigned long or size_t
	//// to account for the potential error code -1 which is returned upon failure
	//// (such as when an uninitialised string object is passed to them)
	
	// Compare two strings:
	int is_same = str_cmp(string, "Some awesome text!");
	//// Note: this function returns 1 if true, 0 if false, -1 on failure

	// You can empty the content of the string object. This function
	// restores the default capacity which is 16 bytes.
	str_clear(string);

	// Don't forget to free the allocated data to avoid memory leaks!
	str_destroy(&string);

	return 0;
}
```

The library is designed with debuggability and error propagation in mind. Every function that can fail returns either NULL or an error code on failure. It is recommended to check function results to handle potential errors. For example:
```c
int main(void) {
    str_t *string = str_new();
    if (string == NULL) return 1;

    if (str_push(string, 'a') != 0) return 1;

    str_destroy(&string);

    return 0;
}
```
etc...

If you are using gcc or clang, you can make use of this simple but handy automatic resource management feature:
```c
int main(void) {
    STR_AUTO string = str_new();
    if (string == NULL) return 1;

    if (str_push(string, 'a') != 0) return 1;

    // Note: we no longer have to explicitly use str_destroy(). 
    // The memory is automatically freed when 'string' goes out of scope.

    return 0;
}
```
To run the unit tests provided, run the following command in the root directory of the cloned repo.
## Testing
```bash
mkdir tests/build &&
cd tests/build &&
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=on &&
make &&
./unit-test
```
