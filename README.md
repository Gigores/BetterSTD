# BetterSTD

BetterSTD is a bloated library for C, that was made to "extend" C's standard library with features like different kinds of containers (list, dynamic array, ...) and utilities like logging system, string views and clunky error handling.

This library is quite opinionated. It was designed by me, for me. Just so that I don't have to reimplement stuff I already implemented in every project I work on. But if you still want to use this library, you can follow the tutorial below.

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
// #include "btrstd/btrstd.h"  // You can use this header to include the entire library
// #define BTR_NO_PREFIX       // You can also define this macro to get aliases without prefixes
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

## Building

Requirements:
- **Compiler**: GNU C Compiler
- **Build system**: Python 3.8 or newer
- **Operating system**: Linux or Windows

To build this library you can use the following command:

```shell
# On Linux
./buildScript build
# On Windows
python buildScript build
```

The `libbtrstd.a` file will be available inside of `build/` directory.

## Current and planned

- [x] [Error handling system](./include/btrstd/error.h)
- [x] [Logging system](./include/btrstd/logger.h)
- [x] [String view](./include/btrstd/string_view.h) (Supports UTF-8)
- [x] [Smart pointers](./include/btrstd/smart_pointers.h)
- [ ] Allocators
  - [x] [Global](./include/btrstd/allocators/global.h)
  - [x] [Arena](./include/btrstd/allocators/arena.h)
  - [ ] Pool
  - [ ] Heap
  - [ ] Stack
- [ ] Containers
  - [x] [Borrowing Array List](./include/btrstd/containers/balist.h)
  - [x] [Borrowing Linked List](./include/btrstd/containers/bllist.h)
  - [x] [Bit Set](./include/btrstd/containers/bit_set.h)
  - [ ] Hash Map
  - [ ] Hash Set
  - [ ] Dynamic String (Supports UTF-8)
- [ ] Json
- [ ] TOML
- [ ] Iterators
- [ ] Slices
- [ ] Time
- [ ] File System

If you need documentation, just read through the header files. They are well documented.

