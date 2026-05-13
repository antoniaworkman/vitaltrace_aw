#ifndef KERNEL_STUB_H
#define KERNEL_STUB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define K_SECONDS(x)  (x)
#define K_MSEC(x)     (x)

/* Work queue stubs */
struct k_work {
    void (*handler)(struct k_work *work);
};

struct k_work_delayable {
    struct k_work work;
};

typedef void (*k_work_handler_t)(struct k_work *work);

#define K_WORK_DELAYABLE_DEFINE(name, handler_fn) \
    struct k_work_delayable name = { .work = { .handler = handler_fn } }

static inline void k_work_schedule(struct k_work_delayable *w, int t)
{
    (void)w;
    (void)t;
}

#endif /* KERNEL_STUB_H */