#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "utl/format/cobs.h"
#include <stdio.h>

TEST_GROUP(CobsCodec)
{
   void setup()
   {
      // Init stuff
   }

   void teardown()
   {
      // Un-init stuff
   }
};

static void CobsCodecBody(const uint8_t* decoded, const ssize_t decoded_len, const uint8_t* encoded,  const ssize_t encoded_len, const uint8_t overhead)
{
    uint8_t actual[overhead + decoded_len] = {0};
    memcpy(&actual[overhead], decoded, decoded_len);

    ssize_t exp_size = encoded_len;
    ssize_t result = COBS_Encode(&actual[0], &actual[overhead], decoded_len);

    CHECK_EQUAL(exp_size, result);
    MEMCMP_EQUAL(encoded, &actual[0], encoded_len);
}

#define COBS_CODEC_CALL_BODY CobsCodecBody(&decoded[0], sizeof(decoded), &encoded[0], sizeof(encoded), overhead)
TEST(CobsCodec, empty)
{
    const uint8_t decoded[]     = {};
    const uint8_t encoded[]     = {0x01, 0x00};
    const ssize_t overhead      = 0;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, many_delim)
{
    const uint8_t decoded[]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const uint8_t encoded[]     = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, no_delim)
{
    const uint8_t decoded[]     = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    const uint8_t encoded[]     = {0x09, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_1_encode)
{
    const uint8_t decoded[]     = {0x00};
    const uint8_t encoded[]     = {0x01, 0x01, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_2_encode)
{
    const uint8_t decoded[]     = {0x00, 0x00};
    const uint8_t encoded[]     = {0x01, 0x01, 0x01, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_3_encode)
{
    const uint8_t decoded[]     = {0x00, 0x11, 0x00};
    const uint8_t encoded[]     = {0x01, 0x02, 0x11, 0x01, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_4_encode)
{
    const uint8_t decoded[]     = {0x11, 0x22, 0x00, 0x33};
    const uint8_t encoded[]     = {0x03, 0x11, 0x22, 0x02, 0x33, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_5_encode)
{
    const uint8_t decoded[]     = {0x11, 0x22, 0x33, 0x44};
    const uint8_t encoded[]     = {0x05, 0x11, 0x22, 0x33, 0x44, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_6_encode)
{
    const uint8_t decoded[]     = {0x11, 0x00, 0x00, 0x00};
    const uint8_t encoded[]     = {0x02, 0x11, 0x01, 0x01, 0x01, 0x00};
    const ssize_t overhead      = 1;

    COBS_CODEC_CALL_BODY;
}

static_assert(COBS_MAX_OFFSET == 0xFF, "Unexpected maximum COBS block length");
TEST(CobsCodec, wiki_ex_7_encode)
{
    uint8_t decoded[0x0FE];
    uint8_t encoded[0x100];
    const ssize_t overhead = 1;

    encoded[0x00]   = 0xFF;

    for(uint16_t i = 0; i < sizeof(decoded); i++)
        encoded[i+1] = decoded[i] = (i + 1) & 0xFF;

    encoded[sizeof(encoded) - 1] = COBS_DELIM;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_8_encode)
{
    uint8_t decoded[0x0FF];
    uint8_t encoded[0x101];
    const uint8_t overhead = 2;

    for(uint16_t i = 0; i < 0xFF; i++)
        encoded[i+1] = decoded[i] = i;

    encoded[0x000]  = 0x01;
    encoded[0x001]  = 0xFF;
    encoded[0x100]  = COBS_DELIM;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_9_encode)
{
    uint8_t decoded[0x0FF];
    uint8_t encoded[0x102];
    const uint8_t overhead = 3;

    for(uint16_t i = 0; i < sizeof(decoded); i++)
        encoded[i+1] = decoded[i] = (i + 1) & 0xFF;

    encoded[0x100] = decoded[0xFE] = 0xFF;

    encoded[0x000] = 0xFF;
    encoded[0x0FF] = 0x02;
    encoded[0x101] = COBS_DELIM;
    // encoded[0x001] = 01;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_10_encode)
{
    uint8_t decoded[0x0FF];
    uint8_t encoded[0x102];
    const uint8_t overhead = 3;

    for(uint16_t i = 0; i < sizeof(decoded); i++)
        encoded[i+1] = decoded[i] = (i + 2) & 0xFF;

    encoded[0x000] = 0xFF;
    encoded[0x0FF] = 0x01;
    encoded[0x100] = 0x01;
    encoded[0x101] = COBS_DELIM;
    // encoded[0x001] = 01;

    COBS_CODEC_CALL_BODY;
}

TEST(CobsCodec, wiki_ex_10_encode_regress)
{
    uint8_t decoded[0x100];
    uint8_t encoded[0x102];
    const uint8_t overhead = 3;

    for(uint16_t i = 0; i < sizeof(decoded); i++)
        encoded[i+1] = decoded[i] = (i + 2) & 0xFF;
    decoded[0x0FF] = 0x01;

    encoded[0x000] = 0xFF;
    encoded[0x0FF] = 0x01;
    encoded[0x100] = 0x01;
    encoded[0x101] = COBS_DELIM;
    // encoded[0x001] = 01;

    CobsCodecBody(&decoded[0], sizeof(decoded) - 1, &encoded[0], sizeof(encoded), overhead);
}

TEST(CobsCodec, wiki_ex_11_encode)
{
    uint8_t decoded[0x0FF];
    uint8_t encoded[0x101];
    const uint8_t overhead = 3;

    for(uint16_t i = 0; i < sizeof(decoded); i++)
        encoded[i+1] = decoded[i] = (i + 3) & 0xFF;

    encoded[0x100] = decoded[0xFE] = 0x01;

    encoded[0x000] = 0xFE;
    encoded[0x0FE] = 0x02;
    encoded[0x100] = COBS_DELIM;

    COBS_CODEC_CALL_BODY;
}


TEST_GROUP(CobsCodecInvalid)
{
   void setup()
   {
      // Init stuff
   }

   void teardown()
   {
      // Un-init stuff
   }
};