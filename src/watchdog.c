#include "watchdog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct Watchdog {
    uint8_t thread_count;
    pthread_t* thread_array[];
};

Watchdog* watchdog_new(const pthread_t* thread_array[const], const uint8_t thread_count){
   
    if(thread_array == NULL)return NULL;
    if(thread_count == 0)return NULL;

    Watchdog* wd = calloc(1, sizeof(Watchdog) + thread_count * sizeof(pthread_t*));
    wd->thread_count = thread_count;

    memcpy(wd->thread_array,thread_array,thread_count*sizeof(pthread_t*));
    return wd;
}
void watchdog_delete(Watchdog* const watchdog){
    if(watchdog == NULL)return;
    free(watchdog);
}

void watchdog_cancel_threads(const Watchdog* const watchdog){

   if(watchdog == NULL)return;

    for(size_t i = 0; i < watchdog->thread_count; i++){
        pthread_cancel(*(watchdog->thread_array[i]));
    }
}


