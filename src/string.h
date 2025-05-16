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

#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __GNUC__
#define AUTO_STRING __attribute__((cleanup(destroy_string))) string_t
#define AUTO_CSTRING __attribute__((cleanup(destroy_cstring))) char
#endif

typedef struct string string_t;
typedef struct string_priv string_priv_t;
struct string {
	string_priv_t *priv;
	//Returns a heap allocated copy of string data
	//The returned string must be freed
	char* (*cstring)(const string_t *self);
	//Returns a heap allocated copy of a slice of the string data
	//The returned string must be freed
	char* (*slice)(const string_t *self, unsigned long first, unsigned long last);
	//Returns character found at 'index'
	char (*at)(const string_t *self, unsigned int index);
	//Returns length of string data
	int (*len)(const string_t *self);
	//Returns length of capacity of string object
	int (*capacity)(const string_t *self);

	//Removes and returns last character in the string data
	char (*pop)(string_t *self);
	//Concatenates existing string data with 'data'
	int (*cat)(string_t *self, const char *data);
	//Pushes 'c' at the end of string data
	int (*push)(string_t *self, char c);
	//Removes character found at 'index' and shrinks string data
	int (*remove_at)(string_t *self, unsigned long index);
	//Sets the character in string data at 'index' to 'c'
	int (*set)(string_t *self, unsigned long index, char c);
};

//Creates a heap allocated empty string_t object
string_t *create_string();
//Creates a heap allocated string_t object and initialises it with 'src'
string_t *create_string_from(const char *src);
//Frees all memory stored in 'string'
//Must be used unless using AUTO_STRING
void destroy_string(string_t **string);
//Frees all memory stored in 'cstring'
//Must be used unless using AUTO_CSTRING
void destroy_cstring(char **cstring);
//for testing the destroy functions
extern bool _is_string_destroyed;
extern bool _is_cstring_destroyed;

#endif
