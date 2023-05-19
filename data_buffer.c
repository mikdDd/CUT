#include "data_buffer.h"
#include <stdlib.h>
#include <pthread.h>
enum{CAPACITY = 5}; 
struct Data_buffer{
    Data buffer[CAPACITY];
    size_t size;
    size_t head;
    size_t tail;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
};

Data_buffer* datar_buffer_new(void){
    Data_buffer* db = calloc(1, sizeof(Data_buffer));
    pthread_mutex_init(&db->mutex,NULL);
    pthread_cond_init(&db->can_produce,NULL);
    pthread_cond_init(&db->can_consume,NULL);

    return db;
}
void data_buffer_delete(Data_buffer* printer);