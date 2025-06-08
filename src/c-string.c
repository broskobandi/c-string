#include "../include/c-string.h"
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
static str_status_t _realloc(str_t *str, ulong capacity);
static ulong _get_num_old_str(char *data, const char *old_str);
static const char *_get_result_str(char *data, const char *old_str, const char *new_str);
static str_status_t _expand_data(char *data, ulong new_capacity, ulong new_len);
static str_status_t _shrink_data(char *data, ulong new_capacity, ulong new_len);

//// Associated functions
static str_status_t append(str_t *self, const char *src);
static str_status_t replace(str_t *self, const char *old_str, const char *new_str);
static str_status_t data(const str_t *self, const char **dest);
static str_status_t len(const str_t *self, ulong *len);
static str_status_t capacity(const str_t *self, ulong *capacity);
static str_status_t push(str_t *self, char c);
static str_status_t pop(str_t *self, char *c);
static str_status_t clear(str_t *self);
static str_status_t cmp(const str_t *self, const char *src, bool *is_same);
static str_status_t has(const str_t *self, const char *src, bool *has);

// Function definitions

// Constructor

str_status_t _str_new(str_t **str) {
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

static str_status_t _realloc(str_t *str, ulong capacity) {
	char *tmp = (char*)realloc(str->priv->data, capacity * sizeof(char));
	if (!tmp) return STR_REALLOC_ERROR;
	str->priv->data = tmp;
	return STR_SUCCESS;
}

static ulong _get_num_old_str(char *data, const char *old_str) {
	ulong num_old_str = 0;

	for (ulong i = 0; data[i] != '\0'; i++) {
		if (strstr(&data[i], old_str) == &data[i]) {
			num_old_str++;
			i += strlen(old_str) - 1;
		}
	}

	return num_old_str;
}

// Associated functions
static str_status_t append(str_t *self, const char *src) {
	if (!self || !src) return STR_NULL_PTR;

	ulong old_len = self->priv->len;
	ulong old_capacity = self->priv->capacity;
	ulong new_len = old_len + strlen(src);
	ulong new_capacity = old_capacity;

	while (new_len + 1 > new_capacity) {
		new_capacity *= 2;
	}

	if (new_capacity != old_capacity) {
		str_status_t status = _realloc(self, new_capacity);
		if (status) return status;
	}

	strcpy(&self->priv->data[old_len], src);

	self->priv->len = new_len;
	self->priv->capacity = new_capacity;

	return STR_SUCCESS;
}

static str_status_t replace(str_t *self, const char *old_str, const char *new_str) {
	if (!self || !old_str || !new_str) return STR_NULL_PTR;

	ulong old_len = self->priv->len;
	ulong old_capacity = self->priv->capacity;
	char *data = self->priv->data;
	ulong num_old_str = _get_num_old_str(data, old_str);
	ulong new_len =
		old_len - (strlen(old_str) * num_old_str) + (strlen(new_str) * num_old_str);
	ulong new_capacity = old_capacity;

	while (new_len + 1 > new_capacity) {
		new_capacity *= 2;
	}

	if (new_capacity != old_capacity) {
		str_status_t status = _realloc(self, new_capacity);
		if (status) return status;
	}



	return STR_SUCCESS;
}

static str_status_t data(const str_t *self, const char **dest) {

}

static str_status_t len(const str_t *self, ulong *len) {

}

static str_status_t capacity(const str_t *self, ulong *capacity) {

}

static str_status_t push(str_t *self, char c) {

}

static str_status_t pop(str_t *self, char *c) {

}

static str_status_t clear(str_t *self) {

}

static str_status_t cmp(const str_t *self, const char *src, bool *is_same) {

}

static str_status_t has(const str_t *self, const char *src, bool *has) {

}
