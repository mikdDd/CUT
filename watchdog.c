#include "watchdog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
struct Watchdog {
    uint8_t thread_count;
    pthread_t* thread_array[];
};

Watchdog* watchdog_new(pthread_t* thread_array[], uint8_t thread_count){
    pthread_t* t;
    for(size_t i = 0; i<thread_count; i++){
        t = thread_array[i];
    }
    if(thread_array == NULL)return NULL;
    if(thread_count == 0)return NULL;

    Watchdog* wd = calloc(1, sizeof(Watchdog) + thread_count * sizeof(pthread_t*));
    wd->thread_count = thread_count;

    memcpy(wd->thread_array,thread_array,thread_count*sizeof(pthread_t*));
    return wd;
}
void watchdog_delete(Watchdog* watchdog){
    free(watchdog);
}

void watchdog_cancel_threads(Watchdog* watchdog){

    size_t debug1 = watchdog->thread_count;

    if(watchdog==NULL)return;
    for(size_t i = 0; i < watchdog->thread_count; i++){
        //pid_t t = (pid_t) *(watchdog->thread_array[i]);
        pthread_cancel(*(watchdog->thread_array[i]));
        printf("THREAD [%ld] CANCELED\n",*(watchdog->thread_array[i]));
    }
}