#pragma once
#include "sys/core.h"

typedef u16 MEM_LEN_SLICE;
typedef u16 MEM_LEN_BOOK;

#pragma pack(push, 1)

typedef union
{
    uint8_t*    ptr;
    uint8_t     arr[];
} MEM_Tape;

typedef struct
{
    uint16_t    len;
    uint8_t*    ptr;
    uint8_t     arr[];
} MEM_Span;

typedef struct
{
    uint16_t    len;
    uint8_t     body[];
} MEM_String ;


typedef struct MEM
{
    uint16_t    count;
    MEM_String  slices[];
} MEM_Book;

#pragma pack(pop)

Status  MEM_Book_Copy   (const MEM_Book* dst, const MEM_Book* src, u32 len);
u32     MEM_Book_Length (const MEM_Book* src);
u8*     MEM_Book_Index  (const MEM_Book* src);