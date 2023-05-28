#include "watchdog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/**
 * @brief Watchdog struct containing cpu count and array of threads tid's
 * 
 */
struct Watchdog {
    uint8_t thread_count;
    pthread_t* thread_array[];
};
/**
 * @brief Function that creates a new Watchdog
 * 
 * @param thread_array array of threads to watch
 * @param thread_count number of threads
 * @return Watchdog* pointer to new Watchdog
 */
Watchdog* watchdog_new(const pthread_t* thread_array[const], const uint8_t thread_count){
   
    if(thread_array == NULL)return NULL;
    if(thread_count == 0)return NULL;

    Watchdog* wd = calloc(1, sizeof(Watchdog) + thread_count * sizeof(pthread_t*));
    wd->thread_count = thread_count;

    memcpy(wd->thread_array,thread_array,thread_count*sizeof(pthread_t*));
    return wd;
}
/**
 * @brief Function that deletes given Watchdog
 * 
 * @param watchdog to deletion
 */
void watchdog_delete(Watchdog* const watchdog){
    if(watchdog == NULL)return;
    free(watchdog);
}
/**
 * @brief Function sending cancel signal to watched threads
 * 
 * @param watchdog which threads we want to cancel
 */
void watchdog_cancel_threads(const Watchdog* const watchdog){

   if(watchdog == NULL)return;

    for(size_t i = 0; i < watchdog->thread_count; i++){
        pthread_cancel(*(watchdog->thread_array[i]));
    }
}


