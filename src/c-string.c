/**
* MIT License
* Copyright (c) 2025 broskobandi
* See LICENSE file for full details.
*/

/**
 * c-string.c
 * Implementation for c-string
 */

#include "../include/c-string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct str_priv {
	char *data;
	unsigned long capacity;
	unsigned long len;
};

// Function forward declarations
static int data(const str_t *self, const char **data);
// static int cat(str_t *self, const str_t *src);
static int cat(str_t *self, const char *src);
static int replace(str_t *self, const char *old_str, const char *new_str);
static int push(str_t *self, char c);
static int pop(str_t *self, char *c);
static int len(const str_t *self, unsigned long *len);
static int capacity(const str_t *self, unsigned long *capacity);
STR_MUST_USE_RESULT
int _alloc(str_t **str, unsigned long capacity);
STR_MUST_USE_RESULT
int _realloc(str_t *str, unsigned long capacity);
void _init_str(str_t *str, unsigned long capacity, unsigned long len);

// Factory functions

#define DEFAULT_CAPACITY 16

int str_new(str_t **str) {
	if (*str) return 1;
	if (_alloc(str, DEFAULT_CAPACITY)) return 1;
	_init_str(*str, DEFAULT_CAPACITY, 0);
	return 0;
}

int str_new_from(str_t **str, const char *data) {
	if (*str) return 1;
	unsigned long capacity = (unsigned long)((float) strlen(data) * 1.5f);
	if (_alloc(str, capacity)) return 1;
	_init_str(*str, capacity, strlen(data));
	strcpy((*str)->priv->data, data);
	return 0;
}

int str_new_from_file(str_t **str, const char *file) {
	if (*str) return 1;

	FILE *src_file = fopen(file, "r");
	if (!src_file) return 1;

	if (str_new(str)) return 1;

	char c;
	while ((c = (char)fgetc(src_file)) != EOF) {
		if ((*str)->push(*str, c)) return 1;
	}

	fclose(src_file);

	return 0;
}

int is_str_destroyed = 0;
void str_destroy(str_t **str) {
	if (str && *str) {
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

// Helpers

int _alloc(str_t **str, unsigned long capacity) {
	*str = (str_t*)calloc(1, sizeof(str_t));
	if (!*str) return 1;
	(*str)->priv = (str_priv_t*)calloc(1, sizeof(str_priv_t));
	if (!(*str)->priv) {
		free(*str);
		return 1;
	}
	(*str)->priv->data = (char*)calloc(capacity, sizeof(char));
	if (!(*str)->priv->data) {
		free((*str)->priv);
		free(*str);
		return 1;
	}
	return 0;
}

int _realloc(str_t *str, unsigned long capacity) {
	char *tmp = realloc(str->priv->data, capacity);
	if (!tmp) return 1;
	str->priv->data = tmp;
	return 0;
}

void _init_str(str_t *str, unsigned long c, unsigned long l) {
	str->priv->capacity = c;
	str->priv->len = l;
	str->data = data;
	str->cat = cat;
	str->replace = replace;
	str->pop = pop;
	str->push = push;
	str->len = len;
	str->capacity = capacity;
}

// Definition for function pointers

static int data(const str_t *self, const char **data) {
	if (!self) return 1;
	*data = self->priv->data;
	return 0;
}

// static int cat(str_t *self, const str_t *src) {
// 	if (!self || !src) return 1;
//
// 	unsigned long len = self->priv->len;
// 	unsigned long capacity = self->priv->capacity;
// 	unsigned long src_len = src->priv->len;
// 	unsigned long new_len = len + src_len;
// 	if (new_len + 1 > capacity) {
// 		capacity = (unsigned long)((float)new_len * 1.5f);
// 		if (_realloc(self, capacity)) return 1;
// 	}
// 	char *data = self->priv->data;
// 	char *src_data = src->priv->data;
//
// 	memcpy(&data[len], src_data, sizeof(char) * src_len);
// 	data[new_len] = '\0';
//
// 	self->priv->capacity = capacity;
// 	self->priv->len = new_len;
//
// 	return 0;
// }
//
static int cat(str_t *self, const char *src) {
	if (!self) return 1;
	unsigned long len = self->priv->len;
	unsigned long capacity = self->priv->capacity;
	unsigned long src_len = strlen(src);
	unsigned long new_len = len + src_len;
	if (new_len + 1 > capacity) {
		capacity = (unsigned long)((float)new_len * 1.5f);
		if (_realloc(self, capacity)) return 1;
	}
	char *data = self->priv->data;

	memcpy(&data[len], src, sizeof(char) * src_len);
	data[new_len] = '\0';

	self->priv->len = new_len;
	self->priv->capacity = capacity;

	return 0;
}

static int replace(str_t *self, const char *old_str, const char *new_str) {
	if (!self) return 1;

	unsigned long *capacity = &self->priv->capacity;
	unsigned long *len = &self->priv->len;
	char *data = self->priv->data;
	unsigned int count = 0;

	for (int i = 0; data[i] != '\0'; i++) {
		if (strstr(&data[i], old_str) == &data[i]) {
			count++;
			i += (int)strlen(old_str) - 1;
		}
	}

	*len = *len - (strlen(old_str) * count) + (strlen(new_str) * count);
	//TODO:
	char new_data[*len + 1]; // this should be dynamically allocated
	int data_i = 0;
	int new_data_i = 0;

	while (data[data_i] != '\0') {
		if (strstr(&data[data_i], old_str) == &data[data_i]) {
			for (int i = 0; i < (int)strlen(new_str); i++) {
				new_data[new_data_i] = new_str[i];
				new_data_i++;
			}
			data_i += (int)strlen(old_str);
		} else {
			new_data[new_data_i] = data[data_i];
			new_data_i++;
			data_i++;
		}
	}

	new_data[new_data_i] = '\0';

	unsigned long required_capacity = (unsigned long)((float)*len * 1.5);
	if (required_capacity > *capacity) {
		if (_realloc(self, required_capacity)) return 1;
	}
	if (required_capacity < *capacity / 2) {
		if (_realloc(self, *capacity / 2)) return 1;
	}
	*capacity = required_capacity;

	strcpy(data, new_data);

	return 0;
}

static int push(str_t *self, char c) {
	if (!self) return 1;
	unsigned long *len = &self->priv->len;
	unsigned long *capacity = &self->priv->capacity;

	if ((*len) + 2 > *capacity) {
		*capacity = (unsigned long)((float)*capacity * 1.5f);
		if (_realloc(self, *capacity)) return 1;
	}
	self->priv->data[*len] = c;
	self->priv->data[*len + 1] = '\0';
	(*len)++;

	return 0;
}

static int pop(str_t *self, char *c) {
	if (!self || !self->priv->len) return 1;
	unsigned long *len = &self->priv->len;
	unsigned long *capacity = &self->priv->capacity;

	*c = self->priv->data[*len - 1];

	if (*len - 1 < *capacity / 2) {
		*capacity /= 2;
		if (_realloc(self, *capacity)) return 1;
	}

	(*len)--;
	self->priv->data[*len] = '\0';

	return 0;
}

static int len(const str_t *self, unsigned long *len) {
	if (!self) return 1;
	*len = self->priv->len;
	return 0;
}

static int capacity(const str_t *self, unsigned long *capacity) {
	if (!self) return 1;
	*capacity = self->priv->capacity;
	return 0;
}
