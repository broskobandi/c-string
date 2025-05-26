/**
* MIT License
* Copyright (c) 2025 broskobandi
* See LICENSE file for full details.
*/

/**
 * c-string.h
 * Dynamic string for C that aims to be type and memory safe.
 */

#ifndef C_STRING_H
#define C_STRING_H

/** This ensures that warning is always issued when the function's
 * return value is ignored. Required GCC or clang. */
#ifdef __GNUC__
#define MUST_USE_RESULT __attribute__((warn_unused_result))
#else
#define MUST_USE_RESULT
#endif

/** String object */
typedef struct str str_t;
typedef struct str_priv str_priv_t;
struct str {
	/** opaque struct for storing private data */
	str_priv_t *priv;

	/** Declaration of funcion pointers: */

	/** Saves a const pointer to string data in 'data' */
	MUST_USE_RESULT
	int (*data)(const str_t *self, const char **data);

	/** Appends 'src' at the end of string data */
	MUST_USE_RESULT
	int (*cat)(str_t *self, const str_t *src);

	/** Replaces all occurrences of 'old_str' with 'new_str' */
	MUST_USE_RESULT
	int (*replace)(str_t *self, const char *old_str, const char *new_str);

	/** Appends 'c' at the end of string data */
	MUST_USE_RESULT
	int (*push)(str_t *self, char c);

	/** Removes last char and saves it in 'c' */
	MUST_USE_RESULT
	int (*pop)(str_t *self, char *c);

	/** Saves the length of string data in 'len' */
	MUST_USE_RESULT
	int (*len)(const str_t *self, unsigned long *len);

	/** Saves the capacity of string data in 'capacity' */
	MUST_USE_RESULT
	int (*capacity)(const str_t *self, unsigned long *capacity);
};

/** Creates a new instance of str_t. str must be NULL */
MUST_USE_RESULT
int str_new(str_t **str);
/** Creates a new instance of str_t with the content of data.
 * 'str' must be NULL */
MUST_USE_RESULT
int str_new_from(str_t **str, const char *data);
extern int is_str_destroyed;
/* Frees up all memory allocated in str object */
void str_destroy(str_t **str);


/**
 * The Following macro wrappers are designed to make the workflow more ergonomic.
 * This is achieved by making each function that can fail automatically retrun 
 * from the current function with either 0 on success or 1 on failure.
 * Getter macros implicitly create and return the desired data so they
 * can be used to initialise variables directly. 
 * All of the following macros require either GCC or clang.
 */

#ifdef __GNUC__
#define STR_AUTO_T\
/** Objects created with STR_AUTO_T are automatically freed at the end of their scope */\
	__attribute__((cleanup(str_destroy))) str_t
#define STR_NEW\
/** Returns a new str_t instance or exits the current function with status code 1 */\
	({\
	 	str_t *str = NULL;\
		if (str_new(&str)) return 1;\
		str;\
	 })
#define STR_NEW_FROM(data)\
/** Returns a new str_t instance or exits the current function with status code 1 */\
	({\
	 	str_t *str = NULL;\
		if (str_new_from(&str, data)) return 1;\
		str;\
	 })
#define STR_DATA(str)\
/** Returns a const pointer to str->priv->data
 * or exits the current function with status code 1 */\
	({\
	 	if (!str) return 1;\
		const char *data = NULL;\
		if (str->data(str, &data)) return 1;\
		data;\
	 })
#define STR_CAT(str, src)\
/** Appends src at the end of str or exits the current function with status code 1 */\
	do {\
		if (!str || !src) return 1;\
		if (str->cat(str, src)) return 1;\
	} while(0)
#define STR_REPLACE(str, old_str, new_str)\
/** Replaces all occurrences of old_str with new_str or exits the current function with status code 1 */\
	do {\
		if (!str) return 1;\
		if (str->replace(str, old_str, new_str)) return 1;\
	} while(0)
#define STR_PUSH(str, c)\
/** Appends c at the end of str data or exits the current function with status code 1 */\
	do {\
		if (!str) return 1;\
		if (str->push(str, c)) return 1;\
	} while (0)
#define STR_POP(str)\
/** Removes and returns the last char in str or exits the current function with status code 1 */\
	({\
		if (!str) return 1;\
	 	char c = {};\
		if (str->pop(str, &c)) return 1;\
		c;\
	 })
#define STR_LEN(str)\
/** Returns the length of str or exits the current function with status code 1 */\
	({\
	 	if (!str) return 1;\
		unsigned long len = {};\
		if (str->len(str, &len)) return 1;\
		len;\
	 })
#define STR_CAPACITY(str)\
/** Returns the capacity of str or exits the current function with status code 1 */\
	({\
	 	if (!str) return 1;\
		unsigned long capacity = {};\
		if (str->capacity(str, &capacity)) return 1;\
		capacity;\
	 })

#else
#define MUST_USE_RESULT
#endif

#endif
