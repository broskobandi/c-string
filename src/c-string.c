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

#include <c-string.h>
#include <stdlib.h>
#include <string.h>

// str opaque struct definition:

struct str {
	char *data;
	ulong capacity;
	ulong len;
};

// Factory functions:

#define DEFAULT_CAPACITY 16

str_t *str_new() {
	str_t *str = calloc(1, sizeof(str_t));
	if (!str) return NULL;
	str->data = calloc(DEFAULT_CAPACITY, sizeof(char));
	if (!str->data) {
		free(str);
		return NULL;
	}
	str->capacity = DEFAULT_CAPACITY;
	str->len = 0;
	return str;
}

str_t *str_new_from(const char *src) {
	str_t *str = calloc(1, sizeof(str_t));
	if (!str) return NULL;
	ulong len = strlen(src);
	ulong capacity = len < 128 ? len * 2 : (ulong)((float)len * 1.5f);
	str->data = calloc(capacity, sizeof(char));
	if (!str->data) {
		free(str);
		return NULL;
	}
	str->capacity = capacity;
	str->len = len;
	strcpy(str->data, src);
	return str;
}

bool is_str_destroyed = false;
void str_destroy(str_t **str) {
	if (str && *str) {
		if ((*str)->data)
			free((*str)->data);
		free(*str);
		*str = NULL;
		is_str_destroyed = true;
	}
}

// Helper functions:

static int _realloc(str_t *str, ulong len, ulong *capacity) {
	if (!str || !str->data) return 1;
	ulong new_capacity = *capacity;
	if (len + 1 > *capacity) {
		new_capacity =
			*capacity < 128 ? *capacity * 2 : (ulong)((float)*capacity * 1.5f);
	} else if (len + 1 < *capacity / 2) {
		new_capacity = *capacity / 2;
	}
	if (new_capacity != *capacity) {
		*capacity = new_capacity;
		char *tmp = realloc(str->data, *capacity * sizeof(char));
		if (!tmp) return 1;
		str->data = tmp;
	}

	return 0;
}

// Functions that operate over the string object:

const char *str_data(const str_t *str) {
	if (!str || !str->data) return NULL;
	return str->data;
}

int str_replace(str_t *str, const char *old_str, const char *new_str) {
	if (!str || !old_str || !new_str) return 1;

	// Local vars
	ulong len = str->len;
	ulong capacity = str->capacity;
	ulong old_str_len = strlen(old_str);
	ulong new_str_len = strlen(new_str);

	// Num occurrecnes
	ulong count = 0;
	for (ulong i = 0; str->data[i] != '\0'; i++) {
		if (strstr(&str->data[i], old_str) == &str->data[i]) {
			count++;
			i += strlen(old_str) - 1;
		}
	}

	if (!count) return 0;

	// Result string
	len = len - (old_str_len * count) + (new_str_len * count);
	char *result = malloc(len + 1 * sizeof(char));
	ulong data_i = 0;
	ulong result_i = 0;
	while (str->data[data_i] != '\0') {
		if (strstr(&str->data[data_i], old_str) == &str->data[data_i]) {
			for (ulong i = 0; i < new_str_len; i++) {
				result[result_i] = new_str[i];
				result_i++;
			}
			data_i += old_str_len;
		} else {
			result[result_i] = str->data[data_i];
			result_i++;
			data_i++;
		}
	}
	result[result_i] = '\0';

	// Realloc
	if (_realloc(str, len, &capacity)) {
		free(result);
		return 1;
	}

	// Update values
	strcpy(str->data, result);
	str->capacity = capacity;
	str->len = len;
	free(result);

	return 0;
}

int str_len(const str_t *str) {
	if (!str || !str->data) return -1;
	return (int)str->len;
}

int str_capacity(const str_t *str) {
	if (!str || !str->data) return -1;
	return (int)str->capacity;
}

int str_push(str_t *str, char c) {
	if (!str || !str->data) return 1;
	ulong len = str->len;
	ulong capacity = str->capacity;

	len++;
	if (_realloc(str, len, &capacity)) return 1;

	str->data[len - 1] = c;
	str->data[len] = '\0';
	str->capacity = capacity;
	str->len = len;

	return 0;
}

char str_pop(str_t *str) {
	if (!str || !str->data) return -1;
	ulong len = str->len;
	ulong capacity = str->len;
	char c = str->data[len - 1];

	len--;
	if (_realloc(str, len, &capacity)) return -1;

	str->data[len] = '\0';
	str->len = len;
	str->capacity = capacity;

	return c;
}

int str_cmp(const str_t *str1, const char *str2) {
	if (!str1 || !str1->data || !str2) return -1;
	int result = -1;
	if (strcmp(str1->data, str2) == 0) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}

const char *str_append(str_t *str, const char *src) {
	if (!str || !src || !str->data) return NULL;
	ulong len = str->len;
	ulong capacity = str->len;
	ulong src_len = strlen(src);
	ulong old_len = len;

	len = len + src_len;
	if (_realloc(str, len, &capacity)) return NULL;

	memcpy(&str->data[old_len], src, src_len);
	str->data[len] = '\0';

	str->len = len;
	str->capacity = capacity;

	return str->data;
}

int str_clear(str_t *str) {
	if (!str || !str->data) return 1;
	memset(str->data, 0, str->capacity * sizeof(char));
	str->len = 0;
	if (str->capacity != DEFAULT_CAPACITY) {
		char *tmp = realloc(str->data, DEFAULT_CAPACITY * sizeof(char));
		if (!tmp) return 1;
		str->data = tmp;
		str->capacity = DEFAULT_CAPACITY;
	}

	return 0;
}
