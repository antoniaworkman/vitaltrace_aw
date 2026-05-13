/******************************************************************************
 * @file:   ble.c
 * @brief:  File for defining BLE services and advertising, connection and disconnection functionality.
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/


/* --- Includes ----------------------------------------------------------- */
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/bluetooth/services/bas.h>
#include "ble.h"
#include "battery.h"
#include "heart_rate.h"

LOG_MODULE_REGISTER(ble, LOG_LEVEL_INF);

/* --- Private Function Prototypes ---------------------------------------- */
static void connected(struct bt_conn *conn, uint8_t err);
static void disconnected(struct bt_conn *conn, uint8_t reason);


/* --- Private Types ------------------------------------------------------ */
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

/* Scan response with device name */
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* Standard Battery Service, in advertising data */
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA_BYTES(BT_DATA_UUID16_ALL,
                  BT_UUID_16_ENCODE(BT_UUID_BAS_VAL)), /* 0x180F */
};


/* --- Public Function Definitions ---------------------------------------- */
/** 
* @brief  Function to check the proper functioning of the bluetooth module and initialise 
*         bluetooth-dependent modules
*/
void bt_ready(void)
{
	int err;

	LOG_INF("Bluetooth initialized\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_INF("Advertising failed to start (err %d)\n", err);
		return;
	}

	LOG_INF("Advertising successfully started\n");

	/* Start the heart rate service */
    hr_service_init();

    /* Initiate the standard battery service */
	battery_init();
}


/* --- Callbacks ---------------------------------------- */
static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("Connection failed (err %u)", err);
        return;
    }
    LOG_INF("Connected");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected");

    int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad),
                               sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Advertising failed to restart (err %d)", err);
    }
}
