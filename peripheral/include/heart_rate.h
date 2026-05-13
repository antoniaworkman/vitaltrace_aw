/******************************************************************************
 * @file:   heart_rate.h
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/

#ifndef HEART_RATE_H
#define HEART_RATE_H

#define STREAM_STACK_SIZE   1024
#define STREAM_PRIORITY     5
#define STREAM_INTERVAL_MS  100

/** Control codes written by the central */
typedef enum {
    MY_SERVICE_CTRL_STOP  = 0x00,
    MY_SERVICE_CTRL_START = 0x01,
} my_service_ctrl_t;

int hr_service_init(void);

#endif
