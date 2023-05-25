#include "data_buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
//enum{CAPACITY = 5};         //zmienic na define 
struct Data_buffer{
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

Data_buffer* data_buffer_new(const size_t elem_size, const size_t capacity){

    if(elem_size == 0)return NULL;
    if(capacity == 0)return NULL;

    Data_buffer* db = calloc(1, sizeof(Data_buffer) + (elem_size * capacity));
    if(db == NULL)return NULL;

    db->capacity = capacity;
    db->elem_size = elem_size;
    db->is_to_deletion = false;
    pthread_mutex_init(&db->mutex,NULL);
    pthread_cond_init(&db->can_produce,NULL);
    pthread_cond_init(&db->can_consume,NULL);

    return db;
}
void data_buffer_delete(Data_buffer* const db){

    if(db == NULL)return;
    pthread_cond_destroy(&db->can_consume);
    pthread_cond_destroy(&db->can_produce);
    pthread_mutex_destroy(&db->mutex);
    free(db);
}
bool buffer_is_full(const Data_buffer* const db){
    if(db == NULL)return false;
    return db->size == db->capacity;
}
bool buffer_is_empty(const Data_buffer* const db){
    if(db == NULL)return false;
    return db->size == 0;
}
bool buffer_is_to_deletion(const Data_buffer* const db){
    if(db == NULL)return NULL;
    return db->is_to_deletion;
}
void buffer_put(Data_buffer* const restrict db, const void *const restrict data){
    if(db == NULL)return;
    if(buffer_is_full(db))return;
    if(data == NULL){return;}
    memcpy(&db->buffer[db->head * db->elem_size], data, db->elem_size);
    db->head = (db->head+1)%db->capacity;
    db->size++;
}
void buffer_get(Data_buffer* const restrict db, void* const restrict ptr){
    
    if(db == NULL)return;
    if(buffer_is_empty(db)){ return; }

    
    memcpy(ptr,&db->buffer[db->tail * db->elem_size],db->elem_size);
    db->tail = (db->tail + 1) % db->capacity;
    db->size--;

}
void buffer_lock(Data_buffer* const db){
    if(db == NULL)return;
    pthread_mutex_lock(&db->mutex);
}
void buffer_unlock(Data_buffer* const db){
    if(db == NULL)return;
    pthread_mutex_unlock(&db->mutex);
}
void buffer_call_producer(Data_buffer* const db){
    if(db == NULL)return;
    pthread_cond_signal(&db->can_produce);
}
void buffer_call_consumer(Data_buffer* const db){
    if(db == NULL)return;
    pthread_cond_signal(&db->can_consume);
}
void buffer_wait_for_producer(Data_buffer* const db){
    if(db == NULL)return;
    pthread_cond_wait(&db->can_consume, &db->mutex);
}
void buffer_wait_for_consumer(Data_buffer* const db){
    if(db == NULL)return;
    pthread_cond_wait(&db->can_produce,&db->mutex);
}
int buffer_wait_for_producer_timedwait(Data_buffer* const db, const struct timespec* const ts){
    
    if(db == NULL)return -1;
    //TODO: sprawdzic timespeca

    return pthread_cond_timedwait(&db->can_consume, &db->mutex, ts);
}

void buffer_thread_producer_cleanup(void* const arg){
     
    if(arg == NULL)return;
    Data_buffer* db = (Data_buffer*) arg;
    if(db->is_to_deletion)return;
    
    buffer_lock(db);
    db->is_to_deletion = true;
    buffer_call_consumer(db);
    
    buffer_unlock(db);

}

void buffer_thread_consumer_cleanup(void* const arg){
    if(arg == NULL)return;
    Data_buffer* db = (Data_buffer*) arg;
    if(db->is_to_deletion)return;
    
    buffer_lock(db);
  
    db->is_to_deletion = true;
    buffer_call_producer(db);
    
    buffer_unlock(db);

   
}

// void watchdog_consumer_cleanup(void* arg){
//      Data_buffer* db = (Data_buffer*) arg;
    
//     thread__log_producer_put_to_buffer(db,(char*){"DDDWWWDDDWWWDDDWW"});
        
// }

void thread__log_producer_put_to_buffer(Data_buffer* db, char* data){
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

void buffer_thread_producer(Data_buffer* const restrict db,  const void *const restrict data){

     pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
          buffer_lock(db);               //SEKCJA KRYTYCZNA
             
             
             if(!buffer_is_to_deletion(db)){
                if(buffer_is_full(db)){
             //   printf("[%d]BUFFER FULL, WAITING\n",tid);
                  //  thread__log_producer_put_to_buffer(log_buffer,(char*){"READER WAITING FOR ANALYZER"});
                    buffer_wait_for_consumer(db);
                   //  printf("[%d]BUFFER END WAIT\n",tid);
               }
             //  void* ptr = malloc(sizeof(Data));
            //   printf("[%d]SPACE FOUND\n",tid);
               buffer_put(db,  (void*)&data);        
              // free(ptr);
           //   thread__log_producer_put_to_buffer(log_buffer,(char*){"READER PRODUCING"});
               buffer_call_consumer(db);
              // str = "READER PUT TO BUFFER";
             }
              
          buffer_unlock(db);
          pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
          pthread_testcancel();
}

void buffer_thread_consumer(Data_buffer* const restrict db, void** const restrict data){
    
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

void buffer_watchdog_thread_consumer(Data_buffer* const db, const char (*error_string)[], const char* const error_message, bool* const cancel_signal, const struct timespec* const ts){
            int rc = 0 ;
           buffer_lock(db);               //SEKCJA KRYTYCZNA

          if(!buffer_is_to_deletion(db) && !(*cancel_signal)){
               if(buffer_is_empty(db)){
                 //   printf("WATCHJDOG WAITING\n");
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
