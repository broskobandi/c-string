#ifndef C_STRING_UTILS_H
#define C_STRING_UTILS_H

// gcc and clang specific macros for convenience
#ifdef __GNUC__
#define STR_MUST_USE_RESULT __attribute__((warn_unused_result))
#define STR_AUTO_T __attribute__((cleanup(str_destroy))) str_t *
#else
#define STR_MUST_USE_RESULT
#endif

// Debugging
#define STR_STATUS_LIST\
	X(STR_SUCCESS)\
	X(STR_ALLOC_FAILURE)\
	X(STR_REALLOC_FAILURE)\
	X(STR_NOT_EMPTY)\
	X(STR_NULL_PTR)\
	X(STR_INT_UNDERFLOW)\

typedef enum str_status {
#define X(name) name,
	STR_STATUS_LIST
#undef X
} str_status_t;

extern const char *str_status_arr[];

void print_status(
	const char *function,
	const char *status_str,
	const char *file,
	int line
);

#ifndef NDEBUG
#define TRY(function)\
	do {\
		str_status_t status = function;\
		if (status) {\
			print_status(#function, str_status_arr[status], __FILE__, __LINE__);\
			return status;\
		}\
	} while (0);
#else
#define TRY(function)\
	do {\
		str_status_t status = function;\
		if (status) return status;\
	} while (0);
#endif

#endif
