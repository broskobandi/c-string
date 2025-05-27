#include "../include/c-string.h"
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

// String object
struct str_priv {
	char *data;
	ulong len;
	ulong capacity;
};

#define DEFAULT_CAPACITY 16

// Associated function forward declarations
static str_status_t data(const str_t *self, const char **dest);
static str_status_t replace(str_t *self, const char *old_str, const char *new_str);
static str_status_t cmp(const str_t *self, const char *src, bool *is_same);

// Helpers
static str_status_t _alloc(str_t **str, ulong capacity) {
	*str = calloc(1, sizeof(str_t));
	if (!*str) return STR_ALLOC_FAILURE;
	(*str)->priv = calloc(1, sizeof(str_priv_t));
	if (!(*str)->priv) {
		free(*str);
		return STR_ALLOC_FAILURE;
	}
	(*str)->priv->data = calloc(capacity, sizeof(char));
	if (!(*str)->priv->data) {
		free((*str)->priv);
		free(*str);
		return STR_ALLOC_FAILURE;
	}
	return STR_SUCCESS;
}

static void _init_str(str_t *str, ulong c, ulong l) {
	str->priv->capacity = c;
	str->priv->len = l;
	str->data = data;
	str->replace = replace;
	str->cmp = cmp;
}

static int _realloc(str_t *str, ulong capacity) {
	char *tmp = realloc(str->priv->data, capacity * sizeof(char));
	if (!tmp) return STR_REALLOC_FAILURE;
	str->priv->data = tmp;
	return STR_SUCCESS;
}

// Factory functions
str_status_t str_new(str_t **str) {
	if (*str) return STR_NOT_EMPTY;
	TRY(_alloc(str, DEFAULT_CAPACITY));
	_init_str(*str, DEFAULT_CAPACITY, 0);
	return STR_SUCCESS;
}

str_status_t str_new_from(str_t **str, const char *src) {
	if (*str) return STR_NOT_EMPTY;
	if (!src) return STR_NULL_PTR;
	ulong len = strlen(src);
	ulong capacity = len < 128 ? len * 2 : (ulong)((float)len * 1.5f);
	TRY(_alloc(str, capacity));
	_init_str(*str, capacity, strlen(src));
	strcpy((*str)->priv->data, src);
	return STR_SUCCESS;
}

bool is_str_destroyed = 0;
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
		is_str_destroyed = 1;
	}
}

// Associated function declarations
static str_status_t data(const str_t *self, const char **dest) {
	if (!self) return STR_NULL_PTR;
	*dest = self->priv->data;
	return STR_SUCCESS;
}

static str_status_t replace(str_t *self, const char *old_str, const char *new_str) {
	if (!self || !old_str || !new_str) return STR_NULL_PTR;
	// Local vars
	ulong len = self->priv->len;
	ulong capacity = self->priv->capacity;
	char *data = self->priv->data;

	// Get num occurrences
	ulong count = 0;
	for (int i = 0; data[i] != '\0'; i++) {
		if (strstr(&data[i], old_str) == &data[i]) {
			count++;
			i += strlen(old_str) - 1;
		}
	}

	// Update len
	len = len - (strlen(old_str) * count) + (strlen(new_str) * count);

	// Update capacity and realloc if necessary
	if (len > capacity) {
		capacity = len + 1 < 128 ? (len + 1) * 2 : (ulong)((float)(len + 1) * 1.5f);
		TRY(_realloc(self, capacity));
	} else if (len < capacity) {
		if (len + 1 < capacity / 2) {
			capacity /= 2;
			TRY(_realloc(self, capacity));
		}
	}

	// Create result string
	char *result = malloc(len + 1);
	ulong data_i = 0;
	ulong result_i = 0;
	while (data[data_i] != '\0') {
		if (strstr(&data[data_i], old_str) == &data[data_i]) {
			for (ulong i = 0; i < strlen(new_str); i++) {
				result[result_i] = new_str[i];
				result_i++;
			}
			data_i += strlen(old_str);
		} else {
			result[result_i] = data[data_i];
			result_i++;
			data_i++;
		}
	}
	result[result_i] = '\0';

	// Update string data
	strcpy(data, result);
	free(result);
	return STR_SUCCESS;
}

static str_status_t cmp(const str_t *self, const char *src, bool *is_same) {
	if (!self || !src || !is_same) return STR_NULL_PTR;
	if (strcmp(self->priv->data, src) == 0) {
		*is_same = true;
	} else {
		*is_same = false;
	}
	return STR_SUCCESS;
}
