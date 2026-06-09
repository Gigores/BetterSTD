#pragma once

#include "stdbool.h"

typedef enum {
    BTR_OK,
    BTR_ERR,
} btr_result_status_t;

// Defines a generic Result type.
// Usage example:
// ```c
// typedef BTR_Result(void *, alloc_error_t) alloc_result_t;
// ```
#define BTR_Result(V, E) struct { \
    btr_result_status_t status;   \
    union {                       \
        V value;                  \
        E error;                  \
    };                            \
}

void BTR_panicNormal(const char *format, ...);
void BTR_panicErrCode(int errCode, const char *format, ...);

// This macro prints out the formatted error message and stops the program.
// Usage example:
// ```c
// void SomeStruct_someMethod(some_struct_t *this, int someData)
// {
//     if (this->head == NULL) BTR_panic("this is null!");
//     // ...
// }
// ```
// You can optionally pass a 32-bit integer as the first parameter to make the program exit with this code.
// The default exit code is 1.
#define BTR_panic(first, ...) _Generic((first), \
    const char *: BTR_panicNormal,              \
    char *: BTR_panicNormal,                    \
    int: BTR_panicErrCode,                      \
)(first __VA_OPT__(,) __VA_ARGS__)

void BTR_panicNormalIf(bool condition, const char *format, ...);
void BTR_panicErrCodeIf(bool condition, int errCode, const char *format, ...);

// This macro prints out the formatted error message and stops the program if the condition is true.
// Usage example:
// ```c
// void SomeStruct_someMethod(some_struct_t *this, int someData)
// {
//     BTR_panicIf(this == NULL, "this is null!");
//     // ...
// }
// ```
// You can optionally pass a 32-bit integer as the second parameter to make the program exit with this code.
// The default exit code is 1.
#define BTR_panicIf(condition, second, ...) _Generic((second), \
    const char *: BTR_panicNormalIf,                           \
    char *: BTR_panicNormalIf,                                 \
    int: BTR_panicErrCodeIf,                                   \
)(condition, second __VA_OPT__(,) __VA_ARGS__)

// This macro panics with the given formatted error message if the given result is Err.
// Otherwise evaluates to the .value of the result.
#define BTR_expect(result, message, ...) ({                 \
    __auto_type _r = (result);                              \
    if (_r.status == BTR_ERR)                               \
        BTR_panicNormal(message __VA_OPT__(,) __VA_ARGS__); \
    _r.value;                                               \
})

// This macro panics if the given result is Err.
// Otherwise evaluates to the .value of the result.
#define BTR_unwrap(result) ({             \
    __auto_type _r = (result);            \
    if (_r.status == BTR_ERR)             \
        BTR_panicNormal("unwrap failed"); \
    _r.value;                             \
})

// This macro evaluates to the given "fallback" value if the result is Err.
// Otherwise evaluates to the .value of the result.
#define BTR_unwrapOr(result, fallback) ({ \
    __auto_type _r = (result);            \
    _r.status == BTR_OK                   \
        ? _r.value                        \
        : (fallback);                     \
})

// This macro evaluates to the .error of the result if it is Err.
// Otherwise panics.
#define BTR_unwrapErr(result) ({           \
    __auto_type _r = (result);             \
    if (_r.status == BTR_OK)               \
        BTR_panicNormal("expected error"); \
    _r.error;                              \
})

// Propagates an error.
// If `result` is Err, immediately returns it from the current function.
// If `result` is Ok, evaluates to the contained value.
// Example:
// ```c
// int value = BTR_try(parseInt("42"));
// ```
#define BTR_try(result) ({     \
    __auto_type _r = (result); \
    if (_r.status == BTR_ERR)  \
        return _r;             \
    _r.value;                  \
})

// Checks if the given result is Ok
#define BTR_isOk(result) ((result).status == BTR_OK)
// Checks if the given result is Err
#define BTR_isErr(result) ((result).status == BTR_ERR)

// Returns an Ok result from the current function.
#define BTR_Ok(T, Value) do { return ((T) { .status = BTR_OK, .value = (Value) }) } while(0)
// Returns an Err result from the current function.
#define BTR_Err(T, Error) do { return ((T) { .status = BTR_ERR, .error = (Error) }) } while(0)

#ifdef BTR_NO_PREFIX

typedef btr_result_status_t result_status_t;
#define Result BTR_Result

#define panicNormal BTR_panicNormal
#define panicErrCode BTR_panicErrCode
#define panic BTR_panic

#define panicNormalIf BTR_panicNormalIf
#define panicErrCodeIf BTR_panicErrCodeIf
#define panicIf BTR_panicIf

#define expect BTR_expect
#define unwrap BTR_unwrap
#define unwrapOr BTR_unwrapOr
#define unwrapErr BTR_unwrapErr
#define try BTR_try

#define isOk BTR_isOk
#define isErr BTR_isErr

#define Ok BTR_Ok
#define Err BTR_Err

#endif  // BTR_NO_PREFIX
