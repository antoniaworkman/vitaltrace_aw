/******************************************************************************
 * @file:   heart_rate.c
 * @brief:  File for defining the heart rate BLE callbacks and module functionality.
 * @author: Antonia Workman
 * @date:   2026-05-12
 *****************************************************************************/


/* --- Includes ----------------------------------------------------------- */
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/gatt.h>
#include <pb_encode.h>
#include "heart_rate.h"
#include "heart_rate_pb.h"
#include "ble.h"

LOG_MODULE_REGISTER(heart_rate, LOG_LEVEL_INF);

/* --- Private Function Prototypes ---------------------------------------- */
static void stream_thread_fn(void *a, void *b, void *c);
static int hr_service_notify(Notify *msg);
static void hr_data_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value);
static int hr_ctrl_read_cb(struct bt_conn *conn,
                             const struct bt_gatt_attr *attr,
                             void *buf, uint16_t len, uint16_t offset);
static int hr_ctrl_write_cb(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr,
                              const void *buf, uint16_t len,
                              uint16_t offset, uint8_t flags);



/* --- Private Defines/Macros --------------------------------------------- */
#define STREAM_INTERVAL_MS      100U

K_THREAD_DEFINE(stream_thread,
                STREAM_STACK_SIZE,
                stream_thread_fn,
                NULL, NULL, NULL,
                STREAM_PRIORITY,
                0,
                0); 

/** Custom Heart Rate Service */
BT_GATT_SERVICE_DEFINE(hr_service,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HR_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_HR_DATA,
                           BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_NONE,
                           NULL, NULL, NULL),
    BT_GATT_CCC(hr_data_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_HR_CTRL,
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
                           hr_ctrl_read_cb, hr_ctrl_write_cb, NULL),
);

/* --- Private Variables -------------------------------------------------- */
static bool streaming_ctrl = false;
static bool notify_enabled = false;
static uint32_t index = 0;
static uint8_t encode_buf[Notify_size];


/* --- Public Function Definitions ---------------------------------------- */
/** 
* @brief Initialises the heart rate module by setting all control values to 0.
*        To be called from BLE module after bluetooth initialisation
* 
* @retval 0 for success
*/
int hr_service_init(void)
{
    streaming_ctrl = false;
    notify_enabled = false;
    LOG_INF("Heart rate service initialised");
    return 0;
}

/** 
* @brief Read if streaming is turned on/off from outside the heart rate module
* 
* @retval 0 streaming disabled, 1 streaming enabled
*/
bool is_hr_streaming(void)
{
    return streaming_ctrl && notify_enabled;
}

/** 
* @brief Sets the streaming control value to turn heart rate streaming on/off
* 
* @param ctrl_val Boolean value to set streaming to on (value = 1) or off (value = 0)
*/
void hr_ctrl_set(bool ctrl_val)
{
    streaming_ctrl = ctrl_val;
}


/* --- Private Function Definitions --------------------------------------- */
static void stream_thread_fn(void *a, void *b, void *c)
{
    while (1) {
        if (is_hr_streaming() == true) {
            Notify msg = Notify_init_zero;
            msg.index = index++;

            /** Fill buffer with fake test data for now */
            for (int i = 0; i < 10; i++) {
                msg.hr_buffer[i] = i;
            }

            hr_service_notify(&msg);
        }
        k_msleep(STREAM_INTERVAL_MS);
    }
}

static int hr_service_notify(Notify *msg)
{
    if (notify_enabled != true) {
        return -EACCES;  /** Permission denied, central notifications are off */
    }
    if (streaming_ctrl != true) {
        return -EBUSY;   /**  streaming not started by central */
    }

    pb_ostream_t stream = pb_ostream_from_buffer(encode_buf, sizeof(encode_buf));

    if (!pb_encode(&stream, Notify_fields, msg)) {
        LOG_ERR("pb_encode failed: %s", stream.errmsg);
        return -EINVAL;
    }

    int err = bt_gatt_notify(NULL, &hr_service.attrs[1],
                             encode_buf, stream.bytes_written);
    if (err) {
        LOG_ERR("Notify failed (err %d)", err);
    }

    return err;
}

/* --- Callbacks -------------------------------------------------- */
/** Callback for read request from central device of streaming status */
static int hr_ctrl_read_cb(struct bt_conn *conn,
                             const struct bt_gatt_attr *attr,
                             void *buf, uint16_t len, uint16_t offset)
{
    LOG_INF("Data characteristic read: %d", streaming_ctrl);
    return bt_gatt_attr_read(conn, attr, buf, len, offset, &streaming_ctrl, sizeof(streaming_ctrl));
}

/** Callback for write from central device */
static int hr_ctrl_write_cb(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr,
                              const void *buf, uint16_t len,
                              uint16_t offset, uint8_t flags)
{
    if (len != 1U) {
        LOG_WRN("Control write: unexpected length %u", len);
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
    }

    uint8_t opcode = *((uint8_t *)buf);

    switch (opcode) {
    case MY_SERVICE_CTRL_START:
        streaming_ctrl = true;
        LOG_INF("Streaming started");
        break;
    case MY_SERVICE_CTRL_STOP:
        streaming_ctrl = false;
        LOG_INF("Streaming stopped");
        break;
    default:
        LOG_WRN("Unknown control command: 0x%02X", opcode);
        return BT_GATT_ERR(BT_ATT_ERR_NOT_SUPPORTED);
    }

    return len;
}

/** Indicates when notifications are switched on/off by the central device 
* This alone does not turn streaming on/off since that is handled by the HR_CTRL characteristic */
static void hr_data_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Data notifications %s", notify_enabled ? "enabled" : "disabled");
}
