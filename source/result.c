
#include <tlib/result.h>
#include <tlib/string.h>
#include <stdbool.h>


Result Ok(void* value) {
    Result r;
    r.is_ok = true;
    r.ok_value = value;
    return r;
}

Result Err(String* msg) {
    Result r;
    r.is_ok = false;
    r.err_msg = msg;
    return r;
}

bool _R_isOk(Result* r) {
    return r->is_ok;
}

void* _R_unwrapOk(Result* r) {
    return r->ok_value;
}

void _R_printErr(Result* r) {
    _S_println(r->err_msg);
}

void* _R_unwrap(Result* r) {
    if (r->is_ok) {
        return _R_unwrapOk(r);
    } else {
        _R_printErr(r);
        return NULL;
    }
}
