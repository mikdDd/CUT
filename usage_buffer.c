// #include "usage_buffer.h"
// #include <stdlib.h>
// #include <pthread.h>
// enum{CAPACITY = 5}; 
// struct Usage_buffer{
//     float* buffer[CAPACITY];
//     size_t size;
//     size_t head;
//     size_t tail;
//     pthread_mutex_t mutex;
//     pthread_cond_t can_produce;
//     pthread_cond_t can_consume;
// };

// Usage_buffer* data_buffer_new(void){
//     Usage_buffer* ub = calloc(1, sizeof(Usage_buffer));
//     pthread_mutex_init(&ub->mutex,NULL);
//     pthread_cond_init(&ub->can_produce,NULL);
//     pthread_cond_init(&ub->can_consume,NULL);

//     return ub;
// }
// void data_buffer_delete(Data_buffer* db){
//     pthread_cond_destroy(&db->can_consume);
//     pthread_cond_destroy(&db->can_produce);
//     pthread_mutex_destroy(&db->mutex);

//     free(db);
// }
// bool buffer_is_full(Data_buffer* db){
//     return db->size == CAPACITY;
// }
// bool buffer_is_empty(Data_buffer* db){
//     return db->size == 0;
// }
// void buffer_put(Data_buffer* db, Data* data){
//     if(buffer_is_full(db))return;

//     db->buffer[db->head] = data;
//     db->head = (db->head+1)%CAPACITY;
//     db->size++;
// }
// Data* buffer_get(Data_buffer* db){
//     if(buffer_is_empty(db))return NULL;

//     Data* d = db->buffer[db->tail];
//     db->tail = (db->tail + 1) % CAPACITY;
//     db->size--;

//     return d;
// }
// void buffer_lock(Data_buffer* db){
//     pthread_mutex_lock(&db->mutex);
// }
// void buffer_unlock(Data_buffer* db){
//     pthread_mutex_unlock(&db->mutex);
// }
// void buffer_call_producer(Data_buffer* db){
//     pthread_cond_signal(&db->can_produce);
// }
// void buffer_call_consumer(Data_buffer* db){
//     pthread_cond_signal(&db->can_consume);
// }
// void buffer_wait_for_producer(Data_buffer* db){
//     pthread_cond_wait(&db->can_consume, &db->mutex);
// }
// void buffer_wait_for_consumer(Data_buffer* db){
//     pthread_cond_wait(&db->can_produce,&db->mutex);
// }