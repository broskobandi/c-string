#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool _is_string_destroyed = false;
bool _is_cstring_destroyed = false;

struct string_priv {
	char *data;
	unsigned long capacity;
	unsigned long len;
};

#define DEFAULT_CAPACITY 16

static char* cstring(const string_t *self);
static char* slice(const string_t *self, unsigned long first, unsigned long last);
static char at(const string_t *self, unsigned int index);
static int len(const string_t *self);
static int capacity(const string_t *self);
static char pop(string_t *self);
static int cat(string_t *self, const char *data);
static int push(string_t *self, char c);
static int remove_at(string_t *self, unsigned long index);
static int set(string_t *self, unsigned long index, char c);
static int _expand_string_data(string_t *string, unsigned long min_capacity);
static int _shrink_string_data(string_t *string);
static void _set_function_pointers(string_t *string);
static string_t *_alloc(unsigned long capacity);

string_t *create_string() {
	string_t *string = _alloc(DEFAULT_CAPACITY);
	if (!string) return NULL;
	_set_function_pointers(string);
	string->priv->len = 0;
	return string;
}

string_t *create_string_from(const char *src) {
	string_t *string = _alloc(strlen(src) + 1);
	if (!string) return NULL;
	string_priv_t *p = string->priv;
	strcpy(p->data, src);
	_set_function_pointers(string);
	p->len = strlen(src);
	return string;
}

static string_t *_alloc(unsigned long capacity) {
	string_t *string = calloc(1, sizeof(string_t));
	if (!string) return NULL;
	string->priv = calloc(1, sizeof(string_priv_t));
	if (!string->priv){
		free(string);
		return NULL;
	}
	string->priv->data = calloc(1, capacity);
	if (!string->priv->data) {
		free(string->priv);
		free(string);
		return NULL;
	}
	string->priv->capacity = capacity;
	return string;
}

static void _set_function_pointers(string_t *string) {
	string->cstring = cstring;
	string->slice = slice;
	string->at = at;
	string->len = len;
	string->capacity = capacity;
	string->pop = pop;
	string->cat = cat;
	string->push = push;
	string->remove_at = remove_at;
	string->set = set;
}

void destroy_string(string_t **string) {
	if (string && *string) {
		if ((*string)->priv) {
			if ((*string)->priv->data)
				free((*string)->priv->data);
			free((*string)->priv);
		}
		free(*string);
		*string = NULL;
		_is_string_destroyed = true;
	}

}

void destroy_cstring(char **cstring) {
	if (cstring && *cstring) {
		free(*cstring);
		*cstring = NULL;
		_is_cstring_destroyed = true;
	}
}

static char* cstring(const string_t *self) {
	if (!self) return NULL;
	string_priv_t *p = self->priv;
	char *cstring = malloc(strlen(p->data) + 1);
	if (!cstring) return NULL;
	strcpy(cstring, p->data);
	return cstring;
}

static char* slice(const string_t *self, unsigned long first, unsigned long last) {
	if (!self) return NULL;
	string_priv_t *p = self->priv;
	unsigned long max_index = p->len - 1;
	if (first > max_index || last > max_index || first > last) return NULL;
	unsigned long slice_len = last - first + 1;
	char *cstring = malloc(slice_len + 1);
	if (!cstring) return NULL;
	memcpy(cstring, &p->data[first], slice_len);
	cstring[slice_len] = '\0';
	return cstring;
}

static char at(const string_t *self, unsigned int index) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	if (index > p->len - 1) return -1;
	return p->data[index];
}

static int len(const string_t *self) {
	if (!self) return -1;
	return (int)(self->priv->len);
}

static int capacity(const string_t *self) {
	if (!self) return -1;
	return (int)(self->priv->capacity);
}

static char pop(string_t *self) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	unsigned long len = p->len;
	char last_char = p->data[len - 1];
	p->data[len - 1] = '\0';
	p->len--;
	if (_shrink_string_data(self)) return -1;
	return last_char;
}

static int cat(string_t *self, const char *data) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	if (p->capacity < p->len + strlen(data) + 1) {
		if (_expand_string_data(self, p->len + strlen(data) + 1)) return -1;
	}
	strcpy(&p->data[p->len], data);
	p->len += strlen(data);
	return 0;
}

static int push(string_t *self, char c) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	if (p->len + 1 > p->capacity) {
		if (_expand_string_data(self, p->len + 1)) return -1;
	}
	p->data[p->len + 1] = '\0';
	p->data[p->len] = c;
	p->len++;
	return 0;
}

static int remove_at(string_t *self, unsigned long index) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	if (index >= p->len) return -1;
	for (int i = (int)index; i < (int)p->len; i++) {
		p->data[i] = p->data[i + 1];
	}
	p->len--;
	if (_shrink_string_data(self)) return -1;
	return 0;
}

static int set(string_t *self, unsigned long index, char c) {
	if (!self) return -1;
	string_priv_t *p = self->priv;
	if (index >= p->len) return -1;
	p->data[index] = c;
	return 0;
}

static int _expand_string_data(string_t *string, unsigned long min_capacity) {
	string_priv_t *p = string->priv;
	do {
		if (p->capacity < 128) {
			p->capacity *= 2;
		} else {
			p->capacity = (unsigned long)((float)p->capacity * 1.5f);
		}
		char *tmp = realloc(p->data, p->capacity);
		if (!tmp) return -1;
		p->data = tmp;
	} while (p->capacity < min_capacity);
	return 0;
}

static int _shrink_string_data(string_t *string) {
	string_priv_t *p = string->priv;
	if (p->len < p->capacity / 2 && p->capacity / 2 > DEFAULT_CAPACITY) {
		p->capacity /= 2;
		char *tmp = realloc(p->data, p->capacity);
		if (!tmp) return -1;
		p->data = tmp;
	}
	return 0;
}
