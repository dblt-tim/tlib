

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

void _A_free(const Alloc* alloc);

#endif // TLIB_ALLOC_H
