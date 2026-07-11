#include "mem/MEM.h"

static Status MEM_Book_Intersection (MEM_Book* dst, const MEM_Book* book_a, const MEM_Book* book_b)
{
    if (!dst || !book_a || !book_b) return EINVAL;

    u32 len_a = MEM_Book_Length(book_a);
    u32 len_b = MEM_Book_Length(book_b);
    u32 len = len_a < len_b ? len_a : len_b;

    u32 iter = 0;
}

Status  MEM_Book_Copy   (const MEM_Book* dst, const MEM_Book* src, u32 len)
{
    if (!dst || !src) return EINVAL;
    if (!len) return EOK;
    
    u32 dst_len = MEM_Book_Length(dst);
    u32 src_len = MEM_Book_Length(src);
    if (dst_len < len || src_len < len) return EINVAL;


}

uint32_t    MEM_Book_Length (const MEM_Book* src)
{
    u32 total = 0;
    for (u16 i = 0; i < src->count; i++) total += src->slices[i].len;
    return total;
}

uint8_t*    MEM_Book_Index  (const MEM_Book* src)
{

}
