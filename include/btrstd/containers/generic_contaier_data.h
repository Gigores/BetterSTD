#pragma once

#include "btrstd/error.h"

typedef enum {
    BTR_CONTAINER_ERR_OUT_OF_BOUNDS,
    BTR_CONTAINER_ERR_NOT_FOUND,
} btr_container_error_t;

typedef BTR_Result(void *, btr_container_error_t) btr_container_ptr_result_t;
typedef BTR_Result(long, btr_container_error_t) btr_container_idx_result_t;

#ifdef BTR_NO_PREFIX

typedef btr_container_error_t container_error_r;

#define CONTAINER_ERR_OUT_OF_BOUNDS BTR_CONTAINER_ERR_OUT_OF_BOUNDS
#define CONTAINER_ERR_NOT_FOUND BTR_CONTAINER_ERR_NOT_FOUND

typedef btr_container_ptr_result_t container_ptr_result_t
typedef btr_container_idx_result_t container_idx_result_t

#endif  // BTR_NO_PREFIX
