#include "unity.h"
#include "heart_rate_pb.h"
#include <pb_encode.h>
#include <pb_decode.h>

void setUp(void)    {}
void tearDown(void) {}

void test_encode_decode_roundtrip(void)
{
    Notify original = Notify_init_zero;
    original.index = 42U;
    for (int i = 0; i < 10; i++) {
        original.hr_buffer[i] = (pb_byte_t)(i * 3);
    }

    uint8_t buf[Notify_size];
    pb_ostream_t ostream = pb_ostream_from_buffer(buf, sizeof(buf));
    TEST_ASSERT_TRUE(pb_encode(&ostream, Notify_fields, &original));
    TEST_ASSERT_GREATER_THAN(0, ostream.bytes_written);

    Notify decoded = Notify_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(buf, ostream.bytes_written);
    TEST_ASSERT_TRUE(pb_decode(&istream, Notify_fields, &decoded));

    TEST_ASSERT_EQUAL_UINT32(original.index, decoded.index);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(original.hr_buffer,
                                   decoded.hr_buffer, 10);
}

void test_index_increments_across_packets(void)
{
    uint8_t buf[Notify_size];
    uint32_t last_index = 0;

    for (uint32_t i = 0; i < 5; i++) {
        Notify msg = Notify_init_zero;
        msg.index = i;

        pb_ostream_t ostream = pb_ostream_from_buffer(buf, sizeof(buf));
        TEST_ASSERT_TRUE(pb_encode(&ostream, Notify_fields, &msg));

        Notify decoded = Notify_init_zero;
        pb_istream_t istream = pb_istream_from_buffer(buf, ostream.bytes_written);
        TEST_ASSERT_TRUE(pb_decode(&istream, Notify_fields, &decoded));
        TEST_ASSERT_EQUAL_UINT32(i, decoded.index);

        if (i > 0) {
            TEST_ASSERT_EQUAL_UINT32(last_index + 1, decoded.index);
        }
        last_index = decoded.index;
    }
}

void test_worst_case_fits_in_notify_size(void)
{
    Notify msg = Notify_init_zero;
    msg.index = UINT32_MAX;
    for (int i = 0; i < 10; i++) {
        msg.hr_buffer[i] = 0xFF;
    }

    uint8_t buf[Notify_size];
    pb_ostream_t ostream = pb_ostream_from_buffer(buf, sizeof(buf));
    TEST_ASSERT_TRUE(pb_encode(&ostream, Notify_fields, &msg));
    TEST_ASSERT_LESS_OR_EQUAL(Notify_size, ostream.bytes_written);
}

void test_zero_init_encodes_cleanly(void)
{
    Notify msg = Notify_init_zero;
    uint8_t buf[Notify_size];
    pb_ostream_t ostream = pb_ostream_from_buffer(buf, sizeof(buf));
    TEST_ASSERT_TRUE(pb_encode(&ostream, Notify_fields, &msg));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_encode_decode_roundtrip);
    RUN_TEST(test_index_increments_across_packets);
    RUN_TEST(test_worst_case_fits_in_notify_size);
    RUN_TEST(test_zero_init_encodes_cleanly);
    return UNITY_END();
}
