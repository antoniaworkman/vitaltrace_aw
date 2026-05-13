/******************************************************************************
 * @file:   battery.h
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/


#ifndef BATTERY_H
#define BATTERY_H

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/services/bas.h>

#define BATTERY_UPDATE_INTERVAL_SEC   5u

void battery_init(void);

#endif /* BATTERY_H */
