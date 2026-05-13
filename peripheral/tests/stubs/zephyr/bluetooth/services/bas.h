#ifndef BT_BAS_STUB_H
#define BT_BAS_STUB_H

#include <stdint.h>

/* Single definition — declared extern, defined in bt_bas_stub.c */
extern uint8_t _bas_level;

static inline int bt_bas_set_battery_level(uint8_t level) {
    if (level > 100U) return -1;
    _bas_level = level;
    return 0;
}

static inline uint8_t bt_bas_get_battery_level(void) { return _bas_level; }

#endif /* BT_BAS_STUB_H */