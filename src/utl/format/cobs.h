#pragma once

#include "sys/core.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COBS_DELIM 0x00
#define COBS_MAX_OFFSET 0xFF
#define COBS_MAX_BLOCK_LEN (COBS_MAX_OFFSET - 1)


ssize_t COBS_Encode(uint8_t* dst, const uint8_t* src, ssize_t count);
ssize_t COBS_Decode(uint8_t* dst, const uint8_t* src, ssize_t count);

#ifdef __cplusplus
}
#endif