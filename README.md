# BetterSTD

BetterSTD is a bloated general-purpose utility library for modern ***GNU C***, that was made to
"extend" C's standard library with features like different kinds of containers (list, dynamic
array, ...) and utilities like logging system, string views and clunky error handling. The
library is also cross-platform and is being tested on both Windows and Linux.

This library is opinionated, container implementations are slow and the build system is in python
like wtf. It was designed by me, for me. Just so that I don't have to reimplement stuff I already
implemented and rely on other people's code as much. But if you still want to use this library,
you can follow the tutorial below.

## Using

Project requirements:
- **Compiler**: GNU C Compiler
- **C standard**: GNU C11 and newer
- **Operating system**: Linux or Windows

After obtaining the `libbtrstd.a` file, you can link with it like this:

```shell
gcc main.c -I/path/to/include -L/path/to/library -lbtrstd -o program
```

Then you just include headers and use the library:

```c
// #include "btrstd/btrstd.h" // You can use this header to include the entire library
// #define BTR_NO_PREFIX      // You can also define this macro to get aliases without prefixes
#include "btrstd/smart_pointers.h"
#include "btrstd/logger.h"

int main(void)
{
    BTR_autoFile file = fopen("log.log", "w+");
    BTR_setLogger((btr_logger_t) {
        .file = file,
    });
    BTR_log(LOG_INFO, "We are logging!");
    return 0;
}
```

For documentation read header files.

## Building

Requirements:
- **Compiler**: GNU C Compiler
- **Build system**: UV
- **Operating system**: Linux or Windows

To build this library you can use the following command:

```shell
# On Linux
./build.py build
# On Windows
uv run python build.py build
```

The resulting `libbtrstd.a` static library will be placed inside the `build/` directory.

## Roadmap

- [x] [Error handling system](./include/btrstd/error.h)
- [x] [Logging system](./include/btrstd/logger.h)
- [x] [String view](./include/btrstd/string_view.h) (Supports UTF-8)
- [x] [Auto pointers](./include/btrstd/smart_pointers.h)
- [x] [DataTime](./include/btrstd/datetime.h)
- [ ] String (Supports UTF-8)
- [ ] Iterators
- [ ] Slices
- [ ] File System

**Allocators**
- [x] [Global](./include/btrstd/allocators/global.h)
- [x] [Arena](./include/btrstd/allocators/arena.h)
- [x] [Pool](./include/btrstd/allocators/pool.h)
- [ ] Tracking
- [ ] Stack
- [ ] Heap

**Containers**
- [x] Hash Table [Borrowing](./include/btrstd/containers/borrowing_hash_table.h)
- [x] Array List [Borrowing](./include/btrstd/containers/borrowing_array_list.h) [Owning](./include/btrstd/containers/owning_array_list.h)
- [x] Linked List [Borrowing](./include/btrstd/containers/borrowing_linked_list.h) [Owning](./include/btrstd/containers/owning_linked_list.h)
- [x] [Bit Set](./include/btrstd/containers/bit_set.h)
- [ ] Hash Set

