

#ifndef TLIB_RESULT_H
#define TLIB_RESULT_H

#include <tlib/string.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct tlib_result {
    union {
        void* ok_value;
        String* err_msg;
    };
    bool is_ok;
} Result;

// NOTE : if value is statically allocated,
// its lifetime must be valid outside of Ok(void*)
Result Ok(void* value);
Result Err(String* msg);

// utility functions
bool _R_isOk(Result* r);
// unwrapOk assumes is_ok has been verified
void* _R_unwrapOk(Result* r);
void _R_printErr(Result* r);
// automatic unwrap with NULL if err
void* _R_unwrap(Result* r);


#endif // TLIB_RESULT_H
