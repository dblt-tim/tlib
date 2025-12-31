

#ifndef TLIB_ALLOC_H
#define TLIB_ALLOC_H

#include <stdint.h>
#include <tlib/result.h>

typedef struct allocation Alloc;


// Result<Alloc*>
Result _A_alloc(size_t size);
Result _A_allocArray(size_t nmemb, size_t size);
Result _A_resizeArray(Alloc* alloc, size_t nmemb); // NOTE : size is not necessary, as Alloc holds the cell size

// returned pointer is always alive, except if Err(msg)
// the pointer returned is meant to only be used to deref
// Result<void*>
Result  _A_get_ptr(const Alloc* alloc);

// returns true if the pointer can be deref safely
bool _A_is_alive(const Alloc* alloc);

void _A_free(Alloc* alloc);


Result _A_alloc_impl(size_t size, const char* file, const int line, const char* func);
Result _A_allocArray_impl(size_t nmemb, size_t size, const char* file, const int line, const char* func);
Result _A_resizeArray_impl(Alloc* alloc, size_t nmemb, const char* file, const int line, const char* func);

#define _A_alloc(size) \
    _A_alloc_impl((size), __FILE__, __LINE__, __func__)
#define _A_allocArray(nmemb, size) \
    _A_allocArray_impl((nmemb), (size), __FILE__, __LINE__, __func__)
#define _A_resizeArray(alloc, nmemb) \
    _A_resizeArray_impl((alloc), (nmemb), __FILE__, __LINE__, __func__)

#endif // TLIB_ALLOC_H
