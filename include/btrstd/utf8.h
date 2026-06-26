#pragma once

#include "stddef.h"
#include "stdbool.h"

// Recieves the first byte of a UTF-8 character
// and returns it total length.
size_t BTR_UTF8_charLen(unsigned char);
// Tells if the given byte is not first
// of its UTF-8 character.
bool BTR_UTF8_isContinuation(unsigned char);
