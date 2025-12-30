
#include <stdint.h>
#include <tlib/string.h>

typedef struct tlib_string {
    char* data;
    size_t len;
    size_t cap;
} String;

String* _S_new(void) {
    String* s = malloc(sizeof(String));
    s->data = NULL;
    s->len = 0;
    s->cap = 0;

    return s;
}

String* _S_from(const char* str) {
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
