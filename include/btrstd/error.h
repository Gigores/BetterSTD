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

__attribute__((noreturn)) void BTR_panicImpl(const char *file, int line, const char *func, const char *format, ...);

// This macro prints out the formatted error message and stops the program.
// Usage example:
// ```c
// void SomeStruct_someMethod(some_struct_t *this, int someData)
// {
//     if (this->head == NULL) BTR_panic("this is null!");
//     // ...
// }
// ```
#define BTR_panic(format, ...) BTR_panicImpl(__FILE__, __LINE__, __FUNCTION__, format __VA_OPT__(,) __VA_ARGS__)

void BTR_panicImplIf(const char *file, int line, const char *func, bool condition, const char *format, ...);

// This macro prints out the formatted error message and stops the program if the condition is true.
// Usage example:
// ```c
// void SomeStruct_someMethod(some_struct_t *this, int someData)
// {
//     BTR_panicIf(this == NULL, "this is null!");
//     // ...
// }
// ```
#define BTR_panicIf(condition, format, ...) BTR_panicImplIf(__FILE__, __LINE__, __FUNCTION__, condition, format __VA_OPT__(,) __VA_ARGS__)

// This macro panics with the given formatted error message if the given result is Err.
// Otherwise evaluates to the .value of the result.
#define BTR_expect(result, message, ...) ({                 \
    __auto_type _r = (result);                              \
    if (_r.status == BTR_ERR)                               \
        BTR_panic(message __VA_OPT__(,) __VA_ARGS__); \
    _r.value;                                               \
})

// This macro panics if the given result is Err.
// Otherwise evaluates to the .value of the result.
#define BTR_unwrap(result) ({             \
    __auto_type _r = (result);            \
    if (_r.status == BTR_ERR)             \
        BTR_panic("unwrap failed"); \
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
        BTR_panic("expected error"); \
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
#define BTR_Ok(T, Value) do { return ((T) { .status = BTR_OK, .value = (Value) }); } while(0)
// Returns an Err result from the current function.
#define BTR_Err(T, Error) do { return ((T) { .status = BTR_ERR, .error = (Error) }); } while(0)

// Can be used as a placeholer for unfinished code.
#define BTR_todo(...) BTR_panic("Not yet implemented" __VA_OPT__(": " __VA_ARGS__) )
// Can be used for the code, that will not be implemented.
#define BTR_unimplemented(...) BTR_panic("Not implemented" __VA_OPT__(": " __VA_ARGS__) )
// Can be used for deprecating code.
#define BTR_deprecated(...) BTR_panic("Deprecated" __VA_OPT__(": " __VA_ARGS__) )

#ifdef BTR_NO_PREFIX

typedef btr_result_status_t result_status_t;
#define Result BTR_Result

#define panic   BTR_panic
#define panicIf BTR_panicIf

#define expect    BTR_expect
#define unwrap    BTR_unwrap
#define unwrapOr  BTR_unwrapOr
#define unwrapErr BTR_unwrapErr
#define try       BTR_try

#define isOk  BTR_isOk
#define isErr BTR_isErr

#define Ok  BTR_Ok
#define Err BTR_Err

#define todo          BTR_todo
#define unimplemented BTR_unimplemented
#define deprecated    BTR_deprecated

#endif  // BTR_NO_PREFIX
