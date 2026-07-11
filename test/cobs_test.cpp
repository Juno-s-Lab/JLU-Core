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

TEST(CobsCodec, empty)
{
    uint8_t actual[]    = {0x00, 0x00};
    uint8_t expected[]  = {0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 0));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, many_delim)
{
    uint8_t actual[]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t expected[]  = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 8));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, no_delim)
{
    uint8_t actual[]    = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    uint8_t expected[]  = {0x09, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 8));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_1_encode)
{
    uint8_t actual[]    = {0x00, 0x00, 0x00};
    uint8_t expected[]  = {0x01, 0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 1));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_2_encode)
{
    uint8_t actual[]    = {0x00, 0x00, 0x00, 0x00};
    uint8_t expected[]  = {0x01, 0x01, 0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 2));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_3_encode)
{
    uint8_t actual[]    = {0x00, 0x00, 0x11, 0x00, 0x00};
    uint8_t expected[]  = {0x01, 0x02, 0x11, 0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 3));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_4_encode)
{
    uint8_t actual[]    = {0x00, 0x11, 0x22, 0x00, 0x33, 0x00};
    uint8_t expected[]  = {0x03, 0x11, 0x22, 0x02, 0x33, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 4));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_5_encode)
{
    uint8_t actual[]    = {0x00, 0x11, 0x22, 0x33, 0x44, 0x00};
    uint8_t expected[]  = {0x05, 0x11, 0x22, 0x33, 0x44, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 4));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_6_encode)
{
    uint8_t actual[]    = {0x00, 0x11, 0x00, 0x00, 0x00, 0x00};
    uint8_t expected[]  = {0x02, 0x11, 0x01, 0x01, 0x01, 0x00};

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], 4));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

static_assert(COBS_MAX_OFFSET == 0xFF, "Unexpected maximum COBS block length");
TEST(CobsCodec, wiki_ex_7_encode)
{
    uint8_t actual[0x100];
    uint8_t expected[sizeof(actual)];

    actual[0x00]    = 0x00;
    expected[0x00]  = 0xFF;

    for(uint16_t i = 1; i <= 0xFE; i++)
        expected[i] = actual[i] = i;

    expected[sizeof(expected) - 1] = COBS_DELIM;

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[1], COBS_MAX_BLOCK_LEN));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_8_encode)
{
    const uint8_t overhead = 2;
    uint8_t actual[257];
    uint8_t expected[sizeof(actual)];


    for(uint16_t i = 0; i < 0xFF; i++)
        expected[i+1] = actual[i + overhead] = i;

    expected[0x000]  = 0x01;
    expected[0x001]  = 0xFF;
    expected[0x100] = COBS_DELIM;

    ssize_t exp_size = sizeof(expected);
    ssize_t result = COBS_Encode(&actual[0], &actual[overhead], sizeof(actual) - overhead);

    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
    CHECK_EQUAL(exp_size, result);
}

TEST(CobsCodec, wiki_ex_9_encode)
{
    const uint8_t overhead = 3;
    uint8_t actual[258];
    uint8_t expected[sizeof(actual)];

    actual[0x00]    = 0x00;
    expected[0x00]  = 0xFF;

    for(uint16_t i = 0; i < 0xFE; i++)
        expected[i+1] = actual[i + overhead] = i + 1;

    expected[0xFF]  = 0x02;
    expected[0x100] = actual[overhead + 0xFE] = 0xFF;
    expected[0x101] = COBS_DELIM;

    CHECK_EQUAL(sizeof(expected), COBS_Encode(&actual[0], &actual[overhead], sizeof(actual) - overhead));
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_10_encode)
{
    const uint8_t overhead = 3;
    uint8_t actual[0x102];
    uint8_t expected[sizeof(actual)];
    memset(actual, 0, sizeof(actual));

    actual[0x000]   = 0x00;
    expected[0x000] = 0xFF;

    for(uint16_t i = 0; i < 0xFF; i++)
        expected[i+1] = actual[i + overhead] = (i + 2) & 0xFF;

    actual[0xFF + overhead] = 0x00;
    expected[0x0FF] = 0x01;
    expected[0x100] = 0x01;
    expected[0x101] = 0x00;

    ssize_t exp_size = sizeof(expected);
    ssize_t result = COBS_Encode(&actual[0], &actual[overhead], 0xFF);

    CHECK_EQUAL(exp_size, result);
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_10_encode_regress)
{
    const uint8_t overhead = 3;
    uint8_t actual[0x102];
    uint8_t expected[sizeof(actual)];
    memset(actual, 0, sizeof(actual));

    actual[0x000]   = 0x00;
    expected[0x000] = 0xFF;

    for(uint16_t i = 0; i < 0xFF; i++)
        expected[i+1] = actual[i + overhead] = (i + 2) & 0xFF;

    actual[0xFF + overhead] = 0x01;
    expected[0x0FF] = 0x01;
    expected[0x100] = 0x01;
    expected[0x101] = 0x00;

    ssize_t exp_size = sizeof(expected);
    ssize_t result = COBS_Encode(&actual[0], &actual[overhead], 0xFF);

    CHECK_EQUAL(exp_size, result);
    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
}

TEST(CobsCodec, wiki_ex_11_encode)
{
    const uint8_t overhead = 2;
    uint8_t actual[257];
    uint8_t expected[sizeof(actual)];

    actual[0x00]    = 0x00;
    expected[0x00]  = 0xFE;

    for(uint16_t i = 0; i < 0xFF; i++)
        expected[i+1] = actual[i + overhead] = (i + 3) & 0xFF;

    expected[0x0FE] = 0x02;
    expected[0x0FF] = 0x01;
    expected[0x100] = COBS_DELIM;

    ssize_t exp_size = sizeof(expected);
    ssize_t result = COBS_Encode(&actual[0], &actual[overhead], sizeof(actual) - overhead);

    MEMCMP_EQUAL(&expected[0], &actual[0], sizeof(expected));
    CHECK_EQUAL(exp_size, result);
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