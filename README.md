# BetterSTD

BetterSTD is a library for C, that was made to "extend" C's standard library with features like different kinds of containers (list, dynamic array, ...) and utilities like logging system, string views and rust-like error handling.
This library, as might have already guessed, is quite opinionated. It was designed by myself, for myself. Just so that I don't have to reimplement stuff I already implemented in every project I work on. But if you still want to use this library, you can follow the tutorial down below.

## Using

Project requirements:
- **Compiler**: GNU C Compiler
- **C standard**: GNU C11 and newer
- **Operating system**: Linux or Windows

After obtaining the `libbtrstd.a` file, you can link with it like this:

```shell
gcc main.c -L/path/to/library -lbtrstd -o program
```

If the headers are located in a custom directory, add the include path:

```shell
gcc main.c -I/path/to/include -L/path/to/library -lbtrstd -o program
```

Then you just include headers and use the library:

```c
// #include "btrstd/btrstd.h"  // You can use this header to include the entire library
// #define BTR_NO_PREFIX       // You can also define this macro to get aliases without prefixes
#include "btrstd/logger.h"

int main(void)
{
    BTR_setLogger((btr_logger_t) {
        .file = fopen("log.log", "w+"),
        .logTime = true,
    });
    BTR_log(LOG_INFO, "We are logging!");
    fclose(BTR_getLogger()->file);
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

## Current modules

- [x] [Error handling system](./include/btrstd/error.h)
- [x] [Logging system](./include/btrstd/logger.h)
- [x] [String view](./include/btrstd/string_view.h)
- [ ] Allocators
  - [ ] Arena
  - [ ] Pool
  - [ ] Heap
- [ ] Containers
  - [x] [Borrowing Array List](./include/btrstd/containers/balist.h)
  - [x] [Borrowing Linked List](./include/btrstd/containers/bllist.h)
  - [ ] Hash Map
  - [ ] Hash Set ?
- [ ] Json
- [ ] Smart pointers
  - [ ] Unique pointer
  - [ ] Shared pointer

If you need documentation, just read throught the header files. They are well documented.

