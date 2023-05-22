#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <inttypes.h>
#include <pthread.h>
#include "data.h"

typedef struct Watchdog Watchdog;
Watchdog* watchdog_new(pthread_t* thread_array[], uint8_t thread_count);
void watchdog_delete(Watchdog* watchdog);
void watchdog_cancel_threads(Watchdog* watchdog);
#endif
