#pragma once

typedef enum {
    BTR_OK,
    BTR_ERR,
} btr_result_satus_t;

#define Result(V, E) { \
    btr_result_status_t status, \
    union { \
        V value, \
        E error, \
    } \
}
