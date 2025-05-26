# c-string
Dynamic string for C that aims to be type and memory safe.

## Requirements
For installation ensure cmake and git is installed.
To use the macro wrappers supplied by the library, gcc or clang is required.
Information regarding how to use the library without the macro wrappers can be
found below.

## Installation
Clone and cd into the repository:
```bash
git clone https://github.com/broskobandi/c-string.git &&
cd c-string
```

Create make files:
```bash
cd build && cmake ..
```

Compile and install library:
```bash
make && sudo make install
```

## Basic usage - no macros
Write a test program.
Each function returns a status code of either 0 (on success), or 1 (on failure).
Getter functions work via out aprameters.
```c
//main.c
#include <c-string.h>
#include <stdio.h>

int main(void) {
    // String object must be NULL initialised.
    str_t *str = NULL;
    if (str_new_from(&str, "Hello, c-string!")) return 1;

    // Since functions return status codes, getter functions
    // work via out parameters:
    const char *str_data = NULL;
    if (str->data(str, &str_data)) return 1;
    printf("%s\n", str_data);

    // deleter function must be called explicitly
    str_destroy(&str);

    return 0;
}
```
Compile it:
```bash
gcc main.c -lc-string -o HELLO_CSTRING
```
Run it:
```bash
./HELLO_CSTRING
```

## Usage through macros (gcc / clang only)
Write test program.
The macro wrappers implicitly carry out error propagation by
automatically exiting the caller function with status code 1
upon failure.
Getter macros return the desired data directly.
The macro wrappers can only be used when using gcc or clang compilers.

```c
//main.c
#include <c-string.h>
#include <stdio.h>

int main(void) {
    // STR_AUTO_T ensures automatic memory management
    STR_AUTO_T *str = STR_NEW_FROM("Hello, c-string!");

    // getter macros directly return the desired data or
    // return from the caller with 1 upon failure
    printf("%s\n", STR_DATA(str));

    // Calling str_destroy is no longer necessary as 
    // STR_AUTO_T ensures automatic freeing when the
    // variable goes out of scope. However, str_destroy() still
    // checks whether the string object has been freed before
    // so it is safe to call str_destroy explicitly, should you wish
    // to free its memory before the end of the scope for some reason.

    return 0;
}
```
