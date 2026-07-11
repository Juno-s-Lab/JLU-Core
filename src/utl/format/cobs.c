#include <string.h>
#include "utl/format/cobs.h"

static ssize_t next_delim_offset(const uint8_t* src, ssize_t max_depth)
{
    ssize_t index = 0;
    for(; index < max_depth; index++)
        if (COBS_DELIM == src[index]) break;

    return index;
}

ssize_t COBS_Encode(uint8_t* dst, const uint8_t* src, ssize_t count)
{
    if (!dst || !src) return -EINVAL;
    ssize_t readHead = 0, writeHead = 0, block_len = COBS_MAX_BLOCK_LEN;

    // Easiest way to handle this edge case
    if (!count) dst[writeHead++] = 0x01;

    while(count > readHead)
    {
        if (block_len != COBS_MAX_BLOCK_LEN) readHead++;
        block_len = next_delim_offset(src + readHead, count - readHead);
        if (block_len > COBS_MAX_BLOCK_LEN) block_len = COBS_MAX_BLOCK_LEN;

        dst[writeHead++] = (block_len + 1);

        memmove(dst + writeHead, src + readHead, block_len);
        writeHead   += block_len;
        readHead    += block_len;
    }

    if (count < readHead) return -EFAULT;
    dst[writeHead++] = COBS_DELIM;
    return writeHead;
}

ssize_t COBS_Decode(uint8_t* dst, const uint8_t* src, ssize_t count)
{
    (void) dst;
    (void) src;
    (void) count;
    return 0;
}
