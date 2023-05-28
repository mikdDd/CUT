#include "buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

/**
 * @brief Buffer struct 
 */
struct Buffer{
    bool is_to_deletion;
    size_t capacity;
    size_t elem_size;
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    
    uint8_t buffer[];   //FAM
};
/**
 * @brief Function that creates a new Buffer
 * 
 * @param elem_size size of element in buffer
 * @param capacity number of elements to store
 * @return Buffer* pointer to new Buffer
 */
Buffer* buffer_new(const size_t elem_size, const size_t capacity){

    if(elem_size == 0)return NULL;
    if(capacity == 0)return NULL;

    Buffer* db = calloc(1, sizeof(Buffer) + (elem_size * capacity));
    if(db == NULL)return NULL;

    db->capacity = capacity;
    db->elem_size = elem_size;
    db->is_to_deletion = false;
    pthread_mutex_init(&db->mutex,NULL);
    pthread_cond_init(&db->can_produce,NULL);
    pthread_cond_init(&db->can_consume,NULL);

    return db;
}
/**
 * @brief Function that deletes given Buffer
 * 
 * @param db to deletion
 */
void buffer_delete(Buffer* const db){

    if(db == NULL)return;
    pthread_cond_destroy(&db->can_consume);
    pthread_cond_destroy(&db->can_produce);
    pthread_mutex_destroy(&db->mutex);
    free(db);
}
/**
 * @brief Function to check if buffer is full
 * 
 * @param db buffer to check
 * @return true if buffer is full
 * @return false if buffer is not full
 */
bool buffer_is_full(const Buffer* const db){
    if(db == NULL)return false;
    return db->size == db->capacity;
}
/**
 * @brief Function to check if buffer is empty
 * 
 * @param db buffer to check
 * @return true if buffer is empty
 * @return false if buffer is not empty 
 */
bool buffer_is_empty(const Buffer* const db){
    if(db == NULL)return true;
    return db->size == 0;
}
/**
 * @brief Function to check if buffer is meant to be deleted
 * after cancelation
 * 
 * @param db buffer to check
 * @return true if is_to_deletion property is true
 * @return false otherwise
 */
bool buffer_is_to_deletion(const Buffer* const db){
    if(db == NULL)return false;
    return db->is_to_deletion;
}
/**
 * @brief Function that puts data inside buffer
 * 
 * @param db buffer to put data in
 * @param data data to put
 */
void buffer_put(Buffer* const restrict db, const void *const restrict data){
    if(db == NULL)return;
    if(buffer_is_full(db))return;
    if(data == NULL){return;}
    memcpy(&db->buffer[db->head * db->elem_size], data, db->elem_size);
    db->head = (db->head+1)%db->capacity;
    db->size++;
}
/**
 * @brief Function that gets data from buffer
 * 
 * @param db buffer to get data from
 * @param ptr pointer to assign data
 */
void buffer_get(Buffer* const restrict db, void* const restrict ptr){
    
    if(db == NULL)return;
    if(buffer_is_empty(db)){ return; }
    if(ptr == NULL)return;
    
    memcpy(ptr,&db->buffer[db->tail * db->elem_size],db->elem_size);
    db->tail = (db->tail + 1) % db->capacity;
    db->size--;

}
/**
 * @brief Function that locks buffer's mutex
 * 
 * @param db buffer
 */
void buffer_lock(Buffer* const db){
    if(db == NULL)return;
    pthread_mutex_lock(&db->mutex);
}
/**
 * @brief Function that unlocks buffer's mutex
 * 
 * @param db buffer
 */
void buffer_unlock(Buffer* const db){
    if(db == NULL)return;
    pthread_mutex_unlock(&db->mutex);
}
/**
 * @brief Function that signals producer to start producing
 * 
 * @param db buffer
 */
void buffer_call_producer(Buffer* const db){
    if(db == NULL)return;
    pthread_cond_signal(&db->can_produce);
}
/**
 * @brief Function that signals consumer to start consuming
 * 
 * @param db buffer
 */
void buffer_call_consumer(Buffer* const db){
    if(db == NULL)return;
    pthread_cond_signal(&db->can_consume);
}
/**
 * @brief Function that makes consumer wait for producer's signal
 * 
 * @param db buffer
 */
void buffer_wait_for_producer(Buffer* const db){
    if(db == NULL)return;
    pthread_cond_wait(&db->can_consume, &db->mutex);
}
/**
 * @brief Function that makes producer wait for consumer's signal
 * 
 * @param db buffer
 */
void buffer_wait_for_consumer(Buffer* const db){
    if(db == NULL)return;
    pthread_cond_wait(&db->can_produce,&db->mutex);
}
/**
 * @brief Function that makes consumer wait for producer's signal for given amount of time
 * 
 * @param db buffer
 * @param ts timespec pointer
 * @return int return code
 */
int buffer_wait_for_producer_timedwait(Buffer* const db, const struct timespec* const ts){
    
    if(db == NULL)return -1;

    return pthread_cond_timedwait(&db->can_consume, &db->mutex, ts);
}

/**
 * @brief Function that makes cleanup after producer cancelation
 * 
 * @param arg buffer
 */
void buffer_thread_producer_cleanup(void* const arg){
     
    if(arg == NULL)return;
    Buffer* db = (Buffer*) arg;
    if(db->is_to_deletion)return;
    
    buffer_lock(db);
    db->is_to_deletion = true;
    buffer_call_consumer(db);
    
    buffer_unlock(db);

}
/**
 * @brief Function that makes cleanup after consumer cancelation
 * 
 * @param arg buffer
 */
void buffer_thread_consumer_cleanup(void* const arg){
    if(arg == NULL)return;
    Buffer* db = (Buffer*) arg;
    if(db->is_to_deletion)return;
    
    buffer_lock(db);
  
    db->is_to_deletion = true;
    buffer_call_producer(db);
    
    buffer_unlock(db);

   
}
/**
 * @brief Function that produces logs
 * 
 * @param db buffer
 * @param data message to log
 */
void buffer_thread_log_producer(Buffer* db, char* data){
    if(db == NULL)return;
    if(data == NULL)return;
    if(strcmp(data,"")==0)return;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(db);               //SEKCJA KRYTYCZNA
               if(buffer_is_full(db)){
                    buffer_wait_for_consumer(db);
               }
               
               buffer_put(db,&data);        
            
               buffer_call_consumer(db);
          
          buffer_unlock(db);
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
}
/**
 * @brief Function that produces data
 * 
 * @param db buffer to put data into
 * @param data pointer to data
 */
void buffer_thread_producer(Buffer* const restrict db,  const void **const restrict data){

    if(db == NULL)return;
    if(data == NULL)return;
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    buffer_lock(db);               
        if(!buffer_is_to_deletion(db)){
            if(buffer_is_full(db)){
                buffer_wait_for_consumer(db);
            }
           
            buffer_put(db, &(*data));        
            buffer_call_consumer(db);
        }
              
    buffer_unlock(db);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
}
/**
 * @brief Function that consumes data
 * 
 * @param db buffer to get data from
 * @param data pointer to data
 */
void buffer_thread_consumer(Buffer* const restrict db, void** const restrict data){
    if(db == NULL)return;
    if(data == NULL)return;
    
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    buffer_lock(db);            
      
        if(!buffer_is_to_deletion(db)){
            if(buffer_is_empty(db)){
                buffer_wait_for_producer(db);
            }
            buffer_get(db,&(*data));
            buffer_call_producer(db);
        }
    buffer_unlock(db);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
}
/**
 * @brief Watchdog thread function that consumes data
 * 
 * @param db buffer
 * @param error_string pointer to string to put data 
 * @param error_message error message
 * @param cancel_signal cancel signal pointer
 * @param ts pointer to timespec until which to wait for watched threads
 */
void buffer_watchdog_thread_consumer(Buffer* const db, char (*error_string)[], const char* const error_message, bool* const cancel_signal, const struct timespec* const ts){
        if(db == NULL)return;
        if(error_string == NULL)return;
        if(error_message == NULL)return;
        if(strcmp(error_message,"")==0)return;
        if(cancel_signal == NULL)return;
        if(ts == NULL)return;
        int rc = 0 ;
        buffer_lock(db);              
            if(!buffer_is_to_deletion(db) && !(*cancel_signal)){
                if(buffer_is_empty(db)){
                    rc = buffer_wait_for_producer_timedwait(db,ts);
                }
                
                if(rc == ETIMEDOUT){
                    *(cancel_signal) = true;
                    strcpy((*error_string),error_message);
                } else {
                    void* dummy_ptr;
                    buffer_get(db,&dummy_ptr);
                    buffer_call_producer(db);
                }
            }
        buffer_unlock(db);
}
