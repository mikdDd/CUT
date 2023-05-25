#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <inttypes.h>
#include <pthread.h>
#include "data.h"

typedef struct Watchdog Watchdog;
Watchdog* watchdog_new(const pthread_t* thread_array[const], const uint8_t thread_count);
void watchdog_delete(Watchdog* const watchdog);
void watchdog_cancel_threads(const Watchdog* const watchdog);
#endif
