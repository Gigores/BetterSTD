# C Coding Style Conventions

These are the code conventions used by BetterSTD.

### **Defines**: UPPER_SNAKE_CASE or camelCase
Its better to use UPPER_SNAKE_CASE by default:
```c
#define BTR_NO_PREFIX
```
The exception is macros that expand into a function or a type without a namespace prefix:
```c
#define BAList BTR_BAList

#define BAList_make      BTR_BAList_make
#define BAList_makeFrom  BTR_BAList_makeFrom
#define BAList_clone     BTR_BAList_clone
```

### **Macros**: UPPER_SNAKE_CASE or camelCase
If a macro tries to mimic a regular function in its behaviour, better use camelCase:
```c
#define BTR_try(result) ({     \
    __auto_type _r = (result); \
    if (_r.status == BTR_ERR)  \
        return _r;             \
    _r.value;                  \
})
```
Otherwise, if a macro expands into something other than something that behaves like a
function it should use UPPER_SNAKE_CASE:
```c
#define BTR_BLLIST_FOREACH(LIST, i)           \
    void *i;                                  \
    for (                                     \
        btr_bllist_node_s *_n = (LIST)->head; \
        _n != NULL && ((i = _n->payload), 1); \
        _n = _n->next                         \
    )
```

### **Variables**: camelCase
Example:
```c
size_t newCapacity = capacity * 2;
```

### **Global constants**: UPPER_SNAKE_CASE
Example:
```c
const int MAX_VALUE = 0;
```

### **Pointers**: type *pointer
Example:
```c
void *data = this->head->payload;
```

### **Float values**: always x.xf
Example:
```c
const float MAX_LOAD_VALUE = 1.5f;
```

### **Access operators**: value.field
Example:
```c
int result = a.b
result += c->d
```

### **Other infix operators**: value1 + value2
Example:
```c
int result = a + b / c;
```

### **Enum**, **Struct**, **Union**: PascalCase
Example:
```c
typedef struct BTR_TableEntry {
    void *key, *value;
    struct BTR_TableEntry *next;
} btr_table_entry_s;
```

### **Enum members**: UPPER_SNAKE_CASE
Example:
```c
typedef enum BTR_LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
} btr_log_level_e;
```

### **Struct** & **union members**: camelCase
Example:
```c
typedef struct BTR_OAList {
    void *data;
    size_t capacity;
    size_t count;
    size_t itemSize;
} btr_oalist_s;
```

### **Functions**: camelCase
Example:
```c
btr_hash_t BTR_hashCString(const void *);
bool BTR_compareCString(const void *, const void *);
```

### **Function parameters**: camelCase
Example:
```c
btr_bllist_s BTR_BLList_makeFrom(void *items[], size_t itemCount, btr_allocator_s *allocator);
```

### **Types**: snake_case + postfix
Its better to generally use cnake_case, but I need to specify different kind of postfixes:
- `_s` for structs:
```c
typedef struct BTR_BitSet {
    char *data;
    size_t bitCount;
    btr_allocator_s *allocator;
} btr_bit_set_s;
```
- `_u` for unions:
```c
typedef union Union {
    // ...
} union_u;
```
- `_e` for enums:
```c
typedef enum BTR_ResultStatus {
    BTR_OK,
    BTR_ERR,
} btr_result_status_e;
```
- `_r` or `_result_s` for result types:
```c
typedef BTR_RESULT(bool, btr_container_error_e) btr_bit_r;
```
- `_t` for general types:
```c
typedef unsigned long long btr_hash_t;
```

## Other conventions

- For structs initialization always use designated initializer or zero initializer.
- Avoid trailing spaces.
- Comments should always start with a space.
- Place all braces on a new line:
```c
if (condition)
{
    // ...
}
```
- All functions, structs, unions, enums and types can be prefixed with:
  - A namespace
  - A type/struct/union/enum, it is associated with
There can be multiple such prefixes at a time, but more than namespace + type is
not recommended. Prefixes should be separated with an underscore.
