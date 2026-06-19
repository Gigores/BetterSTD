#pragma once

#include "btrstd/error.h"

typedef enum {
    BTR_CONTAINER_ERR_OUT_OF_BOUNDS,
    BTR_CONTAINER_ERR_NOT_FOUND,
} btr_container_error_e;

typedef BTR_Result(void *, btr_container_error_e) btr_container_ptr_r;
typedef BTR_Result(long, btr_container_error_e) btr_container_idx_r;

#ifdef BTR_NO_PREFIX

typedef btr_container_error_e container_error_e;

#define CONTAINER_ERR_OUT_OF_BOUNDS BTR_CONTAINER_ERR_OUT_OF_BOUNDS
#define CONTAINER_ERR_NOT_FOUND BTR_CONTAINER_ERR_NOT_FOUND

typedef btr_container_ptr_r container_ptr_r;
typedef btr_container_idx_r container_idx_r;

#endif  // BTR_NO_PREFIX
