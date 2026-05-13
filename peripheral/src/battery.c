/******************************************************************************
 * @file:   battery.c
 * @brief:  Defines battery module with simulated discharge functionality.
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/


/* --- Includes ----------------------------------------------------------- */
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "battery.h"

LOG_MODULE_REGISTER(battery, LOG_LEVEL_INF);


/* --- Private Function Prototypes ---------------------------------------- */
static void bas_notify_handler(struct k_work *work);


/* --- Private Defines/Macros --------------------------------------------- */
static K_WORK_DELAYABLE_DEFINE(battery_work, bas_notify_handler);


/* --- Public Function Definitions ---------------------------------------- */
/** 
* @brief  Initialse battery level and start work scheduler for BLE updates and battery discharge
*/
void battery_init(void)
{
    bt_bas_set_battery_level(100U);
    k_work_schedule(&battery_work, K_SECONDS(BATTERY_UPDATE_INTERVAL_SEC));
}


/* --- Private Function Definitions --------------------------------------- */
/** 
* A function to simulate battery discharge, losing 1% every 5s 
* and to send battery level notifications to the central device
*/ 
static void bas_notify_handler(struct k_work *work)
{
	uint8_t battery_level = bt_bas_get_battery_level();

	if (battery_level == 0U) {
        battery_level = 100U;
    } else {
        battery_level--;
    }

	bt_bas_set_battery_level(battery_level);

    k_work_schedule(&battery_work, K_SECONDS(BATTERY_UPDATE_INTERVAL_SEC));
}
