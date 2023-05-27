#include "test_watchdog.h"
#include "watchdog.h"
#include <assert.h>
#include <stdlib.h>

void test_watchdog_run_all(void){
    test_watchdog_new();
    test_watchdog_delete();
    test_watchdog_cancel_threads();
}
void test_watchdog_new(void){
    {
    const pthread_t** arr = calloc(3, sizeof(pthread_t*));
    Watchdog* watchdog = watchdog_new(arr,3);
    assert(watchdog!=NULL);
    }
    {
    const pthread_t** arr = NULL;
    Watchdog* watchdog = watchdog_new(arr,3);
    assert(watchdog==NULL);
    }
    {
    const pthread_t** arr = calloc(3,sizeof(pthread_t*));
    Watchdog* watchdog = watchdog_new(arr,0);
    assert(watchdog==NULL);
    free(arr);
    }
}

void test_watchdog_delete(void){
    //CHECK IF NOT CRASHING
    {
    Watchdog* watchdog = NULL;
    watchdog_delete(watchdog);
    }

    //USUAL BEHAVIOR
    {
    const pthread_t** arr = calloc(3,sizeof(pthread_t*));
    Watchdog* watchdog = watchdog_new(arr,3);
    watchdog_delete(watchdog);
    free(arr);
    }
}
static void* routine(void* arg){
    return arg;
}
void test_watchdog_cancel_threads(void){
    
    //USUAL BEHAVIOR
    {
    pthread_t t_1, t_2, t_3;
    Watchdog* watchdog = watchdog_new((const pthread_t*[]){&t_1,&t_2,&t_3},3);
    pthread_create(&t_1, NULL, routine, NULL);
    pthread_create(&t_2, NULL, routine, NULL);
    pthread_create(&t_3, NULL, routine, NULL);
    watchdog_cancel_threads(watchdog);

     pthread_join(t_1, NULL);
     pthread_join(t_2, NULL);
     pthread_join(t_3,NULL);

    }

    {
    Watchdog* watchdog = NULL;
    watchdog_cancel_threads(watchdog);
    }
}



