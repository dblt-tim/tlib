
#ifndef TLIB_STRING_H
#define TLIB_STRING_H

#include <stddef.h>

typedef struct tlib_string String;

/// String initialization
// initialize a new empty string;
String* _S_new(void);
// initialize a string from a C-string
String* _S_from(const char* str);
// initialize a cloned string
String* _S_clone(String* s);

// free the string
void _S_free(String* s);

/// String printing
void _S_print(String* s);
void _S_println(String* s);

/// String utilities
size_t _S_len(String* s);
// NOTE: raw char* is dynamically allocated
char* _S_get_raw(String* s);

#endif // TLIB_STRING_H
