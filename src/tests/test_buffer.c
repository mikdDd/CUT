#include "test_buffer.h"
#include "buffer.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
void test_buffer_run_all(void){
    test_buffer_new();
    test_buffer_delete();
    test_buffer_put();
    test_buffer_get();
    test_buffer_is_full();
    test_buffer_is_empty();
    test_put_get_buffer();
    test_buffer_lock();
    test_buffer_unlock();
    test_buffer_call_producer();
    test_buffer_call_consumer();
    test_buffer_thread_producer_cleanup();
    test_buffer_thread_consumer_cleanup();
    test_buffer_is_to_deletion();
    test_buffer_wait_for_producer();
    test_buffer_wait_for_consumer();
    test_buffer_thread_producer();
    test_buffer_thread_consumer();
    test_buffer_watchdog_thread_consumer();
    test_producer_consumer();
    test_buffer_thread_log_producer();
}
void test_buffer_new(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),5);
    assert(buffer != NULL);
    }
    //BUFFER WITH ELEMS SIZE OF 0
    {
    Buffer* buffer = buffer_new(0,5);
    assert(buffer == NULL);
    }
    //BUFFER WITH 0 CAPACITY
    {
    Buffer* buffer = buffer_new(sizeof(int),0);
    assert(buffer == NULL);
    }

    {
    Buffer* buffer = buffer_new(0,0);
    assert(buffer == NULL);
    }
}

void test_buffer_delete(void){
    //CHECK IF NOT CRASHING
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_delete(buffer);
    }

    //USUAL BEHAVIOR
    {
    Buffer* buffer = NULL;
    buffer_delete(buffer);
    }
}
void test_buffer_put(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    }
    //PUTTING TO NULL BUFFER
    {
    Buffer* buffer = NULL;
    buffer_put(buffer,&(void*){NULL});    
    }
    //PUTTING NULL
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,NULL);    
    }
    //PUTTING TO FULL BUFFER
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    buffer_put(buffer,&(void*){NULL});
    buffer_put(buffer,&(void*){NULL});
    }
}

void test_buffer_get(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    buffer_get(buffer,&(void*){NULL});
    }
    //GETTING FROM NULL BUFFER
    {
    Buffer* buffer = NULL;
    buffer_get(buffer,&(void*){NULL});    
    }
    //GETTING TO NULL
    {
    Buffer* buffer = buffer_new(sizeof(int),1);
    buffer_put(buffer,&(void*){NULL});
    buffer_get(buffer,NULL);    
    assert(buffer_is_full(buffer)==true);
    }
    //GETTING FROM EMPTY BUFFER
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_get(buffer,&(void*){NULL});
    }
}

void test_buffer_is_full(void){
    
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    assert(buffer_is_full(buffer) == false);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;
    assert(buffer_is_full(buffer) == false);
    }
    //ONE ELEM ADDED
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_full(buffer) == false);
    }
    //TWO ELEMS ADDED
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_full(buffer) == true);
    }
}
void test_buffer_is_empty(void){
    
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    assert(buffer_is_empty(buffer) == true);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;
    assert(buffer_is_empty(buffer) == true);
    }
    //ONE ELEM ADDED
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_empty(buffer) == false);
    }
    //TWO ELEMS ADDED
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_empty(buffer) == false);
    }
}
void test_put_get_buffer(void){
    
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),2);
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_empty(buffer) == false);
    buffer_get(buffer,&(void*){NULL});
    assert(buffer_is_empty(buffer) == true);
    }
    {
    Buffer* buffer = buffer_new(sizeof(int),1);
    buffer_put(buffer,&(void*){NULL});
    assert(buffer_is_full(buffer) == true);
    buffer_get(buffer,&(void*){NULL});
    assert(buffer_is_full(buffer) == false);
    }
}
void test_buffer_lock(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_lock(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    buffer_lock(buffer);
    }
}
void test_buffer_unlock(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_unlock(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL; 
    buffer_unlock(buffer);
    }
}
void test_buffer_call_producer(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_call_producer(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    buffer_call_producer(buffer);
    }
}
void test_buffer_call_consumer(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_call_consumer(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    buffer_call_consumer(buffer);
    }
}
void test_buffer_thread_consumer_cleanup(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_thread_consumer_cleanup(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    buffer_thread_consumer_cleanup(buffer);
    }
}
void test_buffer_thread_producer_cleanup(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    buffer_thread_producer_cleanup(buffer);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    buffer_thread_producer_cleanup(buffer);
    }
}

void test_buffer_is_to_deletion(void){
    //USUAL BEHAVIOR
    {
    Buffer* buffer = buffer_new(sizeof(int),1);  
    assert(buffer_is_to_deletion(buffer) == false);
    }
    //NULL BUFFER
    {
    Buffer* buffer = NULL;  
    assert(buffer_is_to_deletion(buffer) == false);
    }

    //AFTER CLEANUP
    {
    Buffer* buffer = buffer_new(sizeof(int),1); 
    buffer_thread_producer_cleanup(buffer);
    assert(buffer_is_to_deletion(buffer) == true);
    }

}
void test_buffer_wait_for_producer(void){

    //NULL BUFFER CHECK IF NOT CRASHING
    {
    Buffer* buffer = NULL;  
    buffer_wait_for_producer(buffer);
    }
}
void test_buffer_wait_for_consumer(void){

    //NULL BUFFER CHECK IF NOT CRASHING
    {
    Buffer* buffer = NULL;  
    buffer_wait_for_consumer(buffer);
    }
}

void test_buffer_thread_producer(void){

    //USUAL BEHAVIOR CHECK IF NOT CRASHING
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    int arr[5]={0};
    void* arr_ptr = &arr;
    buffer_thread_producer(buffer,arr_ptr);
    }

    //NULL BUFFER CHECK IF NOT CRASHING
    {
    Buffer* buffer = NULL;  
    int arr[5]={0};
    void* arr_ptr = &arr;
    buffer_thread_producer(buffer,arr_ptr);
    }
    //NULL ARRAY POINTER
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    void* arr_ptr = NULL;
    buffer_thread_producer(buffer,arr_ptr);
    }
}

void test_buffer_thread_consumer(void){

    //USUAL BEHAVIOR CHECK IF NOT CRASHING
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    int arr[5]={0};
    void* arr_ptr = &arr;
    buffer_put(buffer,&(void*){NULL});
    buffer_thread_consumer(buffer,arr_ptr);
    }

    //NULL BUFFER CHECK IF NOT CRASHING
    {
    Buffer* buffer = NULL;  
    int arr[5]={0};
    void* arr_ptr = &arr;
    buffer_put(buffer,&(void*){NULL});
    buffer_thread_consumer(buffer,arr_ptr);
    }
    //NULL ARRAY POINTER
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    void* arr_ptr = NULL;
    buffer_put(buffer,&(void*){NULL});
    buffer_thread_consumer(buffer,arr_ptr);
    }
}
void test_buffer_watchdog_thread_consumer(void){
    //USUAL BEHAVIOR TEST IF NOT CRASHING
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    bool cancel_signal_sent = false;
    char str[20] = "";
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,"ERROR", &cancel_signal_sent, &ts);
    }
    //NULL BUFFER TEST
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    bool cancel_signal_sent = false;
    char str[20] = "";
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,"ERROR", &cancel_signal_sent, &ts);
    }
    //NULL ERROR STRING PTR
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    bool cancel_signal_sent = false;
    Buffer* buffer = buffer_new(sizeof(int),5); 
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,NULL,"ERROR", &cancel_signal_sent, &ts);
    }
    //NULL ERROR MESSAGE PTR
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    char str[20] = "";
    bool cancel_signal_sent = false;
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,NULL, &cancel_signal_sent, &ts);
    }
    //EMPTY ERROR MESSAGE
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    char str[20] = "";
    bool cancel_signal_sent = false;
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,"", &cancel_signal_sent, &ts);
    }
    //NULL CANCEL_SIGNAL PTR
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    char str[20] = "";
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,"", NULL, &ts);
    }
     //NULL timespec
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    char str[20] = "";
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_put(buffer,&(void*){NULL});
    buffer_watchdog_thread_consumer(buffer,&str,"", NULL, NULL);
    }
    //WAITING TEST: NO RESPONSE FOR 2 SECONDS, SO ERROR SHOULD OCCUR
    {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    ts.tv_sec += 2;
    bool cancel_signal_sent = false;
    char str[20] = "";
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_watchdog_thread_consumer(buffer,&str,"ERROR", &cancel_signal_sent, &ts);
    assert(strcmp(str,"ERROR") == 0);
    }


}
void test_producer_consumer(void){
    Buffer* buffer = buffer_new(sizeof(int),5);
    int arr[5]={0};
    void* arr_ptr = &arr;
    buffer_thread_producer(buffer,arr_ptr);
    assert(!buffer_is_empty(buffer));
    buffer_thread_consumer(buffer,arr_ptr);
    assert(buffer_is_empty(buffer));
}

void test_buffer_thread_log_producer(void){
    //USUAL BEHAVIOR CHECK IF NOT CRASHING
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_thread_log_producer(buffer, (char*){"LOG MESSAGE"});
    }

    //NULL BUFFER CHECK IF NOT CRASHING
    {
    Buffer* buffer = NULL;  
    buffer_thread_log_producer(buffer, (char*){"LOG MESSAGE"});
    }

    //EMPTY STRING
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_thread_log_producer(buffer,(char*){""});
    }
    //NULL STRING
    {
    Buffer* buffer = buffer_new(sizeof(int),5);  
    buffer_thread_log_producer(buffer,NULL);
    }
}



