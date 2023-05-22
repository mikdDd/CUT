#include "data_buffer.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//enum{CAPACITY = 5};         //zmienic na define 
struct Data_buffer{
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

    Data_buffer* db = calloc(1, sizeof(Data_buffer) + (elem_size * capacity));
    if(db == NULL)return NULL;

    db->capacity = capacity;
    db->elem_size = elem_size;
    
    pthread_mutex_init(&db->mutex,NULL);
    pthread_cond_init(&db->can_produce,NULL);
    pthread_cond_init(&db->can_consume,NULL);

    return db;
}
void data_buffer_delete(Data_buffer* db){

    if(db == NULL)return;
    pthread_cond_destroy(&db->can_consume);
    pthread_cond_destroy(&db->can_produce);
    pthread_mutex_destroy(&db->mutex);
    free(db);
}
bool buffer_is_full(const Data_buffer* const db){
    return db->size == db->capacity;
}
bool buffer_is_empty(const Data_buffer* const db){
    return db->size == 0;
}
void buffer_put(Data_buffer* restrict db, const void* restrict data){
    if(buffer_is_full(db))return;

    memcpy(&db->buffer[db->head * db->elem_size], data, db->elem_size);
    db->head = (db->head+1)%db->capacity;
    db->size++;
}
void buffer_get(Data_buffer* db, void* restrict ptr){
    if(buffer_is_empty(db))return;
    if(ptr == NULL)return;
    //Data* d = db->buffer[db->tail];
    
    memcpy(ptr,&db->buffer[db->tail * db->elem_size],db->elem_size);
    db->tail = (db->tail + 1) % db->capacity;
    db->size--;

}
void buffer_lock(Data_buffer* db){
    pthread_mutex_lock(&db->mutex);
}
void buffer_unlock(Data_buffer* db){
    pthread_mutex_unlock(&db->mutex);
}
void buffer_call_producer(Data_buffer* db){
    pthread_cond_signal(&db->can_produce);
}
void buffer_call_consumer(Data_buffer* db){
    pthread_cond_signal(&db->can_consume);
}
void buffer_wait_for_producer(Data_buffer* db){
    pthread_cond_wait(&db->can_consume, &db->mutex);
}
void buffer_wait_for_consumer(Data_buffer* db){
    pthread_cond_wait(&db->can_produce,&db->mutex);
}
int buffer_wait_for_producer_timedwait(Data_buffer* db, struct timespec* ts){
    
    if(db == NULL)return -1;
    //TODO: sprawdzic timespeca

    return pthread_cond_timedwait(&db->can_consume, &db->mutex, ts);
}

void thread__producer_cleanup(void* arg){
     //printf("CLEANUP\n");
     Data_buffer* db = (Data_buffer*) arg;
   // if(buffer_is_empty(db)){
      //  void* dummy_ptr = malloc(db->elem_size);
      //  buffer_put(db,dummy_ptr);
    //}
    
    buffer_call_consumer(db);
}

void thread__consumer_cleanup(void* arg){
     Data_buffer* db = (Data_buffer*) arg;

  //  if(buffer_is_full(db)){
    //    void* dummy_ptr = NULL;
      //  buffer_get(db,dummy_ptr);
   // }
     buffer_call_producer(db);
}