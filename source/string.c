
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tlib/string.h>

typedef struct tlib_string {
    char* data;
    size_t len;
    size_t cap;
} String;

String* _S_new(void)
{
    String* s = malloc(sizeof(String));
    s->data = NULL;
    s->len = 0;
    s->cap = 0;

    return s;
}

String* _S_from(const char* str)
{
    String* s = malloc(sizeof(String));
    s->len = strlen(str);
    s->cap = s->len;
    s->data = calloc(s->cap, sizeof(char));
    // copy data
    for (int i = 0; i < strlen(str); i++) {
        s->data[i] = str[i];
    }
    return s;
}

String* _S_clone(String* s)
{
    String* new = _S_from(_S_get_raw(s));
    return new;
}

void _S_free(String* s)
{
    free(s->data);
    free(s);
}

void _S_print(String* s)
{
    for (int i = 0; i < s->len; i++) {
        putchar(s->data[i]);
    }
}

void _S_println(String* s)
{
    _S_print(s);
    putchar('\n');
}

size_t _S_len(String* s)
{
    return s->len;
}

char* _S_get_raw(String* s) {
    char* raw = malloc(sizeof(char) * (s->len+1)); // + 1 for \0
    for (int i = 0; i < s->len; i++) {
        raw[i] = s->data[i];
    }
    raw[s->len] = '\0';
    return raw;
}
