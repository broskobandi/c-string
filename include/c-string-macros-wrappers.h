#ifndef C_STRING_MACRO_WRAPPERS_H
#define C_STRING_MACRO_WRAPPERS_H

#include "c-string-utils.h"

// Macro wrappers. gcc and clang only
#ifdef __GNUC__
#define STR_NEW\
	({\
	 	str_t *str = NULL;\
		TRY(str_new(&str));\
		str;\
	 })
#define STR_NEW_FROM(src)\
	({\
		if (!src) return STR_NULL_PTR;\
	 	str_t *str = NULL;\
		TRY(str_new_from(&str, src));\
		str;\
	 })
#define STR_DATA(str)\
	({\
	 	const char *dest = NULL;\
		TRY(str->data(str, &dest));\
		dest;\
	 })
#define STR_REPLACE(str, old_str, new_str)\
	do {\
		if (!str || !old_str || !new_str) return STR_NULL_PTR;\
		TRY(str->replace(str, old_str, new_str));\
	} while (0)
#define STR_CMP(str, src)\
	({\
	 	if (!str || !src) return STR_NULL_PTR;\
		bool is_same = {};\
		TRY(str->cmp(str, src, &is_same));\
		is_same;\
	 })
#endif

#endif
