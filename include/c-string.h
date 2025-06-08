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

/* c-string.h
 * Dynamic string written in C.*/

#ifndef STR_H
#define STR_H

#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

/* Status codes */
typedef enum str_status {
	STR_SUCCESS,
	STR_ALLOC_ERROR,
	STR_REALLOC_ERROR,
	STR_NOT_EMPTY,
	STR_EMPTY,
	STR_NULL_PTR
} str_status_t;

/* String object struct forward declaration */
typedef struct str str_t;

/* This opaque struct is used to encapsulate the data contained
 * in the string object while allowing exposureof the function pointers. */
typedef struct str_priv str_priv_t;

/* Macro warppers. 
 * NOTE: gcc / clang only!
 *
 * These macros can only be used with clang or gcc.
 * The basic function pointers found further down are pure C and
 * therefore are much more portable.
 * However, if your development environment allows gcc and clang extensions,
 * I recommend that you use these macros instead of the pure 
 * function pointers directly for increased type and memory safety
 * and consistency.
 * All of these macros implicitly check the resulting str_status_t
 * and exit early from the caller function upon failure. On success,
 * getter macros return the desired value directly so they can be used for 
 * in line initialisation.
 * */

#define TRY(function)\
	\
	/* Executes 'function' and checks the resulting status code.
	 * If the status code is not 'STR_SUCCESS', it returns early
	 * from the caller function with the resulting status code.
	 * This function is implicitly called by all macro wrappers
	 * provided by the c-string library. Direct usage by the user 
	 * is only justified if the library functions are being used
	 * directly istead of the macro wrappers. */\
	\
	do {\
		int status = (int)function;\
		if (status) return status;\
	} while(0)

#ifdef __GNUC__
#define MUST_USE_RESULT\
	\
	/* Issues warning when ignoring a return value even if 
	 * -Wunused-result is not set.*/\
	\
	__attribute__((warn_unused_result))
#else
#define MUST_USE_RESULT\
	\
	/* This can only be used with clang or gcc. */
#endif

#define str_auto\
	\
	/* Used when initialising the str_t* object.
	 * Ensures that resources are automaticall freed
	 * when the object goes out of scope.*/\
	\
	__attribute__((cleanup(str_destroy))) str_t *

#define str_new(...)\
	\
	/* Returns a new instance of str_t.
	 * Optionally, variable number of strings can be passed to this
	 * function to initialise the string object with.
	 * Returns early from the caller with a status code on failure.
	 * str_t initialised must be null.*/\
	\
	({\
	 	str_t *str = NULL;\
		TRY(create_str(&str));\
	 	const char *args[] = {__VA_ARGS__};\
		uint num_args = sizeof(args) / sizeof(char*);\
		for (uint i = 0; i < num_args; i++) {\
			str_append(str, args[i]);\
		}\
		str;\
	 })

#define str_append(str, src)\
	\
	/* Appends 'src' at the end of 'str'.
	 * Returns early from the caller with a status code on failure.*/\
	\
	do {\
		if (!str) return STR_NULL_PTR;\
		TRY(str->append(str, src));\
	} while(0)

#define str_replace(str, old_str, new_str)\
	\
	/* Replaces all instances of 'old_str' to 'new_str' in 'str'.
	 * Returns early from the caller with a status code on failure.*/\
	\
	do {\
		if (!str) return STR_NULL_PTR;\
		TRY(str->replace(str, old_str, new_str));\
	} while (0)

#define str_data(str)\
	\
	/* Returns a pointer to the data stored in str.
	 * This is reference and not a copy!
	 * Changes made to the original content will be reflected through
	 * this reference.
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		const char *dest = NULL;\
		TRY(str->data(str, &dest));\
		dest;\
	 })

#define str_len(str)\
	\
	/* Returns the length of 'str'
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		ulong len = 0;\
		TRY(str->len(str, &len));\
		len;\
	 })

#define str_capacity(str)\
	\
	/* Returns the capacity of 'str'
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		ulong capacity = 0;\
		TRY(str->capacity(str, &capacity));\
		capacity;\
	 })

#define str_push(str, c)\
	\
	/* Appends 'c' at the end of 'str'
	 * Returns early from the caller with a status code on failure.*/\
	\
	 do {\
	 	if (!str) return STR_NULL_PTR;\
		TRY(str->push(str, c));\
	 } while(0)

#define str_pop(str)\
	\
	/* Removes and returns last char of 'str'
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		char c;\
		TRY(str->pop(str, &c));\
		c;\
	 })

#define str_clear(str)\
	\
	/* Clears content of 'str'.
	 * Returns early from the caller with a status code on failure.*/\
	\
	 do {\
	 	if (!str) return STR_NULL_PTR;\
		TRY(str->clear(str));\
	 } while (0)

#define str_cmp(str, pattern)\
	\
	/* Compares contents of 'str' and 'pattern'.
	 * Returns a boolean indicating whether they are identical.
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		bool is_same;\
		TRY(str->cmp(str, pattern, &is_same));\
		is_same;\
	})

#define str_has(str, pattern)\
	\
	/* Checks if 'pattern' is present in 'str' and returns a boolean
	 * that indicates the result.
	 * Returns early from the caller with a status code on failure.*/\
	\
	({\
	 	if (!str) return STR_NULL_PTR;\
		bool has;\
		TRY(str->has(str, pattern, &has));\
		has;\
	})

/* Function pointers. 
 * Only to be used if the development environment doesn't allow gcc or 
 * clang extensions or if direct control of required. */

struct str {
	str_priv_t *priv;

	/* Appends 'src' at the end of str */
	MUST_USE_RESULT
	str_status_t (*append)(str_t *self, const char *src);

	/* Replaces all occurrences of 'old_str' with 'new_str' in str */
	MUST_USE_RESULT
	str_status_t (*replace)(str_t *self, const char *old_str, const char *new_str);

	/* Copies a pointer to the content of str into dest.
	 * This is a reference and not a copy of the content!
	 * Changes made to the original content will be reflected through this reference. */
	MUST_USE_RESULT
	str_status_t (*data)(const str_t *self, const char **dest);

	/* Returns the length of str. */
	MUST_USE_RESULT
	str_status_t (*len)(const str_t *self, ulong *len);

	/* Returns the capacity of str. */
	MUST_USE_RESULT
	str_status_t (*capacity)(const str_t *self, ulong *capacity);

	/* Appends 'c' at the end of str. */
	MUST_USE_RESULT
	str_status_t (*push)(str_t *self, char c);

	/* Removes and returns the last character of str. */
	MUST_USE_RESULT
	str_status_t (*pop)(str_t *self, char *c);

	/* Clears content of str */
	MUST_USE_RESULT
	str_status_t (*clear)(str_t *self);

	/* Compares str with 'pattern' and sets 'is_same' to true if they are the same. */
	MUST_USE_RESULT
	str_status_t (*cmp)(const str_t *self, const char *pattern, bool *is_same);

	/* Checks if str has 'pattern' in it and sets 'has' to true of so. */
	MUST_USE_RESULT
	str_status_t (*has)(const str_t *self, const char *pattern, bool *has);
};

/* Creates new instance of str_t.
 * 'str' must be NULL! */
MUST_USE_RESULT
str_status_t create_str(str_t **str);

/* Frees all memory allocated in 'str' */
void str_destroy(str_t **str);

/* This var is important for testing str_destroy() */
extern bool _is_str_destroyed;





#endif
