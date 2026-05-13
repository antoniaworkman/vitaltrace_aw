/******************************************************************************
 * @file:   ble.h
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/


#ifndef BLE_H
#define BLE_H

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/settings/settings.h>

#define HR_SERVICE_UUID BT_UUID_128_ENCODE(0x89565150, 0x1c0d, 0x4a5e, 0x8467, 0x2bf671624c10)
#define HR_DATA_UUID BT_UUID_128_ENCODE(0x89565150, 0x1c0d, 0x4a5e, 0x8467, 0x2bf671624c11)
#define HR_CTRL_UUID BT_UUID_128_ENCODE(0x89565150, 0x1c0d, 0x4a5e, 0x8467, 0x2bf671624c12)

#define BT_UUID_HR_SERVICE              BT_UUID_DECLARE_128(HR_SERVICE_UUID)
#define BT_UUID_HR_DATA                 BT_UUID_DECLARE_128(HR_DATA_UUID)
#define BT_UUID_HR_CTRL                 BT_UUID_DECLARE_128(HR_CTRL_UUID)

void bt_ready(void);

#endif /* BLE_H */