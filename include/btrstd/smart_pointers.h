#pragma once

#include "stdio.h"

// These are pointers, that perform proper cleanup automatically.

void BTR_autoFileCleanup(FILE **file);
void BTR_autoHeapCleanup(void *p);

// Defines a *variable* with a cleanup function
// Example:
// ```c
// struct Player {
//     long long x, y;
// };
// struct Player *Player_make() {
//     return calloc(1, sizeof(struct Player));
// }
// void Player_free(struct Player **player) {
//     free(*(void **)player);
// }
// BTR_autoPtr(struct Player, Player_free) player = Player_make();
// // Will be freed automatically
// ```
// The cleanup function is basically being called like this:
// ```c
// cleanup_fn(&pointer)
// ```
#define BTR_autoPtr(T, cleanup_fn) __attribute__((cleanup(cleanup_fn))) T *
// Convenience "autoPtr" macro for file descriptors.
// Example:
// ```c
// BTR_autoFile file = fopen("log.log", "w+");
// // Will be freed automatically
// ```
#define BTR_autoFile __attribute__((cleanup(BTR_autoFileCleanup))) FILE *
// Convenience "autoPtr" macro for heap-allocated pointers.
// Example:
// ```c
// BTR_autoHeap(int) BUFFER = malloc(sizeof(int) * 1024 * 1024);
// // Will be freed automatically
// ```
#define BTR_autoHeap(T) __attribute__((cleanup(BTR_autoHeapCleanup))) T *

#ifdef BTR_NO_PREFIX

#define autoPtr BTR_autoPtr
#define autoFile BTR_autoFile
#define autoHeap BTR_autoHeap

#endif  // BTR_NO_PREFIX
