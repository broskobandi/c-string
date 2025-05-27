#ifndef C_STRING_H
#define C_STRING_H

#include "c-string-utils.h"
#include <stdbool.h>

// String object
typedef struct str str_t;
typedef struct str_priv str_priv_t;
struct str {
	str_priv_t *priv;

	// Associated functions
	//// Copies a pointer to string data into 'dest'
	STR_MUST_USE_RESULT
	str_status_t (*data)(const str_t *self, const char **dest);

	//// Replaces all occurrences of 'old_str' with 'new_str'
	STR_MUST_USE_RESULT
	str_status_t (*replace)(str_t *self, const char *old_str, const char *new_str);

	//// Compares string data with 'src' and sets 'is_same' accordingly
	STR_MUST_USE_RESULT
	str_status_t (*cmp)(const str_t *self, const char *src, bool *is_same);
};

// Factory functions
STR_MUST_USE_RESULT
str_status_t str_new(str_t **str);
STR_MUST_USE_RESULT
str_status_t str_new_from(str_t **str, const char *src);
extern bool is_str_destroyed;
void str_destroy(str_t **str);

#endif
