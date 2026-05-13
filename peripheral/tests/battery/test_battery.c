#include "unity.h"
#include "battery.h"

void setUp(void)
{
    bt_bas_set_battery_level(100U);
}

void tearDown(void) {}

void test_initial_level_is_100(void)
{
    battery_init();
    TEST_ASSERT_EQUAL_UINT8(100U, bt_bas_get_battery_level());
}

void test_level_decrements(void)
{
    bt_bas_set_battery_level(50U);

    uint8_t level = bt_bas_get_battery_level();
    if (level == 0U) { level = 100U; } else { level--; }
    bt_bas_set_battery_level(level);

    TEST_ASSERT_EQUAL_UINT8(49U, bt_bas_get_battery_level());
}

void test_level_wraps_at_zero(void)
{
    bt_bas_set_battery_level(1U);

    /* First decrement → 0 */
    uint8_t level = bt_bas_get_battery_level();
    if (level == 0U) { level = 100U; } else { level--; }
    bt_bas_set_battery_level(level);
    TEST_ASSERT_EQUAL_UINT8(0U, bt_bas_get_battery_level());

    /* Second decrement → wraps to 100 */
    level = bt_bas_get_battery_level();
    if (level == 0U) { level = 100U; } else { level--; }
    bt_bas_set_battery_level(level);
    TEST_ASSERT_EQUAL_UINT8(100U, bt_bas_get_battery_level());
}

void test_bas_rejects_level_above_100(void)
{
    int err = bt_bas_set_battery_level(101U);
    TEST_ASSERT_NOT_EQUAL(0, err);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initial_level_is_100);
    RUN_TEST(test_level_decrements);
    RUN_TEST(test_level_wraps_at_zero);
    RUN_TEST(test_bas_rejects_level_above_100);
    return UNITY_END();
}
