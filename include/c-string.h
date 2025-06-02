/*
MIT License

Copyright (c) 2025 broskobandi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef C_STRING_H
#define C_STRING_H

#include <stdbool.h>
#include <sys/types.h>

// String opaque struct
typedef struct str str_t;

// Allocates memory for a new string object and returns a pointer to its address.
str_t *str_new();

// Allocates memory for a new string object and returns a pointer to its address.
// Copies 'src' into this memory.
str_t *str_new_from(const char *src);

// This variable is only necessary for testing purposes.
// str_destroy() sets it to 'true'
extern bool is_str_destroyed;

// Frees all memory associated with 'str'
void str_destroy(str_t **str);

// Returns a const pointer to the data stored in 'str' or NULL on failure.
const char *str_data(const str_t *str);

// Replaces all occurrences of 'old_str' with 'new_str' in 'str'.
// Returns 0 on success or 1 on failure.
int str_replace(str_t *str, const char *old_str, const char *new_str);

// Returns an int containing the length of 'str' or -1 on failure.
int str_len(const str_t *str);

// Returns an int containing the capacity of 'str' or -1 on failure.
int str_capacity(const str_t *str);

// Appends 'c' at the end of 'str'.
// Returns 0 on success or 1 on failure.
int str_push(str_t *str, char c);

// Removes and returns the last char in 'str'.
// Returns -1 on failure.
char str_pop(str_t *str);

// If 'str1' and 'str2' are the same, it returns 1,
// otherwise it returns 0.
// Returns -1 on failure.
int str_cmp(const str_t *str1, const char *str2);

// Clears data stored in 'str'.
// Returns 0 on success or 1 on failure.
int str_clear(str_t *str);

// Appends 'src' at the end of 'str'.
// Returns a const pointer to the new string data or NULL on failure.
const char *str_append(str_t *str, const char *src);

#ifdef __GNUC__
#define STR_AUTO __attribute__((cleanup(str_destroy))) str_t*
#endif

#endif
