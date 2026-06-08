#pragma once

typedef enum {
    BTR_OK,
    BTR_ERR,
} btr_result_status_t;

#define BTR_Result(V, E) struct { \
    btr_result_status_t status;   \
    union {                       \
        V value;                  \
        E error;                  \
    };                            \
}

void BTR_panicNormal(const char *format, ...);
void BTR_panicErrCode(int errCode, const char *format, ...);

#define BTR_panic(first, ...) _Generic((first), \
    const char *: BTR_panicNormal,              \
    char *: BTR_panicNormal,                    \
    int: BTR_panicErrCode,                      \
)(first __VA_OPT__(,) __VA_ARGS__)

#define BTR_expect(result, message, ...) ({                 \
    __auto_type _r = (result);                              \
    if (_r.status == BTR_ERR)                               \
        BTR_panicNormal(message __VA_OPT__(,) __VA_ARGS__); \
    _r.value;                                               \
})

#define BTR_unwrap(result) ({             \
    __auto_type _r = (result);            \
    if (_r.status == BTR_ERR)             \
        BTR_panicNormal("unwrap failed"); \
    _r.value;                             \
})

#define BTR_unwrapOr(result, fallback) ({ \
    __auto_type _r = (result);            \
    _r.status == BTR_OK                   \
        ? _r.value                        \
        : (fallback);                     \
})

#define BTR_unwrapErr(result) ({           \
    __auto_type _r = (result);             \
    if (_r.status == BTR_OK)               \
        BTR_panicNormal("expected error"); \
    _r.error;                              \
})

#define BTR_try(result) ({     \
    __auto_type _r = (result); \
    if (_r.status == BTR_ERR)  \
        return _r;             \
    _r.value;                  \
})

#define BTR_isOk(result) ((result).status == BTR_OK)
#define BTR_isErr(result) ((result).status == BTR_ERR)

#define BTR_Ok(T, Value) ((T) { .status = BTR_OK, .value = (Value) })
#define BTR_Err(T, Error) ((T) { .status = BTR_ERR, .error = (Error) })
