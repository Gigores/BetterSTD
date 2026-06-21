#pragma once

#ifdef BTR_INCLUDE_ALL

#include "btrstd/containers/borrowing_hash_table.h"
#include "btrstd/containers/borrowing_linked_list.h"
#include "btrstd/string_view.h"


typedef btr_bhtable_s     btr_json_object_t;
typedef btr_bllist_s      btr_json_array_t;
typedef btr_string_view_s btr_json_string_t;
typedef double            btr_json_number_t;
typedef bool              btr_json_bool_t;
typedef char              btr_json_null_t;

typedef struct BTR_JsonValue {
    enum {
        BTR_JSON_OBJECT,
        BTR_JSON_ARRAY,
        BTR_JSON_STRING,
        BTR_JSON_NUMBER,
        BTR_JSON_BOOL,
        BTR_JSON_NULL,
    } type;
    union {
        btr_json_object_t object;
        btr_json_array_t  array;
        btr_json_string_t string;
        btr_json_number_t number;
        btr_json_bool_t   boolean;
        btr_json_null_t   null;
    };
    btr_allocator_s *allocator;
} btr_json_value_s;

btr_json_value_s BTR_jsonDeserialize(const char *);

void BTR_JsonValue_free(btr_json_value_s *);

#ifdef BTR_NO_PREFIX

#define JsonValue BTR_JsonValue

typedef btr_json_value_s  json_value_s;
typedef btr_json_object_t json_object_t;
typedef btr_json_array_t  json_array_t;
typedef btr_json_string_t json_string_t;
typedef btr_json_number_t json_number_t;
typedef btr_json_bool_t   json_bool_t;
typedef btr_json_null_t   json_null_t;

#define JSON_OBJECT BTR_JSON_OBJECT
#define JSON_ARRAY  BTR_JSON_ARRAY
#define JSON_STRING BTR_JSON_STRING
#define JSON_NUMBER BTR_JSON_NUMBER
#define JSON_BOOL   BTR_JSON_BOOL
#define JSON_NULL   BTR_JSON_NULL

#define jsonDeserialize BTR_jsonDeserialize
#define JsonValue_free  BTR_JsonValue_free

#endif  // BTR_NO_PREFIX

#endif  // BTR_INCLUDE_ALL
