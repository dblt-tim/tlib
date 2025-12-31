

#include <tlib/alloc.h>
#include <tlib/result.h>
#include <tlib/string.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#define _A_alloc(size) \
    _A_alloc_impl((size), __FILE__, __LINE__, __func__)
#define _A_allocArray(nmemb, size) \
    _A_allocArray_impl((nmemb), (size), __FILE__, __LINE__, __func__)
#define _A_resizeArray(alloc, nmemb) \
    _A_resizeArray_impl((alloc), (nmemb), __FILE__, __LINE__, __func__)

typedef struct allocation {
    void* ptr;
    size_t size;
    
    // metadata

    String* file;
    String* func;
    int line;

    bool is_alive;
    bool is_array;
    size_t nmemb; // only used if array
} Alloc;

typedef struct {
    Alloc** list;
    size_t count;
} AllocTable;

// helper AllocTable functions
static AllocTable* AT_init() {
    AllocTable* alloc_table = malloc(sizeof(AllocTable));
    alloc_table->list = NULL;
    alloc_table->count = 0;
    return alloc_table;
};
static void AT_append(AllocTable* table, Alloc* alloc) {
    if (table->list == NULL) {
        table->count = 1;
        table->list = calloc(table->count, sizeof(Alloc*));
    } else {
        table->count++;
        table->list = reallocarray(table->list, table->count, sizeof(Alloc*));
    }
    table->list[table->count -1] = alloc;
};

// entries aren't meant to be removed for memory safety measures, idc about memory usage modern computers can handle a few MB if needed

static void AT_free_allocs(AllocTable* t) {
    for (int i = 0; i < t->count; i++) {
        _S_free(t->list[i]->file);
        _S_free(t->list[i]->func);
        free(t->list[i]);
    }
    free(t->list);
    free(t);
}

static AllocTable* allocTable;

__attribute__((constructor))
static void _A_constructor() {
    allocTable = AT_init();
}

#ifdef TLIB_TRACK_LEAKS
__attribute__((destructor))
static void _A_lookup_for_leaks() {
    Alloc* a;
    size_t size_lost;
    size_t leak_count = 0;
    size_t total_leak = 0;
    char* file;
    char* func;
    printf("== MEMORY LEAK CHECK ==\n");
    for (size_t i = 0; i < allocTable->count; i++) {
        a = allocTable->list[i];
        if (_A_is_alive(a)) {
            file = _S_get_raw(a->file);
            func = _S_get_raw(a->func);
            printf("> LEAK AT %s:%d in %s() : %p\n", file, a->line, func, a->ptr);
            free(file);
            free(func);
            size_lost = a->is_array ? 
                a->size * a->nmemb :
                a->size;
            printf("SIZE LOST : %zuB\n\n", size_lost);
            leak_count++;
            total_leak += size_lost;
        }
    }
    printf("== MEMORY CHECK SUMMARY ==\n");
    printf("TOTAL LEAKS : %zu\n", leak_count);
    printf("TOTAL SIZE LOST : %zuB\n", total_leak);

    AT_free_allocs(allocTable);
}
#else
__attribute__((destructor))
static void _A_free_allocs() {
    AT_free_allocs(allocTable);
}
#endif

Result _A_alloc_memory(size_t size)
{ // if array size should be sizeof(element)*nmemb
    void* ptr = malloc(size);
    if (ptr == NULL) 
        return Err(_S_from("[ERROR] Couldn't allocate memory block"));
    Alloc* a = malloc(sizeof(Alloc));

    if (a == NULL)
        return Err(_S_from("[ERROR] Couldn't allocate memory block for Alloc"));

    a->ptr = ptr;
    a->is_alive = true;

    return Ok(a);
}

Result _A_alloc_impl(size_t size, const char* file, const int line, const char* func)
{

    Result res = _A_alloc_memory(size);
    if (!_R_isOk(&res)) {
        _R_printErr(&res);
        return Err(_S_from("_A_alloc aborted"));
    }
    Alloc* a = _R_unwrapOk(&res);

    a->file = _S_from(file);
    a->line = line;
    a->func = _S_from(func);

    a->size = size;
    a->is_array = false;
    a->nmemb = 1; // ignored when is_array = false
    
    AT_append(allocTable, a);

    return Ok(a);
}

Result _A_allocArray_impl(size_t nmemb, size_t size, const char* file, const int line, const char* func)
{
    Result res = _A_alloc_memory(size * nmemb);
    if (!_R_isOk(&res)) {
        _R_printErr(&res);
        return Err(_S_from("_A_allocArray aborted"));
    }
    Alloc* a = _R_unwrapOk(&res);

    a->size = size;

    a->file = _S_from(file);
    a->line = line;
    a->func = _S_from(func);

    a->is_array = true;
    a->nmemb = nmemb;

    AT_append(allocTable, a);
    return Ok(a);
}

Result _A_resizeArray_impl(Alloc* alloc, size_t nmemb, const char* file, const int line, const char* func)
{
    if (!alloc->is_array)
        return Err(_S_from("can't resize non-array allocation"));
    if (nmemb < alloc->nmemb)
        return Err(_S_from("can't resize array to lower number of members"));
    alloc->is_alive = false;

    Alloc* new = malloc(sizeof(Alloc));

    if (new == NULL)
        return Err(_S_from("_A_resizeArray aborted"));

    new->ptr = reallocarray(alloc->ptr, nmemb, alloc->size);

    new->size = alloc->size;

    new->file = _S_from(file);
    new->line = line;
    new->func = _S_from(func);

    new->is_alive = true;
    new->is_array = true;
    new->nmemb = nmemb;

    AT_append(allocTable, new);

    return Ok(new);
}

Result _A_get_ptr(const Alloc* alloc)
{
    if (!_A_is_alive(alloc))
        return Err(_S_from("error : trying to retrieve dead pointer"));

    return Ok(alloc->ptr);
}

bool _A_is_alive(const Alloc* alloc)
{
    return alloc->is_alive;
}

void _A_free(Alloc* alloc)
{
    free(alloc->ptr);
    alloc->is_alive = false;
}
