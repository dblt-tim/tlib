

#include <tlib/alloc.h>
#include <tlib/result.h>
#include <tlib/string.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct allocation {
    void* ptr;
    size_t size;
    
    bool is_alive;
    bool is_array;
    size_t nmemb; // only used if array
} Alloc;

typedef struct {
    Alloc* list;
    size_t count;
} AllocTable;

// helper AllocTable functions
AllocTable* AT_init() {
    AllocTable alloc_table = malloc(sizeof(AllocTable));
    alloc_table->list = NULL;
    alloc_table->count = 0;
    return alloc_table;
};
void AT_append(AllocTable table, Alloc* alloc) {
    if (table->list == NULL) {
        table->count = 1;
        table->list = calloc(table->count, sizeof(Alloc));
    } else {
        table->count++;
        table->list = reallocarray(table->list, table->count, sizeof(Alloc));
    }
    table->list[table->count -1] = alloc;
};

// entries aren't meant to be removed for memory safety measures, idc about memory usage modern computers can handle a few MB if needed

AllocTable* allocTable;

__attribute__((constructor))
static void _A_constructor() {
    allocTable = AT_init;
}

#ifdef TLIB_TRACK_LEAKS
__attribute__((destructor))
static void _A_lookup_for_leaks() {
    Alloc* a;
    size_t size_lost;
    size_t leak_count = 0;
    size_t total_leak = 0;
    printf("MEMORY LEAK CHECK\n");
    for (size_t i = 0; i < allocTable->count; i++) {
        a = allocTable->list[i];
        if (_A_is_alive(a)) {
            printf("LEAK AT : %p\n", a->ptr);
            size_lost = a->is_array ? 
                a->size * a->nmemb :
                a->size;
            printf("SIZE LOST : %zu\n\n", size_lost);
            leak_count++;
            total_leak += size_lost;
        }
    }
}
#endif
