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

/* c-string.c
 * Dynamic string written in C.
 * Implementation */

#include <c-string.h>
#include <string.h>

#define DEFAULT_CAPACITY 16

// str_priv opaque struct definition
struct str_priv {
	char *data;
	ulong len;
	ulong capacity;
};

// Function forward declarations
//// Helpers
static str_status_t _alloc(str_t **str, ulong capacity);
static void _init(str_t *str, ulong capacity);
static str_status_t _handle_realloc(
	str_t *str, ulong old_capacity, ulong *new_capacity, ulong new_len
);

//// Associated functions
static str_status_t append(str_t *self, const char *src);
static str_status_t replace(str_t *self, const char *old_str, const char *new_str);
static str_status_t data(const str_t *self, const char **dest);
static str_status_t len(const str_t *self, ulong *len);
static str_status_t capacity(const str_t *self, ulong *capacity);
static str_status_t push(str_t *self, char c);
static str_status_t pop(str_t *self, char *c);
static str_status_t clear(str_t *self);
static str_status_t cmp(const str_t *self, const char *pattern, bool *is_same);
static str_status_t has(const str_t *self, const char *pattern, bool *has);

// Function definitions

// Constructor

str_status_t create_str(str_t **str) {
	if (*str) return STR_NOT_EMPTY;

	str_status_t status = _alloc(str, DEFAULT_CAPACITY);
	if (status) return status;

	_init(*str, DEFAULT_CAPACITY);

	return STR_SUCCESS;
}

// Destructor
bool _is_str_destroyed = false;
void str_destroy(str_t **str) {
	if (*str && str) {
		if ((*str)->priv) {
			if ((*str)->priv->data) {
				free((*str)->priv->data);
			}
			free((*str)->priv);
		}
		free(*str);
		*str = NULL;
		_is_str_destroyed = true;
	}
}

// Helpers
static str_status_t _alloc(str_t **str, ulong capacity) {
	*str = calloc(1, sizeof(str_t));
	if (!str) return STR_ALLOC_ERROR;

	(*str)->priv = calloc(1, sizeof(str_priv_t));
	if (!(*str)->priv) {
		str_destroy(str);
		return STR_ALLOC_ERROR;
	}

	(*str)->priv->data = calloc(capacity, sizeof(char));
	if (!(*str)->priv->data) {
		str_destroy(str);
		return STR_ALLOC_ERROR;
	}

	return STR_SUCCESS;
}

static void _init(str_t *str, ulong c) {
	str->priv->capacity = c;
	str->priv->len = 0;

	str->append = append;
	str->replace = replace;
	str->data = data;
	str->len = len;
	str->capacity = capacity;
	str->push = push;
	str->pop = pop;
	str->clear = clear;
	str->cmp = cmp;
	str->has = has;
}

static str_status_t _handle_realloc(
	str_t *str, ulong old_capacity, ulong *new_capacity, ulong new_len
) {
	while (new_len + 1 > *new_capacity) {
		*new_capacity *= 2;
	}

	if (new_len + 1 < *new_capacity / 2 && *new_capacity / 2 >= DEFAULT_CAPACITY) {
		*new_capacity /= 2;
	}

	if (*new_capacity != old_capacity) {
		char *tmp = (char*)realloc(str->priv->data, *new_capacity * sizeof(char));
		if (!tmp) return STR_REALLOC_ERROR;
		str->priv->data = tmp;
	}

	return STR_SUCCESS;
}

// Associated functions
static str_status_t append(str_t *self, const char *src) {
	if (!self || !src) return STR_NULL_PTR;

	ulong old_len = self->priv->len;
	ulong old_capacity = self->priv->capacity;
	ulong new_len = old_len + strlen(src);
	ulong new_capacity = old_capacity;

	str_status_t status = _handle_realloc(self, old_capacity, &new_capacity, new_len);
	if (status) return status;

	strcpy(&self->priv->data[old_len], src);

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	return STR_SUCCESS;
}

static str_status_t replace(str_t *self, const char *old_str, const char *new_str) {
	if (!self || !old_str || !new_str) return STR_NULL_PTR;
	if (!strlen(old_str)) return STR_EMPTY;

	ulong old_len = self->priv->len;
	ulong old_capacity = self->priv->capacity;
	char *data = self->priv->data;
	ulong num_old_str = 0;
	for (ulong i = 0; data[i] != '\0'; i++) {
		if (strstr(&data[i], old_str) == &data[i]) {
			num_old_str++;
			i += strlen(old_str) - 1;
		}
	}
	ulong new_len =
		old_len - (strlen(old_str) * num_old_str) + (strlen(new_str) * num_old_str);
	ulong new_capacity = old_capacity;

	ulong data_i = 0;
	ulong result_i = 0;
	char *result = malloc((new_len + 1) * sizeof(char));

	while (data[data_i] != '\0') {
		if (strstr(&data[data_i], old_str) == &data[data_i]) {
			strcpy(&result[result_i], new_str);
			result_i += strlen(new_str);
			data_i += strlen(old_str);
		} else {
			result[result_i] = data[data_i];
			result_i++;
			data_i++;
		}
	}

	result[result_i] = '\0';

	str_status_t status = _handle_realloc(self, old_capacity, &new_capacity, new_len);
	if (status) return status;
	data = self->priv->data;

	strcpy(data, result);

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	free(result);
	return STR_SUCCESS;
}

static str_status_t data(const str_t *self, const char **dest) {
	if (!self) return STR_NULL_PTR;

	*dest = self->priv->data;

	return STR_SUCCESS;
}

static str_status_t len(const str_t *self, ulong *len) {
	if (!self) return STR_NULL_PTR;

	*len = self->priv->len;

	return STR_SUCCESS;
}

static str_status_t capacity(const str_t *self, ulong *capacity) {
	if (!self) return STR_NULL_PTR;

	*capacity = self->priv->capacity;

	return STR_SUCCESS;
}

static str_status_t push(str_t *self, char c) {
	if (!self) return STR_NULL_PTR;

	ulong old_len = self->priv->len;
	ulong new_len = old_len + 1;
	ulong old_capacity = self->priv->capacity;
	ulong new_capacity = old_capacity;

	str_status_t status = _handle_realloc(self, old_capacity, &new_capacity, new_len);
	if (status) return status;

	self->priv->data[old_len] = c;
	self->priv->data[new_len] = '\0';

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	return STR_SUCCESS;
}

static str_status_t pop(str_t *self, char *c) {
	if (!self) return STR_NULL_PTR;
	if (!self->priv->len) return STR_EMPTY;

	ulong old_len = self->priv->len;
	ulong new_len = old_len - 1;
	ulong old_capacity = self->priv->capacity;
	ulong new_capacity = old_capacity;

	*c = self->priv->data[new_len];
	self->priv->data[new_len] = '\0';

	str_status_t status = _handle_realloc(self, old_capacity, &new_capacity, new_len);
	if (status) return status;

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	return STR_SUCCESS;
}

static str_status_t clear(str_t *self) {
	if (!self) return STR_NULL_PTR;

	ulong new_len = 0;
	ulong old_capacity = self->priv->capacity;
	ulong new_capacity = DEFAULT_CAPACITY;

	memset(self->priv->data, 0, old_capacity * sizeof(char));

	if (old_capacity != new_capacity) {
		char *tmp = (char*)realloc(self->priv->data, new_capacity);
		if (!tmp) return STR_REALLOC_ERROR;
		self->priv->data = tmp;
	}

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	return STR_SUCCESS;
}

static str_status_t cmp(const str_t *self, const char *pattern, bool *is_same) {
	if (!self || !pattern) return STR_NULL_PTR;

	if (strcmp(self->priv->data, pattern) == 0) {
		*is_same = true;
	} else {
		*is_same = false;
	}

	return STR_SUCCESS;
}

static str_status_t has(const str_t *self, const char *pattern, bool *has) {
	if (!self || !pattern) return STR_NULL_PTR;

	if (strstr(self->priv->data, pattern)) {
		*has = true;
	} else {
		*has = false;
	}

	return STR_SUCCESS;
}
